#include "Group.h"

#include "../gui/warpper/ImGuiHandler.h"
#include "callback/UpdateSlave.h"
#include "callback/PreDraw.h"
#include "callback/PostDraw.h"
#include "callback/Swap.h"

namespace sacred
{
    namespace openvr
    {
        Group::Group(osgViewer::View *view, osg::ref_ptr<device::Device> dev, osg::ref_ptr<device::RealizeOperation> realizeOperation)
            : osg::Group(),
              m_configured(false),
              gui_handler(nullptr),
              m_view(view),
              m_cameraRTTLeft(nullptr),
              m_cameraRTTRight(nullptr),
              m_device(dev),
              m_realizeOperation(realizeOperation)
        {
        }

        void Group::traverse(osg::NodeVisitor &nv)
        {
            // Must be realized before any traversal
            if(!m_configured && m_realizeOperation->realized())
                configure();

            osg::Group::traverse(nv);
        }

        void Group::configure()
        {
            osg::ref_ptr<osg::Camera> camera = m_view->getCamera();
            osg::ref_ptr<osg::GraphicsContext> gc =  camera->getGraphicsContext();
            osg::ref_ptr<callback::Swap> swapCallback = new callback::Swap(m_device);

            gc->setSwapCallback(swapCallback);

            osg::Vec4 clearColor = camera->getClearColor();
            camera->setProjectionMatrix(m_device->projectionMatrixCenter());
            camera->setName("Main");

            m_cameraRTTLeft = m_device->createRTTCamera(device::Device::LEFT, osg::Camera::RELATIVE_RF, clearColor, gc);
            m_cameraRTTRight = m_device->createRTTCamera(device::Device::RIGHT, osg::Camera::RELATIVE_RF, clearColor, gc);
            m_cameraRTTLeft->setName("LeftRTT");
            m_cameraRTTRight->setName("RightRTT");

            m_view->addSlave(m_cameraRTTLeft.get(), m_device->projectionOffsetMatrixLeft(), m_device->viewMatrixLeft(), true);
            m_view->getSlave(0)._updateSlaveCallback = new callback::UpdateSlave(callback::UpdateSlave::LEFT_CAMERA, m_device.get(), swapCallback.get());

            m_view->addSlave(m_cameraRTTRight.get(), m_device->projectionOffsetMatrixRight(), m_device->viewMatrixRight(), true);
            m_view->getSlave(1)._updateSlaveCallback = new callback::UpdateSlave(callback::UpdateSlave::RIGHT_CAMERA, m_device.get(), swapCallback.get());

            m_view->setLightingMode(osg::View::SKY_LIGHT);
            camera->setGraphicsContext(nullptr);
			
            if(gui_handler)
            {				
              gui_handler->setCameraCallbacks(m_cameraRTTLeft.get());
              gui_handler->setCameraCallbacks(m_cameraRTTRight.get());
            }
			
            m_configured = true;
        }
    }
}
