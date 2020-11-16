#ifndef __SACRED_OPENVR_VIEWER__
#define __SACRED_OPENVR_VIEWER__

#include "Group.h"

namespace sacred
{
    namespace openvr
    {
        class Viewer : public osgViewer::Viewer
        {
            public:
                Viewer(osg::ArgumentParser &arguments, osgViewer::GraphicsWindow *graphicsWindow);

                virtual void eventTraversal();
            private:
                osg::ref_ptr<osgViewer::GraphicsWindow> _graphicsWindow;
        };
    }
}

#endif
