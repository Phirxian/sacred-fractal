

#ifndef __SACRED_PEAK_DETECTION__
#define __SACRED_PEAK_DETECTION__

#include "../SampleFrame.h"
#include <algorithm>
#include <memory>

/**
 * @file PeakDetection.h
 * @brief Declares the PeakDetection class
 *
 * @author Adrien Grosjean
 * @version 1.1
 * @date April 27 2017
 *
 **/

namespace sacred{
    namespace sounds{

        /**
         * @class PeakDetection
         * @brief The class that allows to detect peaks in data values
         **/
        class PeakDetection
        {

            public:

                PeakDetection(SampleType minPos = 0.0, SampleType maxPos = 1.0, SampleType threshold = -1e6, int maxPeaks = 100, SampleType range = 1.0, bool interpolate = true, std::string orderBy = "position"):
                    m_minPos{std::max(0.0f, minPos)},
                    m_maxPos{std::max(1e-6f, maxPos)},
                    m_threshold{threshold},
                    m_maxPeaks{std::max(1, maxPeaks)},
                    m_range{std::max(1e-6f, range)},
                    m_interpolate{interpolate},
                    m_orderBy{orderBy == "position" ? orderBy : "amplitude"}
                    {
                    }


                /**
                * @fn void compute(const std::vector<SampleType\>& array, std::vector<SampleType\>& values, std::vector<SampleType\>& positions)
                * @brief Computes the peaks postions and values given the array in input
                * @param array : The input array
                * @param values : The amplitudes of the peaks
                * @param positions : The amplitude of the peaks
                **/
                void compute(const std::vector<SampleType>& array, std::vector<SampleType>& values, std::vector<SampleType>& positions);



                void setMinPos(SampleType minPos)
                {
                    m_minPos = minPos;
                }

                void setMaxPos(SampleType maxPos)
                {
                    m_maxPos = maxPos;
                }

                void setThreshold(SampleType threshold)
                {
                    m_threshold = threshold;
                }

                void setMaxPeaks(int maxPeaks)
                {
                    m_maxPeaks = maxPeaks;
                }

                void setRange(SampleType range)
                {
                    m_range = range;
                }

                void setInterpolate(bool interpolate)
                {
                    m_interpolate = interpolate;
                }

                void setOrderBy(std::string orderBy)
                {
                    m_orderBy = (orderBy == "position" ? orderBy : "amplitude");
                }


            protected:
                /** The minimum value of the range to evaluate. [0, inf) **/
                SampleType m_minPos;

                /** The maximum value of the range to evaluate. (0, inf) **/
                SampleType m_maxPos;

                /** Peaks below this given threshold are not output. (-inf, inf) **/
                SampleType m_threshold;

                /** The maximum number of returned peaks. [1, inf) **/
                int m_maxPeaks;

                /** The input range. (0, inf) **/
                SampleType m_range;

                /** Boolean flag to enable interpolation. {true, false} **/
                bool m_interpolate;

                /** The ordering type of the output peaks (ascending by position or descending by value). {position, amplitude} **/
                std::string m_orderBy;

                /**
                * @fn void interpolate(const SampleType leftVal, const SampleType middleVal, const SampleType rightVal, int currentBin, SampleType& resultVal, SampleType& resultBin) const
                * @brief The parabolic interpolation function
                *
                * http://ccrma.stanford.edu/~jos/parshl/Peak_Detection_Steps_3.html
                *
                * Estimating the "true" maximum peak (frequency and magnitude) of the detected local maximum
                * using a parabolic curve-fitting. The idea is that the main-lobe of spectrum of most analysis
                * windows on a dB scale looks like a parabola and therefore the maximum of a parabola fitted
                * through a local maximum bin and it's two neighboring bins will give a good approximation
                * of the actual frequency and magnitude of a sinusoid in the input signal.
                *
                * The parabola f(x) = a(x-n)^2 + b(x-n) + c can be completely described using 3 points;
                * f(n-1) = A1, f(n) = A2 and f(n+1) = A3, where
                * A1 = 20log10(|X(n-1)|), A2 = 20log10(|X(n)|), A3 = 20log10(|X(n+1)|).
                *
                * Solving these equation yields: a = 1/2*A1 - A2 + 1/2*A3, b = 1/2*A3 - 1/2*A1 and
                * c = A2.
                *
                * As the 3 bins are known to be a maxima, solving d/dx f(x) = 0, yields the fractional bin
                * position x of the estimated peak. Substituting delta_x for (x-n) in this equation yields
                * the fractional offset in bins from n where the peak's maximum is.
                *
                * Solving this equation yields: delta_x = 1/2 * (A1 - A3)/(A1 - 2*A2 + A3).
                *
                * Computing f(n+delta_x) will estimate the peak's magnitude (in dB's):
                * f(n+delta_x) = A2 - 1/4*(A1-A3)*delta_x.
                **/
                void interpolate(const SampleType leftVal, const SampleType middleVal, const SampleType rightVal, int currentBin, SampleType& resultVal, SampleType& resultBin) const;
        };
    }
}

#endif
