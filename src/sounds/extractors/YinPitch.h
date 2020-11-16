#ifndef __SACRED_YIN_PITCH__
#define __SACRED_YIN_PITCH__

/**
 * @file YinPitch.h
 * @brief Declares the YinPitch class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 **/

#include "../SignalSource.h"
#include "../window/Window.h"
#include "../window/RectangularWindow.h"
#include "../tools/PeakDetection.h"

#include <algorithm>

namespace sacred
{
    namespace sounds
    {
        /**
         * @class YinPitch
         * @brief The SignalSource class that computes the pitch of its input using the Yin algorithm
         **/
        class YinPitch : public SignalSource
        {
            public:
                constexpr static const double MIN_TOLERANCE{0.001};

                YinPitch(bool stereo=true, double minFreq=20.0, double maxFreq=22050.0, bool interpolate=true, double tolerance=0.15);

                virtual ~YinPitch();

                virtual const char* getTypeName() const { return "YinPitch"; }

                void process() noexcept override;

                void setMinFreq(double minFreq);

                void setMaxFreq(double maxFreq);

                void setInterpolate(bool interpolate);

                void setTolerance(double tolerance);

            protected:

                SampleType m_pitch[2];
                SampleType m_pitchConfidence[2];

                PeakDetection m_peakDetectLocal[2];
                PeakDetection m_peakDetectGlobal[2];

                /** Yin function (cumulative mean normalized difference) **/
                std::vector<SampleType> m_yin[2];
                /** Yin function peak positions **/
                std::vector<SampleType> m_positions[2];
                /** Yin function peak amplitudes **/
                std::vector<SampleType> m_amplitudes[2];

                double m_minFreq;
                double m_maxFreq;
                bool m_interpolate;
                double m_tolerance;
                int m_tauMin;
                int m_tauMax;

                std::shared_ptr<Window> m_window;


                void computePitch(
                    const std::vector<SampleType> &signal, SampleType &pitch,
                    SampleType &pitchConfidence,
                    std::vector<SampleType> &yin,
                    std::vector<SampleType> &positions,
                    std::vector<SampleType> &amplitudes,
                    PeakDetection& peakDetectLocal,
                    PeakDetection& peakDetectGlobal
                );

                /**
                 * @fn void updateParams()
                 * @brief Updates the parameters from the user parameters
                 **/
                void updateParams();
        };
    }
}

#endif
