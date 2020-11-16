#ifndef __SACRED_SQUAREGENERATOR__
#define __SACRED_SQUAREGENERATOR__

/**
 * @file SquareGenerator.h
 * @brief Declares the SquareGenerator class
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
         * @class SquareGenerator
         * @brief A Generator for square waveforms
         **/
        class SquareGenerator : public Generator
        {
            public:
                SquareGenerator(double f=1000.0, double a=1.0, double ph=0.0, bool stereo=true);
                virtual const char* getTypeName() const { return "Square"; }
            protected:
                SampleType genSample(double time) override;
        };
    }
}

#endif
