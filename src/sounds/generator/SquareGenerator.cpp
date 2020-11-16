/**
 * @file SquareGenerator.cpp
 * @brief Defines the SquareGenerator class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "SquareGenerator.h"

namespace sacred
{
    namespace sounds
    {
        SquareGenerator::SquareGenerator(double f, double a, double ph, bool stereo)
            : Generator(f, a, ph, stereo)
        {
        }

        SampleType SquareGenerator::genSample(double time)
        {
            return time < m_oscPeriod/2 ? m_oscAmplitude : -m_oscAmplitude;
        }
    }
}
