/**
 * @file Arpegiator.cpp
 * @brief Defines the Arpegiator class methods
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "./Arpegiator.h"

#include <time.h>

namespace sacred
{
    namespace sounds
    {
        Arpegiator::Arpegiator(bool stereo, double thresh, Arpegiator_mode mode,  double range_down, double range_up, int stepsNumber)
            : SignalSource(stereo,1),
              m_mode(mode),
              m_hasStepped(false),
              m_thresh(thresh),
              m_range_down(std::min(1.,std::max(-1.,std::min(range_up,range_down)))),
              m_range_up(std::min(1.,std::max(-1.,std::max(range_up,range_down)))),
              m_step_size((range_up - range_down)/(stepsNumber-1)),
              m_stepsNumber(std::min(2,stepsNumber))
        {
        }

        void Arpegiator::setThreshold(double thresh)
        {
            m_thresh = thresh;
        }

        void Arpegiator::setMode(Arpegiator_mode mode)
        {
            m_mode = mode;
        }

        void Arpegiator::setRangeDown(double rangeDown)
        {
            m_range_down = rangeDown;
            updateStepSize();
        }

        void Arpegiator::setRangeUp(double rangeUp)
        {
            m_range_up = rangeUp;
            updateStepSize();
        }

        void Arpegiator::setStepsNumber(int stepsNumber)
        {
            m_stepsNumber = stepsNumber;
            updateStepSize();
        }

        void Arpegiator::updateStepSize()
        {
            m_step_size = (m_range_up - m_range_down)/(m_stepsNumber-1);
        }

        void Arpegiator::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                auto frame = m_inputs[0]->getFrame();
                for(unsigned int n=0; n<FRAME_SIZE; ++n)
                {
                    frame->samples[i][n] = arpegyze(frame->samples[i][n]);
                    frame->samples[i][n] = arpegyze(frame->samples[i][n]);
                }
            }
        }

        SampleType Arpegiator::arpegyze(const SampleType sample_in)
        {
            if((sample_in >= m_thresh) && !m_hasStepped)
            {
                m_hasStepped = true;
                switch(m_mode)
                {
                    case random:
                        return stepRandom();
                    case up:
                        return stepUp();
                    case down:
                        return stepDown();
                }
            }
            else
            {
                if(sample_in < m_thresh)
                    m_hasStepped = false;
                return m_prev_step;
            }
        }

        SampleType Arpegiator::stepRandom()
        {
            return (rand()%m_stepsNumber) *m_step_size + m_range_down;
        }

        SampleType Arpegiator::stepUp()
        {
            if(fabs(m_range_up - m_prev_step) < 0.00001)
            {
                m_prev_step = m_range_down;
                return m_prev_step;
            }
            else
            {
                m_prev_step = m_prev_step + m_step_size;
                return m_prev_step;
            }
        }

        SampleType Arpegiator::stepDown()
        {
            if(fabs(m_range_down - m_prev_step) < 0.00001)
            {
                m_prev_step = m_range_up;
                return m_prev_step;
            }
            else
            {
                m_prev_step = m_prev_step + m_step_size;
                return m_prev_step;
            }
        }
    }
}
