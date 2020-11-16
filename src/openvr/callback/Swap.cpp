#include "Swap.h"
#include "../device/Device.h"

namespace sacred
{
    namespace openvr
    {
        namespace device { class Device; }
        namespace callback
        {
            Swap::Swap(osg::ref_ptr<device::Device> device)
                : m_device(device), m_frameIndex(0)
            {
            }

            void Swap::swapBuffersImplementation(osg::GraphicsContext *gc)
            {
                // Submit rendered frame to compositor
                m_device->submitFrame();
                // Blit mirror texture to backbuffer
                m_device->blitMirrorTexture(gc);
                // Run the default system swapBufferImplementation
                gc->swapBuffersImplementation();
            }
        }
    }
}
