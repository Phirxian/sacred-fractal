#ifndef SACRED_SPECTRUM
#define SACRED_SPECTRUM

/**
 * @file Spectrum.h
 * @brief Declares the Spectrum class
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../window/Window.h"
#include "../window/HammingWindow.h"
#include "../SignalSource.h"

#include <fftw3.h>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class Spectrum
         * @brief The class that implements the FFTW algorithm for spectrum computing using a sliding window
         **/
        class Spectrum
        {
            public:
                Spectrum();
                virtual ~Spectrum();

                /**
                 * @fn void setWindow(Window& window)
                 * @brief Sets the window used for computing the spectrum
                 * @param window : The window to set
                 **/
                void setWindow(Window& window);

                /**
                 * @fn const std::vector<double\>& getSpectrum() const
                 * @param @s : left/mono in false, or right/stereo in true
                 * @brief Lets user get the spectrum of the left data of the window
                 * @return The spectrum of the left data of the window
                 **/
                const std::vector<SampleType>& getSpectrum(bool s) const;

                /**
                 * @fn const std::vector<double\>& getFrequencies() const
                 * @brief Lets user get the frequencies of the spectrum organized in the increasing direction
                 * @return The frequencies of the spectrum organized in the increasing direction
                 **/
                const std::vector<SampleType>& getFrequencies() const;

                /**
                 * @fn const std::vector<double\>& getMagnitudes_left() const
                 * @param @s : left/mono in false, or right/stereo in true
                 * @brief Lets the user get the magnitudes of the left channel corresponding to the frequency bins
                 *        ( see getFrequencies() )
                 * @return The magnitudes of the left channel corresponding to the frequency bins
                 **/
                const std::vector<SampleType>& getMagnitudes(bool s) const;

                /**
                 * @fn void pushSample_left(SampleType sample)
                 * @brief Pushes a sample in the left buffer of the window
                 * @param @sample : The sample to push
                 * @param @s : left/mono in false, or right/stereo in true
                 **/
                void pushSample(SampleType sample, bool s);

            protected:
                bool m_stereo;

                /**
                 * @fn void initSpectrum
                 * @brief Initializes the attributes necessary for spectrum computing
                 **/
                void initSpectrum();

                /**
                 * @fn void computeMagnitudes(const std::vector<double\>& spectrum, std::vector<double\>& magnitudes)
                 * @brief Computes the magnitudes of the spectrum of the window
                 **/
                void computeMagnitudes(const std::vector<SampleType>& spectrum, std::vector<SampleType>& magnitudes);

                std::shared_ptr<Window> m_window; /** Window containing data to run the FFT **/


                /** Spectrum of the channel of the window
                 * Storage order for spectrum :
                 * r0, r1, r2, ..., rn/2, i(n+1)/2-1, ..., i2, i1
                 **/
                std::vector<SampleType> m_spectrum[2];

                /** Increasing ordered frequency bins **/
                std::vector<SampleType> m_frequencies;

                /** Increasing ordered magnitudes, corresponding to the fequency bins **/
                std::vector<SampleType> m_magnitudes[2];

                /** Object used to run the FFTW algorithm **/
                #ifdef SINGLE_PRECISION
                    fftwf_plan m_plan[2];
                #else
                    fftw_plan m_plan[2];
                #endif
        };
    }
}

#endif
