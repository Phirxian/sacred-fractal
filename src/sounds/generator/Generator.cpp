/**
 * @file Generator.cpp
 * @brief Defines the Generator class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "Generator.h"
#include <cassert>

namespace sacred
{
    namespace sounds
    {
        Generator::Generator(double f, double a, double ph, bool stereo)
            : SignalSource(stereo,0),
              m_oscFrequency{std::max(0.0,f)},
              m_oscAmplitude{std::max(std::min(a, 1.0), 0.0)},
              m_oscPhase{std::max(std::min(ph, 2*M_PI),0.0)}
        {
            m_samplesDelta = 1.0 / SAMPLE_RATE;
            updateParams();
        }

        Generator::~Generator()
        {
        }

        void Generator::updateParams()
        {
            m_oscPeriod = 1.0 / m_oscFrequency;
            m_time = m_oscPhase / 2*M_PI*m_oscFrequency;
        }

        void Generator::setFrequency(double f)
        {
            m_oscFrequency = std::max(0.0,f);
            updateParams();
        }

        void Generator::setAmplitude(double a)
        {
            m_oscAmplitude = std::max(std::min(a, 1.0), 0.0);
        }

        void Generator::setPhase(double ph)
        {
            m_oscPhase = std::max(std::min(ph, 2*M_PI),0.0);
            updateParams();
        }

        void Generator::changeFrequency(double f)
        {
            double period_new = 1.0 / f;
            m_time *=  period_new / m_oscPeriod;
            m_oscPeriod = period_new;
        }

        void Generator::process() noexcept
        {
            #pragma omp parallel for
            for(int i=0; i<=m_stereo; ++i)
            {
                for(unsigned int n=0; n<FRAME_SIZE; ++n)
                {
                    m_frame->samples[i][n] = genSample(m_time);
                    m_time = fmod(m_time + m_samplesDelta, m_oscPeriod);
                }
            }
        }
    }
}
