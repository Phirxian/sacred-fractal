#ifndef __SACRED_ARPEGIATOR__
#define __SACRED_ARPEGIATOR__

/**
 * @file Arpegiator.h
 * @brief Declares the Arpegiator class and other objects closely related
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"
#include <algorithm>

namespace sacred
{
    namespace sounds
    {
        /**
         * @enum Arpegiator_mode
         * @brief The mode avaible for the Arpegiator objects
         **/
        enum Arpegiator_mode : unsigned char
        {
            /** Random steps **/
            random = 0,
            /** Increasing ordered steps **/
            up = 1,
            /** Decreasing ordered steps **/
            down = 2
        };

        /**
         * @class Arpegiator
         * @brief A SignalSource that outputs a new step according to a mode each time dynamic hits
         **/
        class Arpegiator : public SignalSource
        {
            public:
                Arpegiator(bool stereo = true, double thresh = 0.7, Arpegiator_mode mode = random, double range_down = -1., double range_up = 1., int stepsNumber = 8);
                virtual ~Arpegiator();

                virtual const char* getTypeName() const { return "Arpegiator"; }

                void process() noexcept override;

                void setThreshold(double thresh);

                void setMode(Arpegiator_mode mode);

                void setRangeDown(double rangeDown);

                void setRangeUp(double rangeUp);

                void setStepsNumber(int stepsNumber);


            protected:

                /** Mode of the Arpegiator **/
                Arpegiator_mode m_mode;

                /** True if dynamic has already made the Arpegiator step, false otherwise **/
                bool m_hasStepped;

                /** Threshold of the dynamic **/
                double m_thresh;

                /** Value of the lower step **/
                double m_range_down;

                /** Value of the upper step **/
                double m_range_up;

                /** Size between two steps **/
                double m_step_size;

                /** Number of steps to take between the range, including m_range_down & m_range_up **/
                int m_stepsNumber;

                /** The previous step taken **/
                int m_prev_step;

            private:
                /**
                 * @fn SampleType arpegyze(const SampleType sample_in)
                 * @brief Processes the sample in input and ouputs the right step value
                 * @param sample_in : The sample in input
                 * @return The output sample : the right step value
                 **/
                SampleType arpegyze(const SampleType sample_in);

                /**
                 * @fn SampleType stepRandom()
                 * @brief Takes a random step value
                 * @return A random step value
                 *
                 * There is m_stepsNumber values possible and they are taken in the range of [m_range_down, m_range_up]
                 **/
                SampleType stepRandom();

                /**
                 * @fn SampleType stepUp()
                 * @brief Takes the upper step value
                 * @return The upper step value
                 *
                 * There is m_stepsNumber values possible and they are taken in the range of [m_range_down, m_range_up]
                 * If the previous step taken had the value m_range_up, then it continues from bottom
                 **/
                SampleType stepUp();

                /**
                 * @fn SampleType stepDown()
                 * @brief Takes the lower step value
                 * @return The lower step value
                 *
                 * There is m_stepsNumber values possible and they are taken in the range of [m_range_down, m_range_up]
                 * If the previous step taken had the value m_range_down, then it continues from top
                 **/
                SampleType stepDown();

                /**
                 * @fn void updateStepSize()
                 * @brief Re-computes the step size with range up & down
                 **/
                void updateStepSize();
        };
    }
}

#endif
