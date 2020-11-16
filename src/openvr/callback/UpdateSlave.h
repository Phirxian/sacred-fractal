#ifndef __OPENVR_UPDATE_SLAVE_CALLBACK__
#define __OPENVR_UPDATE_SLAVE_CALLBACK__

#include <osgViewer/View>
#include "../device/Device.h"

namespace sacred
{
    namespace openvr
    {
        namespace device { class Device; }
        namespace callback
        {
            class Swap;
            struct UpdateSlave: public osg::View::Slave::UpdateSlaveCallback
            {
                enum CameraType
                {
                    LEFT_CAMERA,
                    RIGHT_CAMERA
                };

                UpdateSlave(CameraType cameraType, device::Device *device, Swap *swapCallback);

                virtual void updateSlave(osg::View &view, osg::View::Slave &slave);

                CameraType m_cameraType;
                osg::ref_ptr<device::Device> m_device;
                osg::ref_ptr<Swap> m_swapCallback;
            };
        }
    }
}

#endif
