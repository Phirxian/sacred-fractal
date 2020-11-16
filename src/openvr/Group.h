#ifndef __SACRED_OPENVR_GROUP__
#define __SACRED_OPENVR_GROUP__

#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgViewer/Viewer>

#include "device/Device.h"

namespace osgViewer
{
    class View;
}

namespace sacred
{
	namespace gui { class ImGuiHandler; }
    namespace openvr
    {
        class Group : public osg::Group
        {
            public:
                Group(osgViewer::View *view, osg::ref_ptr<device::Device> dev, osg::ref_ptr<device::RealizeOperation> realizeOperation);
				
				void setImGuiHandler(gui::ImGuiHandler *i){ gui_handler = i; }
				
				osg::observer_ptr<osg::Camera> getLeftCamera() { return m_cameraRTTLeft; }
				osg::observer_ptr<osg::Camera> getRightCamera() { return m_cameraRTTRight; }
				
                virtual void configure();
                virtual void traverse(osg::NodeVisitor &nv);

            protected:
                ~Group() {};

                bool m_configured;
				gui::ImGuiHandler *gui_handler;

                osg::observer_ptr<osgViewer::View> m_view;
                osg::observer_ptr<osg::Camera>     m_cameraRTTRight;
                osg::observer_ptr<osg::Camera>     m_cameraRTTLeft;

                osg::observer_ptr<device::Device>           m_device;
                osg::observer_ptr<device::RealizeOperation> m_realizeOperation;
        };
    }
}

#endif
