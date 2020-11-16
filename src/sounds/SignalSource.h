#ifndef __SACRED_SIGNAL_SOURCE__
#define __SACRED_SIGNAL_SOURCE__

/**
 * @file SignalSource.h
 * @brief Declares the SignalSource class and other objects closely related
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/


#include <cstddef>
#include <iterator>
#include <utility>
#include <memory>

#include "SampleFrame.h"

namespace sacred
{
    namespace sounds
    {
        /**
        * @class SignalSource
        * @brief Abstract class representing signals
        *
        * Abstract class representing signals.
        * Each SignalSource has zero or more inputs that are other SignalSources
        * SignalSource processes the data from its input and stores them into its SampleFrame
        **/
        class SignalSource : public FrameProcessing, public std::enable_shared_from_this<SignalSource>
        {
            public:
                SignalSource(bool s = true, int inputsNumber = 0);
                virtual ~SignalSource();

                virtual const char* getTypeName() const { return "SignalSource"; }

                std::shared_ptr<SignalSource> getptr() {
                    return shared_from_this();
                }

                // ---------------- Source manager ---------------------

                /**
                 * @fn virtual const SignalSource_ptr& operator[](int pos) const
                 * @brief Returns a reference to the input at specified location pos.
                 * @param pos : The position of the input to return
                 * @return Requested reference to the input
                 **/
                virtual const SignalSource_ptr& operator[](int pos) const;

                /**
                 * @fn virtual const SignalSource_ptr& operator[](int pos) const
                 * @brief Returns a reference to the input at specified location pos.
                 * @param pos : The position of the input to return
                 * @return Requested reference to the input
                 **/
                virtual const SignalSource_ptr& get(int pos) const;

                /**
                 * @fn virtual SignalSource_ptr& operator[](int pos)
                 * @brief Returns a reference to the input at specified location pos.
                 * @param pos : The position of the input to return
                 * @return Requested reference to the input
                 **/
                virtual SignalSource_ptr& operator[](int pos);

                /**
                 * @fn virtual SignalSource_ptr& operator[](int pos)
                 * @brief Returns a reference to the input at specified location pos.
                 * @param pos : The position of the input to return
                 * @return Requested reference to the input
                 **/
                virtual SignalSource_ptr& get(int pos);

                /**
                 * @fn int getInputsNumber() const
                 * @brief Returns the number of inputs reserved
                 * @return The number of inputs reserved
                 **/
                int getInputsNumber() const;

                /**
                 * @fn virtual void addInput(std::shared_ptr<SignalSource\> source)
                 * @brief Adds an input to the SignalSource input list
                 * @param source : Input to add
                 **/
                virtual void addInput(SignalSource_ptr source);

                /**
                 * @fn virtual void removeInput(std::shared_ptr<SignalSource\> source)
                 * @brief Removes an input to the SignalSource input list
                 * @param source : Input to remove
                 */
                virtual void removeInput(SignalSource_ptr source);


                inline SignalSource_ptr operator[](std::size_t i) { return m_inputs[i]; }

                // ---------------- Data processing ---------------------

                /**
                 * @fn virtual void setStereo(bool s)
                 * @brief Sets the stereo for the SignalSource
                 * @param s : Stereo enabled
                 **/
                virtual void setStereo(bool s);

                /**
                 * @fn virtual bool isStereo() const
                 * @brief Tells if the SignalSource is stereo
                 * @return True if SignalSource is stereo, false otherwise
                 **/
                virtual bool isStereo() const;

                /**
                 * @fn virtual bool isFinished() const
                 * @brief Tells if the SignalSource is finished eg. will
                 *        ouput something else than zeros if frames are retrieved
                 * @return True if SignalSource is finished, false otherwise
                 **/
                virtual bool isFinished() const;

                // ---------------- Data processing ---------------------

                /**
                 * @fn virtual void processMonoFrame(const std::vector<SampleFrame_ptr\>& inputFrames) = 0
                 * @brief Does some processing using the input frames (specific for each SignalSource).
                 *        Mono version of the process
                 * @param inputFrames : The sample frames used for processing
                 **/
                virtual void process() noexcept = 0;

                /**
                 * @fn SampleFrame_ptr getProcessedFrame(const unsigned int frameNb)
                 * @brief Lets user get the SampleFrame of the SignalSource.
                 *        The SampleFrame is updated before return if the ID of the frame asked
                 *        corresponds to the next frame.
                 * @param frameNb : ID of the frame requested.
                 *                  frameNB = 0 corresponds to the frame BEFORE any processing (used for linking).
                 *                  The first real frame is frame 1.
                 * @return The latest processed SampleFrame of the SignalSource
                 **/
                SampleFrame_ptr getProcessedFrame(const unsigned int frameNb);

                inline SampleFrame_ptr getFrame() const noexcept { return m_frame; }

            protected:

                /** Stereo enabled for the SignalSource **/
                bool m_stereo;

                /** SampleFrame of the SignalSource **/
                SampleFrame_ptr m_frame;

                /** Inputs **/
                SignalSource_ptr *m_inputs;
                unsigned m_inputs_counts;
        };
    }
}

#endif // SIGNALSOURCE_H
