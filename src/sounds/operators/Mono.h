#ifndef __SACRED_MONO__
#define __SACRED_MONO__

/**
 * @file Mono.h
 * @brief Declares the Mono class
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
         * @class Mono
         * @brief SignalSource that converts a signal to mono
         **/
        class Mono : public SignalSource
        {
            public:
                Mono();
                virtual ~Mono();

                virtual const char* getTypeName() const { return "Mono"; }

                void process() noexcept override;
        };
    }
}

#endif
