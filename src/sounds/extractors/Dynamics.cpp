
/**
 * @file Dynamics.cpp
 * @brief Defines the Dynamics class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 *
 **/

#include "Dynamics.h"
#include "../SignalSource.h"

#include <iostream>

namespace sacred
{
    namespace sounds
    {
        Dynamics::Dynamics(bool stereo, double threshup, double threshdown, double attack, double release):
            SignalSource(stereo,1),
            m_threshold_up{std::max(std::min(0.0, threshup),1.0)},
            m_threshold_down{std::max(std::min(0.0, threshdown),1.0)},
            m_attack{std::max(0.0,attack)},
            m_release{std::max(0.0,release)}
        {
            cookRelease();
            m_sampleDelta = 1.0 / SAMPLE_RATE;
        }
        
        void Dynamics::cookRelease()
        {
            double p{0.999};
            m_release_cooked = m_release / (-2*log(1-p));
            m_shift = log((1-p)/p) * m_release_cooked;
        }
        
        void Dynamics::setThresholdUp(double threshup)
        {
            m_threshold_up = std::max(std::min(0.0, threshup),1.0);
        }
        
        void Dynamics::setThresholdDown(double threshdown)
        {
            m_threshold_down = std::max(std::min(0.0, threshdown),1.0);
        }
        
        void Dynamics::setAttack(double attack)
        {
            m_attack = std::max(0.0,attack);
        }
        
        void Dynamics::setRelease(double release)
        {
            m_release = std::max(0.0,release);
            cookRelease();
        }

        Dynamics::~Dynamics()
        {
        }

        SampleType Dynamics::sigmo(double& time)
        {
            SampleType sample_out = 1.0 / (1+std::exp((time+m_shift)/m_release_cooked));
            time += m_sampleDelta;
            return sample_out;
        }

        SampleType Dynamics::attack(double& time)
        {
            SampleType sample_out = time/m_attack;
            time += m_sampleDelta;
            return sample_out;
        }

        SampleType Dynamics::dynamize(const SampleType sample_in, SampleType& sample_prev, double& time, bool& dynup, bool& dyndown, bool& dynattack)
        {
            SampleType sample_out = 0.0;

            if(std::abs(sample_in) > m_threshold_up)
            {
                dynup = true;
                dynattack = true;
                time = m_attack*sample_prev;
            }
            else if(std::abs(sample_in) < m_threshold_down)
            {
                dyndown = true;
                dynattack = true;
                time = -m_attack*sample_prev;
            }

            if(dynup)
            {
                if(dynattack)
                {
                    sample_out = attack(time);
                    if(time > m_attack)
                    {
                        time = 0.0;
                        dynattack = false;
                    }
                }
                else
                {
                    sample_out = sigmo(time);
                    if(time > m_release)
                    {
                        sample_out = 0.0;
                        dynup = false;
                    }
                }
            }
            else if(dyndown)
            {
                if(dynattack)
                {
                    sample_out = -attack(time);
                    if(time > m_attack)
                    {
                        time = 0.0;
                        dynattack = false;
                    }
                }
                else
                {
                    sample_out = -sigmo(time);
                    if(time > m_release)
                    {
                        sample_out = 0.0;
                        dynup = false;
                    }
                }
            }

            sample_prev = sample_out;

            return sample_out;
        }

        void Dynamics::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;
        
            auto frame = m_inputs[0]->getFrame();

            #pragma omp parallel for
            for(int i = 0; i<m_stereo; ++i)
            {
                for(unsigned int n=0; n<FRAME_SIZE; ++n)
                {
                    m_frame->samples[i][n] = dynamize(
                        frame->samples[i][n],
                        m_previous[i], m_time[i],
                        m_dynup[i], m_dyndown[i], m_dynattack[i]
                    );
                }
            }
        }
    }
}
