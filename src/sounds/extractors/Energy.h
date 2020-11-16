#ifndef __SACRED_ENERGY__
#define __SACRED_ENERGY__

/**
 * @file Energy.h
 * @brief Declares the Energy class
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
         * @class Energy
         * @brief The SignalSource class that ouputs the energy of its input
         **/
        class Energy : public SignalSource
        {
            public:
                Energy();
                virtual ~Energy();

                virtual const char* getTypeName() const { return "Energy"; }

                void process() noexcept override;

            protected:
                /**
                 * @fn SampleType computeAvgEnergy(Sample_ptr samples)
                 * @brief Computes the average energy in a list of samples
                 **/
                SampleType computeAvgEnergy(Sample_ptr samples);

                /** The average energy for the last frame - left/right **/
                SampleType avgEnergy[2];
        };
    }
}

#endif
