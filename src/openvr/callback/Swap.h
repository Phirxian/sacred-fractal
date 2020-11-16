#ifndef __OPENVR_SWAP_CALLBACK__
#define __OPENVR_SWAP_CALLBACK__

#include <osg/GraphicsContext>

namespace sacred
{
    namespace openvr
    {
        namespace device { class Device; }
        namespace callback
        {
            class Swap : public osg::GraphicsContext::SwapCallback
            {
                public:
                    explicit Swap(osg::ref_ptr<device::Device> device);

                    void swapBuffersImplementation(osg::GraphicsContext *gc);

                    int frameIndex() const
                    {
                        return m_frameIndex;
                    }
                private:
                    osg::observer_ptr<device::Device> m_device;
                    int m_frameIndex;
            };
        }
    }
}

#endif // __OPENVR_SWAP_CALLBACK__
