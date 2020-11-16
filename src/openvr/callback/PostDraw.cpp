#include "PostDraw.h"
#include "../device/Texture.h"

namespace sacred
{
    namespace openvr
    {
        namespace callback
        {
            PostDraw::PostDraw(osg::Camera *camera, device::TextureBuffer *textureBuffer)
                : m_camera(camera)
                , m_textureBuffer(textureBuffer)
            {
            }

            void PostDraw::operator()(osg::RenderInfo &renderInfo) const
            {
                m_textureBuffer->onPostRender(renderInfo);
            }
        }
    }
}
