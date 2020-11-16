/**
 * @file SignalsMultiplier.cpp
 * @brief Defines the SignalsMultiplier class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "SignalsMultiplier.h"

namespace sacred
{
    namespace sounds
    {
        SignalsMultiplier::SignalsMultiplier()
            : SignalSource(false,2)
        {
        }

        SignalsMultiplier::~SignalsMultiplier()
        {
        }

        void SignalsMultiplier::process() noexcept
        {
            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                for(unsigned short n=0; n<FRAME_SIZE; n++)
                {
                    double tmp = 1.0;
                    for(int i = 0; i<m_inputs_counts; ++i)
                        if(m_inputs[i] != nullptr)
                            tmp *= m_inputs[i]->getFrame()->samples[i][n];
                    m_frame->samples[i][n] = tmp / m_inputs_counts;
                }
            }
        }
    }
}
