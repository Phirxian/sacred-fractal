/**
 * @file TriangleGenerator.h
 * @brief Declares the TriangleGenerator class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "TriangleGenerator.h"

namespace sacred
{
    namespace sounds
    {
        TriangleGenerator::TriangleGenerator(double f, double a, double ph, bool stereo)
            : Generator(f, a, ph, stereo)
        {
        }

        SampleType TriangleGenerator::genSample(double time)
        {
            if(time<m_oscPeriod/2)
            {
                double u = time*2/m_oscPeriod; // changing variable
                return 2*u*m_oscAmplitude -m_oscAmplitude;
            }
            else
            {
                double u = (time-m_oscPeriod/2)*2/m_oscPeriod; // changing variable
                return m_oscAmplitude - 2*u*m_oscAmplitude;
            }
        }
    }
}
