#ifndef __OPENVR_POST_DRAW_CALLBACK__
#define __OPENVR_POST_DRAW_CALLBACK__

#include <osg/Camera>

namespace sacred
{
    namespace openvr
    {
        namespace device { class TextureBuffer; }
        namespace callback
        {
            class PostDraw : public osg::Camera::DrawCallback
            {
                public:
                    PostDraw(osg::Camera *camera, device::TextureBuffer *textureBuffer);

                    virtual void operator()(osg::RenderInfo &renderInfo) const;
                protected:
                    osg::Camera *m_camera;
                    device::TextureBuffer *m_textureBuffer;

            };
        }
    }
}

#endif
