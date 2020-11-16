#pragma once

#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <vector>

#include "../../sounds/SignalSource.h"

namespace test
{
    #define FRAME_SIZE sacred::sounds::FRAME_SIZE

    using SampleType  = sacred::sounds::SampleType;
    using SampleFrame = sacred::sounds::SampleFrame;

    using Sample_ptr      = sacred::sounds::Sample_ptr;
    using SampleFrame_ptr = sacred::sounds::SampleFrame_ptr;

    // prototype for index
    std::ostream& operator<<(std::ostream& stream, const unsigned int *frame)
    {
        if(!frame)
            return stream;

        stream << frame[0];
        for(int n=1; n<FRAME_SIZE; ++n)
            stream << ", " << frame[n];

        return stream;
    }

    // prototype for data
    std::ostream& operator<<(std::ostream& stream, const SampleType *frame)
    {
        if(!frame)
            return stream;

        stream << frame[0];
        for(int n=1; n<FRAME_SIZE; ++n)
            stream << ", " << frame[n];

        return stream;
    }

    // prototype for the entire frame
    std::ostream& operator<<(std::ostream& stream, const SampleFrame_ptr &frame)
    {
        if(!frame)
            return stream;

        stream << "  {" << std::endl;

        std::vector<unsigned int> l(FRAME_SIZE);
        std::iota(l.begin(), l.end(), (frame->number-1)*FRAME_SIZE);

        stream << "   \"index\" : [" << &l[0] <<  "], " << std::endl;

        if(frame->stereo)
            stream << "   \"right\" : [" << frame->samples[1] <<  "], " << std::endl;

        stream << "   \"left\"  : [" << frame->samples[1]<<  "] " << std::endl;

        stream << "  }";

        return stream;
    }

}
