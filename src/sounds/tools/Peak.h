#ifndef __SACRED_PEAK__
#define __SACRED_PEAK__

/**
 * @file Peak.h
 * @brief Defines the Peak class and other class & methods closely related
 * 
 * @author Adrien Grosjean
 * @version 1.1
 * @date May 2 2017
 * 
 **/

#include <utility> // std::pair
#include <iostream>
#include "../SampleFrame.h"

namespace sacred {
    namespace sounds {

        /**
         * @class Peak
         * @brief A Peak rassembles a position and a magnitude
         **/
        class Peak
        {
            public:
                SampleType position;
                SampleType magnitude;

            Peak() : position(), magnitude()
            {
            }
            
            Peak(const Peak& p) : position(p.position), magnitude(p.magnitude)
            {
            }

            template<typename T, typename U>
            Peak(const T& pos, const U& mag) : position(pos), magnitude(mag)
            {
            }

            template<typename T, typename U>
            Peak(const std::pair<T,U>& p) : position(p.first), magnitude(p.second)
            {
            }

            bool operator ==(const Peak& p) const
            {
                return (position == p.position) && (magnitude == p.magnitude);
            }

            bool operator !=(const Peak& p) const
            {
                return (position != p.position) || (magnitude != p.magnitude);
            }

            bool operator< (const Peak& p) const
            {
                return magnitude <  p.magnitude;
            }
            
            bool operator> (const Peak& p) const
            {
                return magnitude >  p.magnitude;
            }
            
            bool operator<=(const Peak& p) const
            {
                return magnitude <= p.magnitude;            
            }
            
            bool operator>=(const Peak& p) const
            {
                return magnitude >= p.magnitude;
            }

            Peak& operator=(const Peak& p)
            {
                position = p.position; magnitude = p.magnitude;
                return *this;
            }

            template<typename T, typename U>
            Peak& operator=(const std::pair<T, U>& p)
            {
                position = p.first;magnitude = p.second;
                return *this;
            }
        };

        
        // peak comparison:

        /**
         * @class ComparePeakPosition
         * @brief Comparing by position, by default sorts by ascending position and in case the positions are equal it sorts by descending magnitude
         **/
        template<typename Comp1=std::less<SampleType>,
                typename Comp2=std::greater_equal<SampleType> >
        class ComparePeakPosition : public std::binary_function<SampleType, SampleType, bool>
        {
            Comp1 _cmp1;
            Comp2 _cmp2;
            public:
                bool operator () (const Peak& p1, const Peak& p2) const
                {
                    if (_cmp1(p1.position, p2.position)) return true;
                    if (_cmp1(p2.position, p1.position)) return false;
                    return _cmp2(p1.magnitude, p2.magnitude);
                }
        };

        
        /**
         * @class ComparePeakMagnitude
         * @brief Comparing by magnitude, by default sorts by descending magnitude and in case the magnitudes are equal it sorts by ascending position
         **/
        template<typename Comp1=std::greater<SampleType>,
                typename Comp2=std::less_equal<SampleType> >
        class ComparePeakMagnitude : public std::binary_function<SampleType, SampleType, bool>
        {
            Comp1 _cmp1;
            Comp2 _cmp2;
            public:
                bool operator () (const Peak& p1, const Peak& p2) const
                {
                    if (_cmp1(p1.magnitude, p2.magnitude)) return true;
                    if (_cmp1(p2.magnitude, p1.magnitude)) return false;
                    return _cmp2(p1.position, p2.position);
                }
        };
        

        /**
         * @fn inline std::vector<Peak\> realsToPeaks(const std::vector<SampleType\>& pos, const std::vector<SampleType\>& mag)
         * @brief From 2 vector<SampleType\> to vector<Peak\>
         **/
        inline std::vector<Peak> realsToPeaks(const std::vector<SampleType>& pos,
                                            const std::vector<SampleType>& mag)
        {
            int size = pos.size();
            if (size != int(mag.size()))
            {
                std::cerr << "realsToPeaks: position vector size != magnitude vector size" << std::endl;
            }
            
            std::vector<Peak> peaks(size);
            
            for (int i=0; i<size; i++)
            {
                peaks[i] = Peak(pos[i], mag[i]);
            }
            
            return peaks;
        }
        

        /**
         * @fn inline void peaksToReals(const std::vector<Peak\>& peaks, std::vector<SampleType\>& pos, std::vector<SampleType\>& mag)
         * @brief From vector<Peak\> to 2 vector<Real\>
         **/
        inline void peaksToReals(const std::vector<Peak>& peaks,
                                std::vector<SampleType>& pos, std::vector<SampleType>& mag)
        {
            int size = peaks.size();
            if (size != int(pos.size())) pos.resize(size);
            if (size != int(mag.size())) mag.resize(size);

            for (int i=0; i<size; i++) {
                pos[i] = peaks[i].position;
                mag[i] = peaks[i].magnitude;
            }
        }

        
        inline std::ostream& operator<<(std::ostream& out, const Peak& p)
        {
            return out << "position: " << p.position << ", magnitude: " << p.magnitude;
        }

    }
}

#endif

