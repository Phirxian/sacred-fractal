#ifndef __OPENVR_PRE_DRAW_CALLBACK__
#define __OPENVR_PRE_DRAW_CALLBACK__

#include <osg/Camera>

namespace sacred
{
    namespace openvr
    {
        namespace device { class TextureBuffer; }
        namespace callback
        {
            class PreDraw : public osg::Camera::DrawCallback
            {
                public:
                    PreDraw(osg::Camera *camera, device::TextureBuffer *textureBuffer);

                    virtual void operator()(osg::RenderInfo &renderInfo) const;
                protected:
                    osg::Camera *m_camera;
                    device::TextureBuffer *m_textureBuffer;

            };
        }
    }
}

#endif
