/**
 * @file Spectrum.cpp
 * @brief Defines the Spectrum class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "Spectrum.h"

#include <iostream>

namespace sacred
{
    namespace sounds
    {
        Spectrum::Spectrum()
            : m_window{new HammingWindow()}, m_stereo(true)
        {
            initSpectrum();
        }

        Spectrum::~Spectrum()
        {
            #ifdef SINGLE_PRECISION
                fftwf_destroy_plan(m_plan[0]);
                fftwf_destroy_plan(m_plan[1]);
            #else
                fftw_destroy_plan(m_plan[0]);
                fftw_destroy_plan(m_plan[1]);
            #endif
        }

        void Spectrum::computeMagnitudes(const std::vector<SampleType>& spectrum, std::vector<SampleType>& magnitudes)
        {
            auto iterspec_up = spectrum.begin();
            auto iterspec_down = spectrum.rend();

            auto itermag = magnitudes.begin();

            while(itermag != magnitudes.end())
            {
                // magnitudes
                *itermag = std::sqrt(
                    (*iterspec_up) * (*iterspec_up) +
                    (*iterspec_down) * (*iterspec_down)
                );

                ++itermag;
                ++iterspec_up;
                --iterspec_down;
            }
        }

        void Spectrum::setWindow(Window& window)
        {
            *m_window = window;
            initSpectrum();
        }

        void Spectrum::initSpectrum()
        {
            std::size_t size = m_window->getSize();
            double freqstep(double(SAMPLE_RATE) / size);

            double f(0);
            m_frequencies.clear();

            for(int i=0; i<size/2; ++i)
            {
                m_frequencies.push_back(f);
                f += freqstep;
            }

            for(int i = 0; i<m_stereo; ++i)
            {
                m_spectrum[i] = std::vector<SampleType>(size, 0);
                m_magnitudes[i] = std::vector<SampleType>(size, 0);
                #ifdef SINGLE_PRECISION
                    m_plan[i] = fftwf_plan_r2r_1d(size, &(m_window->getData(i)[0]), &(m_spectrum[i][0]), FFTW_R2HC, FFTW_MEASURE);
                #else
                    m_plan[i] = fftw_plan_r2r_1d(size, &(m_window->getData(i)[0]), &(m_spectrum[i][0]), FFTW_R2HC, FFTW_MEASURE);
                #endif
            }
        }

        void Spectrum::pushSample(SampleType sample, bool s)
        {
            if(m_window->pushSample(sample,
 s))
            {
                #ifdef SINGLE_PRECISION
                    fftwf_execute(m_plan[s]);
                #else
                    fftw_execute(m_plan[s]);
                #endif
                computeMagnitudes(m_spectrum[s], m_magnitudes[s]);
            }
        }

        const std::vector<SampleType>& Spectrum::getFrequencies() const
        {
            return m_frequencies;
        }
        const std::vector<SampleType>& Spectrum::getSpectrum(bool s) const
        {
            return m_spectrum[s];
        }
        const std::vector<SampleType>& Spectrum::getMagnitudes(bool s) const
        {
            return m_magnitudes[s];
        }
    }
}
