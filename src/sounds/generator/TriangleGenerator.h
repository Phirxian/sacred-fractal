#ifndef __SACRED_TRIANGLEGENERATOR__
#define __SACRED_TRIANGLEGENERATOR__

/**
 * @file TriangleGenerator.h
 * @brief Declares the TriangleGenerator class
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
         * @class TriangleGenerator
         * @brief A Generator for triangle waveforms
         **/
        class TriangleGenerator : public Generator
        {
            public:
                TriangleGenerator(double f=1000.0, double a=1.0, double ph=0.0, bool stereo=true);
                virtual const char* getTypeName() const { return "Triangle"; }
            protected:
                SampleType genSample(double time) override;
        };
    }
}

#endif
