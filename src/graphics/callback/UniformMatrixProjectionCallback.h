#pragma once

namespace sacred
{
    namespace graphics
    {
        struct UniformMatrixProjectionCallback : public osg::UniformCallback
        {
            UniformMatrixProjectionCallback(osg::Camera *cam)
                : camera(cam)
            {
            }

            virtual void operator()(osg::Uniform *uniform, osg::NodeVisitor *nv) noexcept
            {
                auto projection = camera->getProjectionMatrix();
                uniform->set(projection);
            }

            osg::Camera *camera;
        };
    }
}
