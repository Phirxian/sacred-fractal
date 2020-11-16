#ifndef __SACRED_GUI_IM_HANDLER__
#define __SACRED_GUI_IM_HANDLER__

#include "../imgui/imgui.h"
#include <osgViewer/ViewerEventHandlers>
#include <osg/Camera>

namespace sacred
{
    namespace gui
    {
        struct ImGuiRenderDrawLists;

        struct GuiCallback
        {
            virtual void operator()() noexcept {}
        };

        class ImGuiHandler : public osgGA::GUIEventHandler
        {
            public:
                ImGuiHandler(GuiCallback *theGuicallback) noexcept;

                void init(osg::RenderInfo &theRenderInfo) noexcept;

                void newFrame(osg::RenderInfo &theRenderInfo) noexcept;
                void setCameraCallbacks(osg::Camera *theCamera) noexcept;

                virtual bool handle(
                    const osgGA::GUIEventAdapter &theEventAdapter,
                    osgGA::GUIActionAdapter &theActionAdapter
                ) noexcept;

                void render(osg::RenderInfo &theRenderInfo) noexcept;
            private:
                bool g_MousePressed[3];

                GLuint g_FontTexture;
                float g_MouseWheel;
                double g_Time;

                GuiCallback *m_callback;
                ImGuiRenderDrawLists *draw;
        };
    }
}

#endif // UTILS_IMGUI_HANDLER_HEADER
