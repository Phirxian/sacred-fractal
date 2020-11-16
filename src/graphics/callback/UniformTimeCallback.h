#pragma once

#include "../Program.h"

namespace sacred
{
    namespace graphics
    {
        struct UniformTimeCallback : public osg::UniformCallback
        {
            UniformTimeCallback(Program *sample)
                : samples(sample)
            {
            }
            
            virtual void operator()(osg::Uniform *uniform, osg::NodeVisitor *nv) noexcept
            {
                float time = nv->getFrameStamp()->getSimulationTime();
                samples->update(time);
                uniform->set(time);
            }
            
            Program *samples;
        };
    }
}
