/**
 * @file SignalsAdditioner.cpp
 * @brief Defines the SignalsAdditioner class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "SignalsAdditioner.h"

#include <algorithm>
#include <cassert>

namespace sacred
{
    namespace sounds
    {
        SignalsAdditioner::SignalsAdditioner()
            : SignalSource(false,2)
        {
        }

        SignalsAdditioner::~SignalsAdditioner()
        {
        }

        void SignalsAdditioner::process() noexcept
        {
            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                for(unsigned short n=0; n<FRAME_SIZE; n++)
                {
                    double tmp = 0.0;
                    for(int i = 0; i<m_inputs_counts; ++i)
                        if(m_inputs[i] != nullptr)
                            tmp += m_inputs[i]->getFrame()->samples[i][n];
                    m_frame->samples[i][n] = tmp / m_inputs_counts;
                }
            }
        }
    }
}
