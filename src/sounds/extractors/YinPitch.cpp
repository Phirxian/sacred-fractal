
/**
 * @file YinPitch.cpp
 * @brief Defines the YinPitch class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 * 
 **/

#include "./YinPitch.h"
#include "../SignalSource.h"
#include "../tools/PeakDetection.h"
#include "../FrameProcessing.h"
#include <math.h>
#include <algorithm>

namespace sacred
{
    namespace sounds
    {
        YinPitch::YinPitch(bool stereo, double minFreq, double maxFreq, bool interpolate, double tolerance)
            : SignalSource(stereo,1), m_minFreq{minFreq}, m_maxFreq{maxFreq},
              m_interpolate{interpolate}, m_tolerance{std::min(MIN_TOLERANCE, tolerance)}, m_window{new RectangularWindow()}
        {
            updateParams();
        }
        
        YinPitch::~YinPitch()
        {
        }
        
        void YinPitch::updateParams()
        {
            int frameSize = m_window->getSize();
            m_tauMax = std::min(int(ceil(SAMPLE_RATE / m_minFreq)), frameSize/2);
            m_tauMin = std::min(int(floor(SAMPLE_RATE / m_maxFreq)), frameSize/2);
            
            for (int i = 0; i <= 1; ++i)
            {
                m_yin[i].resize(frameSize/2 + 1);
                m_pitch[i] = 1.;
                
                m_peakDetectLocal[i].setInterpolate(m_interpolate);
                m_peakDetectLocal[i].setRange(frameSize/2 + 1);
                m_peakDetectLocal[i].setMaxPeaks(1);
                m_peakDetectLocal[i].setMinPos(m_tauMin);
                m_peakDetectLocal[i].setMaxPos(m_tauMax);
                m_peakDetectLocal[i].setOrderBy("position");
                m_peakDetectLocal[i].setThreshold(-1 * m_tolerance);
                
                m_peakDetectGlobal[i].setInterpolate(m_interpolate);
                m_peakDetectGlobal[i].setRange(frameSize/2 + 1);
                m_peakDetectGlobal[i].setMaxPeaks(1);
                m_peakDetectGlobal[i].setMinPos(m_tauMin);
                m_peakDetectGlobal[i].setMaxPos(m_tauMax);
                m_peakDetectGlobal[i].setOrderBy("amplitude");
            }
        }

        void YinPitch::setMinFreq(double minFreq)
        {
            m_minFreq = minFreq;
            updateParams();
        }
        
        void YinPitch::setMaxFreq(double maxFreq)
        {
            m_maxFreq = maxFreq;
            updateParams();
        }
        
        void YinPitch::setInterpolate(bool interpolate)
        {
            m_interpolate = interpolate;
        }
        
        void YinPitch::setTolerance(double tolerance)
        {
            m_tolerance = tolerance;
        }

        void YinPitch::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;
            
            auto frame = m_inputs[0]->getFrame();
            
            bool update[2];
            
            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                for(int n=0; n<FRAME_SIZE; ++n)
                {
                    update[i] |= m_window->pushSample(frame->samples[i][n], i);
                }
                
                if (update[i])
                {
                    computePitch(m_window->getData(i), m_pitch[i], m_pitchConfidence[i], m_yin[i], m_positions[i], m_amplitudes[i], m_peakDetectLocal[i], m_peakDetectGlobal[i]);
                }
                
                for(int n=0; n<FRAME_SIZE; ++n)
                {
                    m_frame->samples[i][n] = m_pitch[i];
                }
            }
        }

        
        
        void YinPitch::computePitch(const std::vector<SampleType> &signal, SampleType &pitch,
                                          SampleType &pitchConfidence,
                                          std::vector<SampleType> &yin,
                                          std::vector<SampleType> &positions,
                                          std::vector<SampleType> &amplitudes,
                                          PeakDetection& peakDetectLocal,
                                          PeakDetection& peakDetectGlobal
                                         )
        {
            
            yin[0] = 1.;

            // Compute difference function
            for(int tau=1; tau < (int) yin.size(); ++tau)
            {
                yin[tau] = 0.;

                for(int j=0; j < (int) yin.size()-1; ++j)
                    yin[tau] += pow(signal[j] - signal[j+tau], 2);
            }

            // Compute a cumulative mean normalized difference function
            SampleType sum = 0.;

            for(int tau=1; tau < (int) yin.size(); ++tau)
            {
                sum += yin[tau];
                yin[tau] = yin[tau] * tau / sum;

                // Cannot simply check for sum==0 because NaN will be also produced by
                // infinitely small values
                if(std::isnan(yin[tau]))
                    yin[tau] = 1;
            }

            // yin[tau] is equal to 1 in the case if the df value for
            // this tau is the same as the mean across all df values from 1 to tau
            // Detect best period
            SampleType period = 0.;
            SampleType yinMin = 0.;

            // Select the local minima below the absolute threshold with the smallest
            // period value. Use global minimum if no minimas were found below threshold.

            // invert yin values because we want to detect the minima while PeakDetection
            // detects the maxima

            for(int tau=0; tau < (int) yin.size(); ++tau)
                yin[tau] = -yin[tau];

            peakDetectLocal.compute(yin, amplitudes, positions);

            if(positions.size())
            {
                period = positions[0];
                yinMin = -amplitudes[0];
            }
            else
            {
                // no minima found below the threshold --> find the global minima
                peakDetectGlobal.compute(yin, amplitudes, positions);

                if(positions.size())
                {
                    period = positions[0];
                    yinMin = -amplitudes[0];
                }
            }

            // NOTE: not sure if it is faster to run peak detection once to detect many
            // peaks and process the values manually instead of running peak detection
            // twice and detecting only two peaks.

            // Treat minimas with yin values >= 1 and respective negative confidence values
            // as completely unreliable (in the case if they occur in results).

            if(period)
            {
                pitch = SAMPLE_RATE / period;
                pitchConfidence = 1. - yinMin;

                if(pitchConfidence < 0)
                    pitchConfidence = 0.;
            }
            else
            {
                pitch = 0.;
                pitchConfidence = 0.;
            }
            
        }
    }
}
