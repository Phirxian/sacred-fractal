#ifndef __SACRED_SIGNALS_ADDITIONER___
#define __SACRED_SIGNALS_ADDITIONER___

/**
 * @file SignalsAdditioner.h
 * @brief Declares the SignalsAdditioner class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"
#include <vector>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class SignalsAdditioner
         * @brief The SignalSource class that allows to add signals together
         *
         * Each input has amplitude divided by the number of inputs before sum
         **/
        class SignalsAdditioner : public SignalSource
        {
            public:
                SignalsAdditioner();
                virtual ~SignalsAdditioner();

                virtual const char* getTypeName() const { return "Additioner"; }

                void process() noexcept override;
        };
    }
}


#endif
