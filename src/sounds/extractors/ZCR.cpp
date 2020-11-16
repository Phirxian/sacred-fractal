/**
 * @file ZCR.cpp
 * @brief Defines the ZCR class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "./ZCR.h"

namespace sacred
{
    namespace sounds
    {
        ZCR::ZCR()
            : SignalSource(false,1)
        {
        }

        ZCR::~ZCR()
        {
        }

        void ZCR::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;

            auto frame = m_inputs[0]->getFrame();

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                zcr[i] = computeZCR(frame->samples[i]);
                std::fill(m_frame->samples[i], m_frame->samples[i]+FRAME_SIZE, zcr[i]);
            }
        }

        SampleType ZCR::computeZCR(Sample_ptr samples)
        {
            SampleType zcr{0};
            SampleType prev{samples[0]};

            for(unsigned int n=1; n<FRAME_SIZE; ++n)
            {
                zcr += (samples[n]*prev <= 0 ? 1 : 0);
                prev = samples[n];
            }

            zcr /= FRAME_SIZE;
            return zcr;
        }
    }
}
