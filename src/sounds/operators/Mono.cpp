/**
 * @file Mono.cpp
 * @brief Defines the Mono class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "./Mono.h"

namespace sacred
{
    namespace sounds
    {
        Mono::Mono()
            : SignalSource(false,1)
        {
        }

        Mono::~Mono()
        {
        }

        void Mono::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;

            auto frame = m_inputs[0]->getFrame();

            #pragma omp parallel for
            for(int i=0; i<=m_stereo; ++i)
            {
                for(unsigned int n=0; n<FRAME_SIZE; ++n)
                    m_frame->samples[i][n] = (frame->samples[i][n] + frame->samples[i][n])/2;
            }
        }
    }
}
