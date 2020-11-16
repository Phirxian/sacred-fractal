#ifndef __SACRED_FRAME_PROCESSING__
#define __SACRED_FRAME_PROCESSING__

#include <vector>
#include <complex>
#include <functional>

#ifndef M_PI
#	define M_PI 3.14159265359
#endif

namespace sacred
{
    namespace sounds
    {
        class SignalSource;

        #define SINGLE_PRECISION

        #ifdef SINGLE_PRECISION
            using ComplexType   = std::complex<float> ;
            using SpectrumType  = std::vector<ComplexType> ;
            using SampleType    = float;
            using FrequencyType = float;
        #else
            using ComplexType   = std::complex<double> ;
            using SpectrumType  = std::vector<ComplexType> ;
            using SampleType    = double;
            using FrequencyType = double;
        #endif

        /**
         * @def static const unsigned int FRAME_SIZE
         * @brief The number of samples per SampleFrame (x2 if stereo)
         **/
        static const std::size_t FRAME_SIZE = 1024;


        /**
         * @def static const unsigned int SAMPLE_RATE
         * @brief Sample rate of the signals
         **/
        static const std::size_t SAMPLE_RATE = 44100;


        /**
         * @def static const unsigned int BIT_DEPTH
         * @brief Bit depth of the signals
         **/
        static const std::size_t BIT_DEPTH = 16;

        class FrameProcessing
        {
            public:
                virtual void process() noexcept = 0;
        };
    }
}

#endif
