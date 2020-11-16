/**
 * @file SignalSource.cpp
 * @brief Defines the SignalSource class methods and other objects closely related
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "SignalSource.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <utility>
#include <vector>
#include <iterator>
#include <cassert>
#include <string.h>

#include<iostream>

namespace sacred
{
    namespace sounds
    {
        SignalSource::SignalSource(bool s, int inputsNumber)
            : m_stereo(s), m_frame(new SampleFrame(s))
        {
            m_inputs_counts = inputsNumber;
            m_inputs = new SignalSource_ptr[inputsNumber]();
        }

        SignalSource::~SignalSource()
        {
        }

        bool SignalSource::isFinished() const
        {
            bool isFinished = false;

            if(m_inputs_counts)
            {
                isFinished = true;
                for(int i = 0; i<m_inputs_counts; ++i)
                    isFinished &= m_inputs[i]->isFinished();
            }

            return isFinished;
        }

        void SignalSource::setStereo(bool s)
        {
            m_stereo = s;
            m_frame->setStereo(s);
        }

        bool SignalSource::isStereo() const
        {
            bool stereo = m_stereo;

            for(int i = 0; i< m_inputs_counts && !stereo; ++i)
                if(m_inputs[i] != nullptr)
                    stereo |= m_inputs[i]->isStereo();

            return m_stereo;
        }

        // ------------------------ Data Processing -------------------------

        SampleFrame_ptr SignalSource::getProcessedFrame(const unsigned int frameNb)
        {
            // if frame is not up to date, process frames from inputs and update
            // NB :     1st frameNb has to be 1 !!!!!!!!!
            //          frameNb = 0 corresponds to the frame BEFORE the beginning of the process
            if(frameNb - m_frame->number)
            {
                #pragma omp parallel for
                for(int i = 0; i<m_inputs_counts; ++i)
                    if(m_inputs[i] != nullptr)
                        m_inputs[i]->getProcessedFrame(frameNb);

                m_frame->number = frameNb;
                m_frame->setStereo(isStereo());

                process();
            }

            return m_frame;
        }

        // ----------------- Inputs & Outputs managing -----------------------
        
        
        const SignalSource_ptr& SignalSource::operator[](int pos) const
        {
            return m_inputs[pos];
        }
        
        const SignalSource_ptr& SignalSource::get(int pos) const
        {
            return m_inputs[pos];
        }
        
        SignalSource_ptr& SignalSource::operator[](int pos)
        {
            return m_inputs[pos];
        }
        
        SignalSource_ptr& SignalSource::get(int pos)
        {
            return m_inputs[pos];
        }
        
        int SignalSource::getInputsNumber() const
        {
            return m_inputs_counts;
        }

        void SignalSource::addInput(std::shared_ptr<SignalSource> source)
        {
        }

        void SignalSource::removeInput(std::shared_ptr<SignalSource> source)
        {
        }
    }
}
