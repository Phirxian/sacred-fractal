#ifndef __SACRED_HANN_WINDOW__
#define __SACRED_HANN_WINDOW__

/**
 * @file HannWindow.h
 * @brief Defines the HannWindow class
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
         * @class HannWindow
         * @brief A Window with the Hann shape
         **/
        class HannWindow : public Window
        {
            public:
                HannWindow() : Window()
                {
                    initWindowShape();
                }
            protected:
                void initWindowShape() override
                {
                    for(int n=0; n<m_size; n++)
                    {
                        float val = std::cos(2.0 * M_PI * n / double(m_size - 1));
                        m_windowshape.push_back(0.5 * (1.0 - val));
                    }
                }
        };
    }
}

#endif
