#ifndef __SACRED_HAMMING_WINDOW__
#define __SACRED_HAMMING_WINDOW__

/**
 * @file HammingWindow.h
 * @brief Defines the HammingWindow class
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
         * @class HammingWindow
         * @brief A Window with the Hamming shape
         **/
        class HammingWindow : public Window
        {
            public:
                HammingWindow() : Window()
                {
                    initWindowShape();
                }

            protected:
                void initWindowShape() override
                {
                    for(int n=0; n<m_size; n++)
                    {
                        float val = std::cos(2.0 * M_PI * n / double(m_size - 1));
                        m_windowshape.push_back(0.53836 - 0.46164 * val);
                    }
                }
        };
    }
}

#endif
