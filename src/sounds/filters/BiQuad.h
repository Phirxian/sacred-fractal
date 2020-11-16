#ifndef __SACRED_BIQUAD__
#define __SACRED_BIQUAD__

/**
 * @file BiQuad.h
 * @brief Declares the BiQuad class and other objects closely related
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 28 2017
 **/

#include "../SignalSource.h"

namespace sacred
{
    namespace sounds
    {
        /**
         * @enum BiQuad_type
         * @brief The type of the filter
         **/
        enum BiQuad_type : unsigned char
        {
            bq_type_lowpass = 0,
            bq_type_highpass,
            bq_type_bandpass,
            bq_type_notch,
            bq_type_peak,
            bq_type_lowshelf,
            bq_type_highshelf
        };

        /**
         * @class BiQuad
         * @brief The SignalSource class that allows to filter its input using the biquadric structure
         **/
        class BiQuad : public SignalSource
        {
            public:
                BiQuad();
                BiQuad(bool stereo, BiQuad_type type, double Fc, double Q, double peakGainDB);
                ~BiQuad();

                void setType(BiQuad_type type);
                void setQ(double Q);
                void setFc(double Fc);
                void setPeakGain(double peakGainDB);
                void setBiQuad(BiQuad_type type, double Fc, double Q, double peakGain);

                void process() noexcept;

                /**
                 * @fn SampleType filterSample(SampleType in)
                 * @brief Filters the input sample
                 * @param in : The input sample
                 * @return The filtered sample
                 **/
                SampleType filterSample(SampleType in);

            protected:
                /**
                 * @fn void calcBiquad()
                 * @brief Computes the coefficients of the biquad
                 **/
                void calcBiquad();

                BiQuad_type type;
                double a0, a1, a2, b1, b2;
                double Fc, Q, peakGain;
                double z1, z2;
        };

        inline SampleType BiQuad::filterSample(SampleType in)
        {
            double out = in * a0 + z1;
            z1 = in * a1 + z2 - b1 * out;
            z2 = in * a2 - b2 * out;
            return out;
        }
    }
}

#endif
