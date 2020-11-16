/**
 * @file SawGenerator.cpp
 * @brief Defines the SawGenerator class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "SawGenerator.h"

namespace sacred
{
    namespace sounds
    {
        SawGenerator::SawGenerator(double f, double a, double ph, bool stereo)
            : Generator(f, a, ph, stereo)
        {
        }

        SampleType SawGenerator::genSample(double time)
        {
            double u{(time/m_oscPeriod)}; //changing variable
            return 2*u*m_oscAmplitude - m_oscAmplitude;
        }
    }
}
