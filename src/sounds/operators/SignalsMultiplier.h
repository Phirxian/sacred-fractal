#ifndef __SACRED_SIGNALS_MULTIPLIER___
#define __SACRED_SIGNALS_MULTIPLIER___

/**
 * @file SignalsMultiplier.h
 * @brief Declares the SignalsMultiplier class
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
         * @class SignalsMultiplier
         * @brief The SignalSource class that allows to multiply signals together
         **/
        class SignalsMultiplier : public SignalSource
        {
            public:
                SignalsMultiplier();
                virtual ~SignalsMultiplier();

                virtual const char* getTypeName() const { return "Multiplier"; }

                void process() noexcept override;
        };
    }
}

#endif
