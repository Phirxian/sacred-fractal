#ifndef __SACRED_BARLETT_WINDOW__
#define __SACRED_BARLETT_WINDOW__

/**
 * @file BarlettWindow.h
 * @brief Defines the BarlettWindow class
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
         * @class BarlettWindow
         * @brief A Window with the Barlett shape
         **/
        class BarlettWindow : public Window
        {
            public:
                BarlettWindow() : Window()
                {
                    initWindowShape();
                }
            protected:
                void initWindowShape()
                {
                    for(int n=0; n<m_size; n++)
                    {
                        float count = double(m_size - 1);
                        float a = 2.0 * std::fabs(n - (m_size - 1) / 2.0) / count;
                        m_windowshape.push_back(1.0 - a);
                    }
                }
        };
    }
}

#endif
