#pragma once

namespace sacred
{
    namespace graphics
    {
        struct UniformMatrixViewCallback : public osg::UniformCallback
        {
            UniformMatrixViewCallback(osg::Camera *cam)
                : camera(cam)
            {
            }

            virtual void operator()(osg::Uniform *uniform, osg::NodeVisitor *nv) noexcept
            {
                auto view = camera->getViewMatrix();
                uniform->set(view);
            }

            osg::Camera *camera;
        };
    }
}
