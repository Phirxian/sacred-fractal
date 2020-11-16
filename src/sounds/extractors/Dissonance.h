#ifndef __SACRED_DISSONANCE__
#define __SACRED_DISSONANCE__

/**
 * @file Dissonance.h
 * @brief Declares the Dissonance class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../transform/Spectrum.h"
#include "../SignalSource.h"
#include <vector>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class Dissonance
         * @brief The SignalSource class that outputs the "roughness" of its input
         **/
        class Dissonance : public SignalSource
        {
            public:

                /**
                 * @def static const int DEFAULT_THRESHOLD
                 * @brief The default value for the threshold of the magnitude peaks
                 **/
                static const int DEFAULT_THRESHOLD = 30;

            public:
                Dissonance(bool stereo = false, double th = DEFAULT_THRESHOLD);
                ~Dissonance();

                void setWindow(Window& window);

                virtual const char* getTypeName() const { return "Dissonance"; }

                void process() noexcept override;

            protected:
                SampleType aWeighting(FrequencyType f);
                SampleType plompLevelt(FrequencyType df);
                SampleType consonance(SampleType f1, SampleType f2);

                /**
                 * @fn SampleType calcDissonance(const std::vector<SampleType>& frequencies, const std::vector<SampleType>& magnitudes)
                 * @brief Computes the dissonance given the list of frequencies and magnitudes
                 **/
                SampleType calcDissonance(const std::vector<SampleType>& frequencies, const std::vector<SampleType>& magnitudes);

                /**
                 * @fn void computeMagnitudeAndFrequencyPeaks_stereo()
                 * @brief   Computes magnitude and frequency peaks
                 **/
                void computeMagnitudeAndFrequencyPeaks();

                void setThreshold(double threshold);


            protected:

                /** The spectrum used to compute dissonance **/
                Spectrum m_spectrum;

                /** The threshold for the magnitude peaks **/
                double m_threshold;

                /** The increasingly ordered frequencies corresponding to magnitude peaks in the spectrum - left/right **/
                std::vector<SampleType> m_frequencyPeaks[2];

                /** The increasingly magnitude peaks in the spectrum - left/right **/
                std::vector<SampleType> m_magnitudePeaks[2];
        };
    }
}

#endif
