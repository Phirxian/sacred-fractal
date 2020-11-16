#ifndef __SACRED_GAUSSIAN_WINDOW__
#define __SACRED_GAUSSIAN_WINDOW__

/**
 * @file GaussianWindow.h
 * @brief Defines the GaussianWindow class
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
         * @class GaussianWindow
         * @brief A Window with the Gaussian shape
         **/
        class GaussianWindow : public Window
        {
            public:
                GaussianWindow()
                    : Window()
                {
                    initWindowShape();
                }
            protected:

                void initWindowShape() override
                {
                    double sigma = 0.5;
                    for(int n=0; n<m_size; n++)
                    {
                        float val = sigma * (m_size - 1.0)/2.0;
                        float pow = std::pow((n-(m_size - 1.0) / 2.0) / val, 2.0);
                        m_windowshape.push_back(std::exp((-0.5) * pow));
                    }
                }
        };
    }
}

#endif
