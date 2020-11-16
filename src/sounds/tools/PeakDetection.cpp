
#include "./PeakDetection.h"
#include "./Peak.h"
#include <iostream>

namespace sacred{
    namespace sounds{
        
        
        void PeakDetection::compute(const std::vector<SampleType>& array, std::vector<SampleType>& values, std::vector<SampleType>& positions)
        {
            const int size = (int)array.size();

            if (size < 2) {
                std::cerr << "PeakDetection: The size of the array must be at least 2, for the peak detection to work" << std::endl;
                return;
            }

            // dividing by array.size()-1 means the last bin is included in the range
            // dividing by array.size() means it is not (like STL's end interator)
            // which makes more sense in general?
            const SampleType scale = m_range / (SampleType)(size - 1);

            std::vector<Peak> peaks;
            peaks.reserve(size);

            // we want to round up to the next integer instead of simple truncation,
            // otherwise the peak frequency at i can be lower than _minPos
            int i = std::max(0, (int) ceil(m_minPos / scale));

            // first check the boundaries:
            if (i+1 < size && array[i] > array[i+1]) {
                if (array[i] > m_threshold) {
                peaks.push_back(Peak(i*scale, array[i]));
                }
            }

            while(true) {
                // going down
                while (i+1 < size-1 && array[i] >= array[i+1]) {
                i++;
                }

                // now we're climbing
                while (i+1 < size-1 && array[i] < array[i+1]) {
                i++;
                }

                // not anymore, go through the plateau
                int j = i;
                while (j+1 < size-1 && (array[j] == array[j+1])) {
                j++;
                }

                // end of plateau, do we go up or down?
                if (j+1 < size-1 && array[j+1] < array[j] && array[j] > m_threshold) { // going down again
                SampleType resultBin = 0.0;
                SampleType resultVal = 0.0;

                if (j != i) { // plateau peak between i and j
                    if (m_interpolate) {
                    resultBin = (i + j) * 0.5;
                    }
                    else {
                    resultBin = i;
                    }
                    resultVal = array[i];
                }
                else { // interpolate peak at i-1, i and i+1
                    if (m_interpolate) {
                    interpolate(array[j-1], array[j], array[j+1], j, resultVal, resultBin);
                    }
                    else {
                    resultBin = j;
                    resultVal = array[j];
                    }
                }

                SampleType resultPos = resultBin * scale;

                if (resultPos > m_maxPos)
                    break;

                peaks.push_back(Peak(resultPos, resultVal));
                }

                // nothing found, start loop again
                i = j;

                if (i+1 >= size-1) { // check the one just before the last position
                if (i == size-2 && array[i-1] < array[i] &&
                    array[i+1] < array[i] &&
                    array[i] > m_threshold) {
                    SampleType resultBin = 0.0;
                    SampleType resultVal = 0.0;
                    if (m_interpolate) {
                    interpolate(array[i-1], array[i], array[i+1], j, resultVal, resultBin);
                    }
                    else {
                    resultBin = i;
                    resultVal = array[i];
                    }
                    peaks.push_back(Peak(resultBin*scale, resultVal));
                }
                break;
                }
            }

            // check upper boundary here, so peaks are already sorted by position
            SampleType pos = m_maxPos/scale;
            if (size-2 <pos && pos <= size-1 && array[size-1] > array[size-2]) {
                if (array[size-1] > m_threshold) {
                peaks.push_back(Peak((size-1)*scale, array[size-1]));
                }
            }

            // we only want this many peaks
            int nWantedPeaks = std::min((int)m_maxPeaks, (int)peaks.size());

            if (m_orderBy == "amplitude") {
                // sort peaks by magnitude, in case of equality,
                // return the one having smaller position
                std::sort(peaks.begin(), peaks.end(),
                        ComparePeakMagnitude<std::greater<SampleType>, std::less<SampleType> >());
            }
            else if (m_orderBy == "position") {
                // they're already sorted by position
            }
            else {
                std::cerr << "PeakDetection: Unsupported ordering type: '" << m_orderBy << "'" << std::endl;
                return;
            }

            positions.resize(nWantedPeaks);
            values.resize(nWantedPeaks);

            for (int k=0; k<nWantedPeaks; k++) {
                positions[k] = peaks[k].position;
                values[k] = peaks[k].magnitude;
            }
        }
        
        
        
        void PeakDetection::interpolate(const SampleType leftVal, const SampleType middleVal, const SampleType rightVal, int currentBin, SampleType& resultVal, SampleType& resultBin) const
        {
            SampleType delta_x = 0.5 * ((leftVal - rightVal) / (leftVal - 2*middleVal + rightVal));
            resultBin = currentBin + delta_x;
            resultVal = middleVal - 0.25 * (leftVal - rightVal) * delta_x;
        }
        
    }
}
