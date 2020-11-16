#ifndef __OPENVR_EVENT_HANDLER__
#define __OPENVR_EVENT_HANDLER__

#include <osgViewer/ViewerEventHandlers>

namespace sacred
{
    namespace openvr
    {
        namespace device
        {
            class Device;

            class EventHandler : public osgGA::GUIEventHandler
            {
                public:
                    explicit EventHandler(osg::ref_ptr<Device> device) : m_openvrDevice(device), m_usePositionalTracking(true) {}
                    virtual bool handle(const osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter &);
                protected:
                    osg::ref_ptr<Device> m_openvrDevice;
                    bool m_usePositionalTracking;

            };
        }
    }
}

#endif
