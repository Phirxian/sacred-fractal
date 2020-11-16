#include "PreDraw.h"
#include "../device/Texture.h"

namespace sacred
{
    namespace openvr
    {
        namespace callback
        {
            PreDraw::PreDraw(osg::Camera *camera, device::TextureBuffer *textureBuffer)
                : m_camera(camera)
                , m_textureBuffer(textureBuffer)
            {
            }

            void PreDraw::operator()(osg::RenderInfo &renderInfo) const
            {
                m_textureBuffer->onPreRender(renderInfo);
            }
        }
    }
}
