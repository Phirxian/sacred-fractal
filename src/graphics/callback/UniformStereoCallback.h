#pragma once

#include <osg/Texture2D>

namespace sacred
{
    namespace graphics
    {
        struct UniformStereoCallback : public osg::UniformCallback
        {
            UniformStereoCallback(Program *sample)
                : samples(sample)
            {
            }

            virtual void operator()(osg::Uniform *uniform, osg::NodeVisitor *nv) noexcept
            {
                float stereo = samples->isStereo();
                uniform->set(stereo);
            }

            Program *samples;
        };
    }
}
