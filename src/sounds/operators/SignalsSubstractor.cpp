#include "SignalsSubstractor.h"

/**
 * @file SignalsSubstractor.cpp
 * @brief Defines the SignalsSubstractor class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include <algorithm>
#include <cassert>

namespace sacred
{
    namespace sounds
    {
        SignalsSubstractor::SignalsSubstractor()
            : SignalSource(false,2)
        {
        }

        SignalsSubstractor::~SignalsSubstractor()
        {
        }

        void SignalsSubstractor::process() noexcept
        {
            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                for(unsigned short n=0; n<FRAME_SIZE; n++)
                {
                    double tmp = 0.0;
                    int odd = 0;

                    for(int i = 0; i<m_inputs_counts; ++i)
                    {
                        if(m_inputs[i] != nullptr)
                        {
                            float sign = 1.f - 2.f*(odd++&0x1);
                            tmp += sign*m_inputs[i]->getFrame()->samples[i][n];
                        }
                    }
                    m_frame->samples[i][n] = tmp;
                }
            }
        }
    }
}
