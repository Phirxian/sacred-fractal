#include "UpdateSlave.h"
#include "Swap.h"

namespace sacred
{
    namespace openvr
    {
        namespace callback
        {
            UpdateSlave::UpdateSlave(CameraType cameraType, device::Device *device, Swap *swapCallback)
                : m_cameraType(cameraType),
                  m_device(device),
                  m_swapCallback(swapCallback)
            {
            }

            void UpdateSlave::updateSlave(osg::View &view, osg::View::Slave &slave)
            {
                if(m_cameraType == LEFT_CAMERA)
                    m_device->updatePose();

                osg::Vec3 position = m_device->position();
                osg::Quat orientation = m_device->orientation();
                osg::Matrix viewOffset = (m_cameraType == LEFT_CAMERA) ? m_device->viewMatrixLeft() : m_device->viewMatrixRight();

                viewOffset.preMultRotate(orientation);
                viewOffset.setTrans(viewOffset.getTrans() + position);

                slave._viewOffset = viewOffset;
                slave.updateSlaveImplementation(view);
            }
        }
    }
}
