#ifndef __SACRED_GENERATOR__
#define __SACRED_GENERATOR__

/**
 * @file Generator.h
 * @brief Declares the Generator class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"
#include <cmath>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class Generator
         * @brief The SignalSource class that generates a periodic waveform
         **/
        class Generator : public SignalSource
        {
            public:
                Generator(double f=1000.0, double a=1.0, double ph=0.0, bool stereo = true);

                virtual ~Generator();

                void setFrequency(double f);

                void setAmplitude(double a);

                void setPhase(double ph);



                /**
                 * @fn void changeFrequency(double f)
                 * @brief Allows to change the frequency in real time and keep the position on the unit circle
                 * @param f : The new frequency
                 **/
                void changeFrequency(double f);

                void process() noexcept override;

            protected:

                /**
                 * @fn virtual SampleType genSample(double time) = 0
                 * @brief Generates the sample that corresponds to the time
                 * @param time : Time in sec
                 **/
                virtual SampleType genSample(double time) = 0;

                /**
                 * @fn void updateParams()
                 * @brief Updates cooked parameters from user parameters
                 **/
                void updateParams();

            public:

                /** Time in sec with phase added **/
                double m_time;

                /** Delta in sec between 2 samples **/
                double m_samplesDelta;

                /** Frequency in Hz **/
                double m_oscFrequency;

                /** Period corresponding to m_oscFrequency **/
                double m_oscPeriod;

                /** Amplitude **/
                double m_oscAmplitude;

                /** Phase in radians **/
                double m_oscPhase;
        };
    }
}



#endif
