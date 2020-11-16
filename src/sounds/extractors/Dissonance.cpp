#include "Dissonance.h"

#include <iostream>

namespace sacred
{
    namespace sounds
    {
        Dissonance::Dissonance(bool stereo, double th)
            : SignalSource(stereo,1), m_threshold(th)
        {
        }

        Dissonance::~Dissonance()
        {
        }

        void Dissonance::setThreshold(double threshold)
        {
            m_threshold = threshold;
        }

        void Dissonance::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;

            auto frame = m_inputs[0]->getFrame();

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                m_frequencyPeaks[i].clear();
                m_magnitudePeaks[i].clear();

                for(int n=0; n<FRAME_SIZE; ++n)
                    m_spectrum.pushSample(i, frame->samples[i][n]);
            }

            computeMagnitudeAndFrequencyPeaks();

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                SampleType dissonance(calcDissonance(m_frequencyPeaks[i], m_magnitudePeaks[i]));

                for(int n=0; n<FRAME_SIZE; ++n)
                    m_frame->samples[i][n] = dissonance;
            }
        }

        void Dissonance::computeMagnitudeAndFrequencyPeaks()
        {
            const std::vector<SampleType>& frequencies(m_spectrum.getFrequencies());

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                const std::vector<SampleType>& magnitudes(m_spectrum.getMagnitudes(i));
                auto it_mag(magnitudes.begin());

                for(auto f : frequencies)
                {
                    if(*it_mag > m_threshold)
                    {
                        m_frequencyPeaks[i].push_back(f);
                        m_magnitudePeaks[i].push_back(*it_mag);
                    }
                    ++it_mag;
                }
            }
        }

        SampleType Dissonance::aWeighting(FrequencyType f)
        {
            // from http://www.cross-spectrum.com/audio/weighting.html
            // 1.25893 = 2 dB

            return 1.25893*12200*12200*(f*f*f*f) / (
                       (f*f +20.6*20.6) *
                       (f*f +12200*12200) *
                       sqrt(f*f +107.7*107.7) *
                       sqrt(f*f +737.9*737.9)
                   );
        }

        SampleType Dissonance::plompLevelt(FrequencyType df)
        {
            // df is the frequency difference on with critical bandwidth as  a unit.
            // the cooeficients were fitted with a polynom
            // to the data from the plomp & Levelt 1965 publication
            // To verify the fit run this and plot with e.g. gnuplot

            if(df < 0) return 1;
            if(df > 1.18) return 1;

            SampleType res =
                -6.58977878 * df*df*df*df*df +
                28.58224226 * df*df*df*df +
                -47.36739986 * df*df*df +
                35.70679761 * df*df +
                -10.36526344 * df +
                1.00026609;

            if(res < 0) return 0;
            if(res > 1) return 1;

            return res;
        }

        SampleType Dissonance::consonance(SampleType f1, SampleType f2)
        {
            // critical bandwidth between f1, f2:
            // see  http://www.sfu.ca/sonic-studio/handbook/Critical_Band.html for a
            // definition of critical bandwidth between two partials of a complex tone:
            SampleType cbwf1 = barkCriticalBandwidth(hz2bark(f1));
            SampleType cbwf2 = barkCriticalBandwidth(hz2bark(f2));
            SampleType cbw = std::min(cbwf1, cbwf2);
            return plompLevelt(fabs(f2-f1)/cbw);
        }

        SampleType Dissonance::calcDissonance(const std::vector<SampleType>& frequencies, const std::vector<SampleType>& magnitudes)
        {
            std::vector<SampleType> loudness = magnitudes;
            SampleType totalLoudness = 0;
            int size = frequencies.size();

            // calculate dissonance
            for(int i = 0; i < size; i++)
            {
                // dBA-weighting
                // The factor should be applied to the amplitudes,
                // but we receive already the intensities (squared amplitudes),
                // thus, the factor is applied twice
                SampleType aWeightingFactor = aWeighting(frequencies[i]);
                loudness[i] *= aWeightingFactor * aWeightingFactor;
                totalLoudness += loudness[i];
            }


            if(totalLoudness == 0.0)
                return 0.0;

            //vector<SampleType> loudness(size);
            //for (int i=0; i<size; i++) partialLoudness = loudness[i]/totalLoudness;

            SampleType totalDissonance = 0;
            for(int p1 = 0; p1 < size; p1++)
            {
                if(frequencies[p1] > 50)    // ignore frequencies below 50 Hz
                {
                    SampleType barkFreq = hz2bark(frequencies[p1]);
                    SampleType startF = bark2hz(barkFreq - 1.18);
                    SampleType endF = bark2hz(barkFreq + 1.18);
                    int p2 = 0;
                    SampleType peakDissonance = 0;
                    while(p2 < size && frequencies[p2] < startF && frequencies[p2] < 50) p2++;
                    while(p2 < size && frequencies[p2] < endF && frequencies[p2] < 10000)
                    {
                        SampleType d = 1.0 - consonance(frequencies[p1], frequencies[p2]);
                        // Dissonance from p1 to p2, should be the same as dissonance from p2
                        // to p1, this is the reason for using both peaks' loudness as
                        // weight
                        if(d > 0) peakDissonance += d*(loudness[p2] + loudness[p1])/totalLoudness;
                        p2++;
                    }
                    SampleType partialLoudness = loudness[p1]/totalLoudness;
                    if(peakDissonance > partialLoudness) peakDissonance = partialLoudness;
                    totalDissonance += peakDissonance;
                }
            }
            // total dissonance is divided by two, because each peak from a pair
            // contributes to it
            return totalDissonance/2;
        }

        void Dissonance::setWindow(Window& window)
        {
            m_spectrum.setWindow(window);
        }
    }
}
