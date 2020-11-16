#ifndef __SACRED_BOUNCE_GENERATOR__
#define __SACRED_BOUNCE_GENERATOR__

/**
 * @file BounceGenerator.h
 * @brief Declares the BounceGenerator class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "Generator.h"

namespace sacred
{
    namespace sounds
    {
        /**
         * @class BounceGenerator
         * @brief A Generator that makes bouncy waveforms :)
         **/
        class BounceGenerator : public Generator
        {
            public:
                BounceGenerator(double f=1000.0, double a=1.0, double ph=0.0, bool stereo=true, double b = 1.0);

                virtual const char* getTypeName() const { return "Bounce"; }
            protected:
                virtual SampleType genSample(double time);
                double m_bounce_cooked;
        };
    }
}


#endif
