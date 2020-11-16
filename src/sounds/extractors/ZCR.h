#ifndef __SACRED_ZCR__
#define __SACRED_ZCR__

/**
 * @file ZCR.h
 * @brief Declares the ZCR class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"

namespace sacred
{
    namespace sounds
    {
        /**
         * @class ZCR
         * @brief The SignalSource class that computes the zero crossing rate of its input
         **/
        class ZCR : public SignalSource
        {
            public:
                ZCR();
                virtual ~ZCR();
                virtual const char* getTypeName() const { return "ZCR"; }

                void process() noexcept override;

            protected:
                /**
                 * @fn SampleType computeZCR(Sample_ptr samples)
                 * @brief Computes the ZCR of a list of samples
                 * @param samples : The list of samples
                 * @return Zero crossing rate
                 **/
                SampleType computeZCR(Sample_ptr samples);

                SampleType zcr[2];
        };
    }
}

#endif
