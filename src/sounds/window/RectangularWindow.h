#ifndef __SACRED_RECTANGULAR_WINDOW__
#define __SACRED_RECTANGULAR_WINDOW__

/**
 * @file RectangularWindow.h
 * @brief Defines the RectangularWindow class
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
         * @class RectangularWindow
         * @brief A Window with a rectangular shape
         **/
        class RectangularWindow : public Window
        {
            public:
                RectangularWindow()
                    : Window()
                {
                    initWindowShape();
                }
            protected:
                void initWindowShape() override
                {
                    m_windowshape = std::vector<SampleType>(m_size, 1.f);
                }
        };
    }
}

#endif
