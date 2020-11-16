#include "EventHandler.h"
#include "Device.h"

namespace sacred
{
    namespace openvr
    {
        namespace device
        {
            bool EventHandler::handle(const osgGA::GUIEventAdapter &ea,osgGA::GUIActionAdapter &ad)
            {
                switch(ea.getEventType())
                {
                    case osgGA::GUIEventAdapter::KEYUP:
                    {
                        switch(ea.getKey())
                        {
                            case osgGA::GUIEventAdapter::KEY_R:
                                m_openvrDevice->resetSensorOrientation();
                                break;
                        }
                    }
                }

                return osgGA::GUIEventHandler::handle(ea, ad);
            }
        }
    }
}
