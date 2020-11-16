#ifndef __SACRED_BLACKMAN_WINDOW__
#define __SACRED_BLACKMAN_WINDOW__

/**
 * @file BlackmanWindow.h
 * @brief Defines the BlackmanWindow class
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
         * @class BlackmanWindow
         * @brief A Window with the Blackman shape
         **/
        class BlackmanWindow : public Window
        {
            public:
                BlackmanWindow() : Window()
                {
                    initWindowShape();
                }
            protected:
                void initWindowShape()
                {
                    for(int n=0; n<m_size; n++)
                    {
                        float count = double(m_size - 1);
                        float a = 0.50 * std::cos(2.0 * M_PI * n / count);
                        float b = 0.08 * std::cos(4.0 * M_PI * n / count);
                        m_windowshape.push_back(0.42 - a + b);
                    }
                }
        };
    }
}

#endif
