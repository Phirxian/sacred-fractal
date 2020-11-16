
/**
 * @file BiQuad.cpp
 * @brief Defines the BiQuad class methods
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 28 2017
 **/

#include <math.h>
#include "BiQuad.h"

namespace sacred
{
    namespace sounds
    {
        BiQuad::BiQuad()
        {
            type = bq_type_lowpass;
            a0 = 1.0;
            a1 = a2 = b1 = b2 = 0.0;
            Fc = 0.50;
            Q = 0.707;
            peakGain = 0.0;
            z1 = z2 = 0.0;
        }

        BiQuad::BiQuad(bool stereo, BiQuad_type type, double Fc, double Q, double peakGainDB)
            : SignalSource(stereo,1)
        {
            setBiQuad(type, Fc, Q, peakGainDB);
            z1 = z2 = 0.0;
        }

        BiQuad::~BiQuad()
        {
        }

        void BiQuad::process() noexcept
        {
            if(m_inputs[0] == nullptr)
                return;

            auto frame = m_inputs[0]->getFrame();

            #pragma omp parallel for
            for(int i = 0; i<=m_stereo; ++i)
            {
                for(unsigned int n=0; n<FRAME_SIZE; ++n)
                    m_frame->samples[i][n] = filterSample(frame->samples[i][n]);
            }
        }

        void BiQuad::setType(BiQuad_type type)
        {
            this->type = type;
            calcBiquad();
        }

        void BiQuad::setQ(double Q)
        {
            this->Q = Q;
            calcBiquad();
        }

        void BiQuad::setFc(double Fc)
        {
            this->Fc = Fc;
            calcBiquad();
        }

        void BiQuad::setPeakGain(double peakGainDB)
        {
            this->peakGain = peakGainDB;
            calcBiquad();
        }

        void BiQuad::setBiQuad(BiQuad_type type, double Fc, double Q, double peakGainDB)
        {
            this->type = type;
            this->Q = Q;
            this->Fc = Fc;
            setPeakGain(peakGainDB);
        }

        void BiQuad::calcBiquad(void)
        {
            double norm;
            double V = pow(10, fabs(peakGain) / 20.0);
            double K = tan(M_PI * Fc);
            switch (this->type)
            {
                case bq_type_lowpass:
                    norm = 1 / (1 + K / Q + K * K);
                    a0 = K * K * norm;
                    a1 = 2 * a0;
                    a2 = a0;
                    b1 = 2 * (K * K - 1) * norm;
                    b2 = (1 - K / Q + K * K) * norm;
                    break;

                case bq_type_highpass:
                    norm = 1 / (1 + K / Q + K * K);
                    a0 = 1 * norm;
                    a1 = -2 * a0;
                    a2 = a0;
                    b1 = 2 * (K * K - 1) * norm;
                    b2 = (1 - K / Q + K * K) * norm;
                    break;

                case bq_type_bandpass:
                    norm = 1 / (1 + K / Q + K * K);
                    a0 = K / Q * norm;
                    a1 = 0;
                    a2 = -a0;
                    b1 = 2 * (K * K - 1) * norm;
                    b2 = (1 - K / Q + K * K) * norm;
                    break;

                case bq_type_notch:
                    norm = 1 / (1 + K / Q + K * K);
                    a0 = (1 + K * K) * norm;
                    a1 = 2 * (K * K - 1) * norm;
                    a2 = a0;
                    b1 = a1;
                    b2 = (1 - K / Q + K * K) * norm;
                    break;

                case bq_type_peak:
                    if (peakGain >= 0) {    // boost
                        norm = 1 / (1 + 1/Q * K + K * K);
                        a0 = (1 + V/Q * K + K * K) * norm;
                        a1 = 2 * (K * K - 1) * norm;
                        a2 = (1 - V/Q * K + K * K) * norm;
                        b1 = a1;
                        b2 = (1 - 1/Q * K + K * K) * norm;
                    }
                    else {    // cut
                        norm = 1 / (1 + V/Q * K + K * K);
                        a0 = (1 + 1/Q * K + K * K) * norm;
                        a1 = 2 * (K * K - 1) * norm;
                        a2 = (1 - 1/Q * K + K * K) * norm;
                        b1 = a1;
                        b2 = (1 - V/Q * K + K * K) * norm;
                    }
                    break;

                case bq_type_lowshelf:
                    if (peakGain >= 0) {    // boost
                        norm = 1 / (1 + sqrt(2) * K + K * K);
                        a0 = (1 + sqrt(2*V) * K + V * K * K) * norm;
                        a1 = 2 * (V * K * K - 1) * norm;
                        a2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                        b1 = 2 * (K * K - 1) * norm;
                        b2 = (1 - sqrt(2) * K + K * K) * norm;
                    }
                    else {    // cut
                        norm = 1 / (1 + sqrt(2*V) * K + V * K * K);
                        a0 = (1 + sqrt(2) * K + K * K) * norm;
                        a1 = 2 * (K * K - 1) * norm;
                        a2 = (1 - sqrt(2) * K + K * K) * norm;
                        b1 = 2 * (V * K * K - 1) * norm;
                        b2 = (1 - sqrt(2*V) * K + V * K * K) * norm;
                    }
                    break;

                case bq_type_highshelf:
                    if (peakGain >= 0) {    // boost
                        norm = 1 / (1 + sqrt(2) * K + K * K);
                        a0 = (V + sqrt(2*V) * K + K * K) * norm;
                        a1 = 2 * (K * K - V) * norm;
                        a2 = (V - sqrt(2*V) * K + K * K) * norm;
                        b1 = 2 * (K * K - 1) * norm;
                        b2 = (1 - sqrt(2) * K + K * K) * norm;
                    }
                    else {    // cut
                        norm = 1 / (V + sqrt(2*V) * K + K * K);
                        a0 = (1 + sqrt(2) * K + K * K) * norm;
                        a1 = 2 * (K * K - 1) * norm;
                        a2 = (1 - sqrt(2) * K + K * K) * norm;
                        b1 = 2 * (K * K - V) * norm;
                        b2 = (V - sqrt(2*V) * K + K * K) * norm;
                    }
                    break;
            }

            return;
        }
    }
}
