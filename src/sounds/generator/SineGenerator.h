#ifndef __SACRED_SINEGENERATOR__
#define __SACRED_SINEGENERATOR__

/**
 * @file SineGenerator.h
 * @brief Declares the SineGenerator class
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
         * @class SineGenerator
         * @brief A Generator for sine waveforms
         **/
        class SineGenerator : public Generator
        {
            public:
                SineGenerator(double f=1000.0, double a=1.0, double ph=0.0, bool stereo=true);
                virtual const char* getTypeName() const { return "Sine"; }
            protected:
                SampleType genSample(double time) override;
        };
    }
}

#endif
