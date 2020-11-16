#ifndef __SACRED_WINDOW__
#define __SACRED_WINDOW__

/**
 * @file Window.h
 * @brief Defines the class for creating a sliding window
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"

namespace sacred
{
    namespace sounds
    {
        /**
         * @class Window
         * @brief The sliding window class
         **/
        class Window
        {
            public:
                /**
                 * @def static const int WINDOW_SIZE
                 * @brief The default size of a sliding window
                 *
                 * 2048 is the standard value for music analysis
                 **/
                static const int WINDOW_SIZE = 2048;

            public:
                Window()
                    : m_size(WINDOW_SIZE)
                {
                    m_data[0] = std::vector<SampleType>(m_size, 0);
                    m_data[1] = std::vector<SampleType>(m_size, 0);
                }

                /**
                 * @fn virtual bool pushSample(SampleType sample, bool s)
                 * @brief Pushes a sample in the right buffer and slides the window if needed
                 * @param sample : The sample to push
                 * @param @s : left/mono in false, or right/stereo in true
                 **/
                virtual bool pushSample(SampleType sample, bool s)
                {
                    bool update = false;
                    m_buffer[s].push_back(sample);

                    // buffer is full -> update the data
                    if(m_buffer[s].capacity() == m_buffer[s].size())
                    {
                        m_buffer[s]= shapeBufferToData(m_data[s], m_buffer[s]);
                        update = true;
                    }

                    return update;
                }

                /**
                 * @fn virtual std::size_t getSize() const
                 * @brief Lets user get the size of the window
                 * @return The size of the window
                 **/
                virtual std::size_t getSize() const
                {
                    return m_size;
                }

                /**
                 * @fn virtual std::vector<SampleType\>& getData_left()
                 * @brief Lets user get the right windowed data
                 * @param @s : left/mono in false, or right/stereo in true
                 * @return The right windowed data
                 **/
                virtual std::vector<SampleType>& getData(bool s)
                {
                    return m_data[s];
                }

            protected:
                /**
                 * @fn virtual void initWindowShape() = 0
                 * @brief Initilize the shape of the window in the m_windowshape vector
                 **/
                virtual void initWindowShape() = 0;

                /**
                 * @fn std::vector<SampleType\>& shapeBufferToData(std::vector<SampleType\>& data, std::vector<SampleType\>& buffer)
                 * @brief Shapes the buffer according to the window shape
                 * @param data : The object to store the shaped values
                 * @param buffer : The data to shape
                 * @return The shaped data ( = data )
                 **/
                std::vector<SampleType>& shapeBufferToData(std::vector<SampleType>& data, std::vector<SampleType>& buffer)
                {
                    auto iter_wshape = m_windowshape.begin();
                    auto iter_buffer = buffer.begin();

                    for(auto iter_data = data.begin();  iter_data != data.end(); ++iter_data)
                    {
                        *iter_data = (*iter_wshape) * (*iter_buffer);
                        ++iter_wshape;
                        ++iter_buffer;
                    }

                    buffer = moveBufferForward(buffer);
                }

                /**
                 * @fn std::vector<SampleType\>& moveBufferForward(std::vector<SampleType\>& buffer)
                 * @brief Moves the buffer forward ( slides the window )
                 * @param buffer : The buffer to move forward
                 * @return The forward moved buffer
                 **/
                std::vector<SampleType>& moveBufferForward(std::vector<SampleType>& buffer)
                {
                    buffer.assign(buffer.begin() + m_size/2, buffer.end());
                    return buffer;
                }

            protected:
                
                /** size of the window **/
                std::size_t m_size;
                
                /** shaped data to use **/
                std::vector<SampleType> m_data[2];
                
                /** buffer used to store incoming samples **/
                std::vector<SampleType> m_buffer[2];
                
                /** shape of the window **/
                std::vector<SampleType> m_windowshape;
        };
    }
}

#endif
