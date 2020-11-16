#pragma once

namespace sacred
{
    namespace graphics
    {
        struct UniformResolutionCallback : public osg::UniformCallback
        {
            UniformResolutionCallback(osg::Camera *cam)
                : camera(cam)
            {
            }

            virtual void operator()(osg::Uniform *uniform, osg::NodeVisitor *nv) noexcept
            {
                osg::Viewport *aViewport = camera->getViewport();
                auto size = osg::Vec2f(aViewport->width(), aViewport->height());
                uniform->set(size);
            }

            osg::Camera *camera;
        };
    }
}
