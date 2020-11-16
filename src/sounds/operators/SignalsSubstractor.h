#ifndef __SACRED_SIGNALS_SUBSTRACTOR___
#define __SACRED_SIGNALS_SUBSTRACTOR___

/**
 * @file SignalsSubstractor.h
 * @brief Declares the SignalsSubstractor class
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
         * @class SignalsSubstractor
         * @brief The SignalSource class that allows to susbtract signals together
         *
         * Each input has amplitude divided by the number of inputs before substraction
         **/
        class SignalsSubstractor : public SignalSource
        {
            public:
                SignalsSubstractor();
                virtual ~SignalsSubstractor();

                virtual const char* getTypeName() const { return "Substractor"; }

                void process() noexcept override;
        };
    }
}

#endif
