#ifndef __SACRED_FLATTOP_WINDOW__
#define __SACRED_FLATTOP_WINDOW__

/**
 * @file FlattopWindow.h
 * @brief Defines the FlattopWindow class
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
         * @class FlattopWindow
         * @brief A Window with the flat top shape
         **/
        class FlattopWindow : public Window
        {
            public:
                FlattopWindow() : Window()
                {
                    initWindowShape();
                }

            protected:
                void initWindowShape()
                {
                    for(int n=0; n<m_size; n++)
                    {
                        float count = float(m_size - 1);

                        float a = 1.930 * std::cos(2.0 * M_PI * n / count);
                        float b = 1.290 * std::cos(4.0 * M_PI * n / count);
                        float c = 0.388 * std::cos(6.0 * M_PI * n / count);
                        float d = 0.0322 * std::cos(8.0 * M_PI * n / count);

                        m_windowshape.push_back(1.0 - a + b - c +d);
                    }
                }
        };
    }
}

#endif
