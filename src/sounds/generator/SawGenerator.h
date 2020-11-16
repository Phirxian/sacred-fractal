#ifndef __SACRED_SAWGENERATOR__
#define __SACRED_SAWGENERATOR__

/**
 * @file SawGenerator.h
 * @brief Declares the SawGenerator class
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
         * @class SawGenerator
         * @brief A Generator of saw waveforms
         **/
        class SawGenerator : public Generator
        {
            public:
                SawGenerator(double f=1000.0, double a=1.0, double ph=0.0, bool stereo=true);
                virtual const char* getTypeName() const { return "Saw"; }
            protected:
                SampleType genSample(double time) override;
        };
    }
}

#endif


