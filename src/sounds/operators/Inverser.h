#ifndef __SACRED_INVERSER__
#define __SACRED_INVERSER__

/**
 * @file Inverser.h
 * @brief Declares the Inverser class
 *
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
         * @class Inverser
         * @brief SignalSource that inverts the signal
         **/
        class Inverser : public SignalSource
        {
            public:
                Inverser();
                virtual ~Inverser();

                virtual const char* getTypeName() const { return "Inversser"; }

                void process() noexcept override;
        };
    }
}

#endif
