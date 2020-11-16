/**
 * @file BounceGenerator.cpp
 * @brief Defines the BounceGenerator class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "BounceGenerator.h"

namespace sacred
{
    namespace sounds
    {
        BounceGenerator::BounceGenerator(double f, double a, double ph, bool stereo, double b)
            : Generator(f, a, ph, stereo), m_bounce_cooked{1.0/b}
        {
        }

        SampleType BounceGenerator::genSample(double time)
        {
            return std::pow(std::abs(std::cos(2*M_PI*m_oscFrequency*time)), m_bounce_cooked) * m_oscAmplitude;
        }
    }
}
