#ifndef __SACRED_DYNAMICS__
#define __SACRED_DYNAMICS__

/**
 * @file Dynamics.h
 * @brief Declares the Dynamics class
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
         * @class Dynamics
         * @brief The SignalSource class that transforms a signal in input into dynamic data
         **/
        class Dynamics : public SignalSource
        {
            public:
                Dynamics(bool stereo = true, double threshup = 0.7, double threshdown = 0.1, double attack = 0.2, double release = 3.);

                virtual ~Dynamics();

                virtual const char* getTypeName() const { return "Dynamic"; }

                void process() noexcept override;

                void setThresholdUp(double threshup);

                void setThresholdDown(double threshdown);

                void setAttack(double attack);

                void setRelease(double release);


            protected:

                /** Threshold for the upper dynamic response **/
                double m_threshold_up;

                /** Threshold for the lower dynamic response **/
                double m_threshold_down;

                /** Attack of dynamic response in sec **/
                double m_attack;

                /** Release of dynamic response in sec **/
                double m_release;

                /** "cooked" version of release (maths...) **/
                double m_release_cooked;

                /** Delta in sec between two samples **/
                double m_sampleDelta;

                /** Shift in sec for sigmoid function to start at 0 **/
                double m_shift;

                /** Time elapsed used to count release and attack - left/right **/
                double m_time[2];

                /** Upper dynamic phase indicator - left/right **/
                bool m_dynup[2];

                /** Lower dynamic phase indicator - left/right **/
                bool m_dyndown[2];

                /** Linear attack phase indicator - left/right **/
                bool m_dynattack[2];

                /** Previous sample out for the left/right channel **/
                SampleType m_previous[2];

            private:
                /**
                 * @fn SampleType dynamize(const SampleType sample_in, SampleType& previous, double& time, bool& dynup, bool& dyndown, bool& dynattack)
                 * @brief Processes the dynamics
                 *
                 * @param sample_in : The input sample
                 * @param previous : The previous sample out
                 * @param time : The time in sec used for release and attack
                 * @param dynup : The upper dynamic phase indicator
                 * @param dyndown : The lower dynamic phase indicator
                 * @param dynattack : The linear attack phase indicator
                 *
                 * @return The dynamized sample
                 **/
                SampleType dynamize(const SampleType sample_in, SampleType& previous, double& time, bool& dynup, bool& dyndown, bool& dynattack);

                /**
                 * @fn SampleType sigmo(double& time)
                 * @brief The "cooked" sigmoid function
                 * @param time : Time in sec
                 * @return The sigmoid value
                 **/
                SampleType sigmo(double& time);

                /**
                 * @fn SampleType attack(double& time)
                 * @brief The linear attack function
                 * @param time : Time in sec
                 * @return The attack value
                 **/
                SampleType attack(double& time);


                /**
                 * @fn void updateParams()
                 * @brief Re-calculate cooked release from user release
                 */
                void cookRelease();
        };
    }
}

#endif
