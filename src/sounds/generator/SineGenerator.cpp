/**
 * @file SineGenerator.cpp
 * @brief Defines the SineGenerator class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "SineGenerator.h"

namespace sacred
{
    namespace sounds
    {
        SineGenerator::SineGenerator(double f, double a, double ph, bool stereo)
            : Generator(f, a, ph, stereo)
        {
        }

        SampleType SineGenerator::genSample(double time)
        {
            return m_oscAmplitude * std::sin(2*M_PI*m_oscFrequency*time);
        }
    }
}
