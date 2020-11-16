#ifndef __SACRED_SAMPLE_FRAME__
#define __SACRED_SAMPLE_FRAME__

#include "FrameProcessing.h"
#include <memory>

namespace sacred
{
    namespace sounds
    {
        /**
         * @struct SampleFrame
         * @brief Frame object that stores samples data
         *
         * SampleFrame stores data processed by a SignalSource
         * It uses 2 pointers (one for each channel -> left & right)
         * They point to FRAME_SIZE samples each
         **/
        struct SampleFrame : public std::enable_shared_from_this<SampleFrame>
        {
            /** true if frame is stero, false otherwise **/
            bool stereo;
            
            /** ID of the frame **/
            unsigned int number;
            
            /** mono (0) or stereo (0+1) samples **/
            SampleType *samples[2];

            /**
             * @fn SampleFrame(bool s, unsigned int n = 0)
             * @brief Builds a SampleFrame object
             * @param s : Stereo enabled
             * @param n : ID of the frame
             **/
            SampleFrame(bool s, unsigned int n = 0)
                : stereo{s}, number{n}
            {
                data = new SampleType[2*FRAME_SIZE]{0.0};
                samples[0] = data;
                samples[1] = &data[s*FRAME_SIZE];
            }

            virtual ~SampleFrame()
            {
                delete [] data;
            }
                
            std::shared_ptr<SampleFrame> getptr() {
                return shared_from_this();
            }

            /**
             * @fn setStereo(bool s)
             * @brief Sets stereo for the SampleFrame
             * @param s : Stereo enabled
             **/
            void setStereo(bool s)
            {
                samples[1] = &data[s*FRAME_SIZE];
                stereo = s;
            }

            //! mono[0:FRAME_SIZE] and stereo[FRAME_SIZE:2*FRAME_SIZE]
            SampleType *data;
        };

        typedef SampleType* Sample_ptr;
        typedef std::shared_ptr<SampleFrame> SampleFrame_ptr;
        typedef std::shared_ptr<SignalSource> SignalSource_ptr;

        /***************************************************************************
         *
         * Free-standing functions closely related to signals.
         *
         **************************************************************************/

        /**
        * Converts a given frequency into its Bark value.
        * This formula is taken from:
        *  H. Traunmüller (1990) "Analytical expressions for the tonotopic sensory scale" J. Acoust. Soc. Am. 88: 97-100.
        * and has been independently verified to be the one that best matches the band
        * frequencies defined by Zwicker in 1961.
        * @param f the input frequency, in Hz
        */
        inline SampleType hz2bark(const SampleType f) noexcept
        {
            SampleType b = ((26.81*f)/(1960 + f)) - 0.53;

            if (b < 2) b += 0.15*(2-b);
            if (b > 20.1) b += 0.22*(b - 20.1);

            return b;
        }



        /**
        * Converts a Bark value into its corresponding frequency.
        * This formula is deduced from:
        *  H. Traunmüller (1990) "Analytical expressions for the tonotopic sensory scale" J. Acoust. Soc. Am. 88: 97-100.
        * and has been independently verified to be the one that best matches the band
        * frequencies defined by Zwicker in 1961.
        * @param z the critical band rate, in Bark
        */
        inline SampleType bark2hz(SampleType z) noexcept
        {
            // note: these conditions have been deduced by inverting the ones from hz2bark

            if (z < 2) z = (z - 0.3) / 0.85;
            if (z > 20.1) z = (z - 4.422) / 1.22;

            // this part comes from Traunmüller's paper

            return 1960.0 * (z + 0.53) / (26.28 - z);
        }

        inline SampleType barkCriticalBandwidth(SampleType z) noexcept
        {
            return 52548.0 / (z*z - 52.56 * z + 690.39);
        }
    }
}

#endif
