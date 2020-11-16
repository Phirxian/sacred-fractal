/**
 * @file Energy.cpp
 * @brief Defines the Energy class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "./Energy.h"

#include <algorithm>

namespace sacred
{
    namespace sounds
    {
        Energy::Energy()
            : SignalSource(false,1)
        {
        }

        Energy::~Energy()
        {
        }

        void Energy::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;

            auto frame = m_inputs[0]->getFrame();

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                avgEnergy[i] = computeAvgEnergy(frame->samples[i]);
                std::fill(m_frame->samples[i], m_frame->samples[i]+FRAME_SIZE, avgEnergy[i]);
            }
        }

        SampleType Energy::computeAvgEnergy(Sample_ptr samples)
        {
            SampleType avgNrj{0};

            for(unsigned int n=0; n<FRAME_SIZE; ++n)
                avgNrj += samples[n]*samples[n];
            avgNrj /= FRAME_SIZE;

            return avgNrj;
        }
    }
}
