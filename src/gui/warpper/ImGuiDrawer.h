#ifndef __SACRED_GUI_DRAWER__
#define __SACRED_GUI_DRAWER__

#include "../imgui/imgui.h"
#include <osg/Drawable>

namespace sacred
{
    namespace gui
    {
        /** This is the main rendering function that you have to implement and provide to ImGui (via setting up 'RenderDrawListsFn' in the ImGuiIO structure) */
        struct ImGuiRenderDrawLists : public ImGuiIO::RenderDrawLists
        {
            public:
                ImGuiRenderDrawLists();
                virtual ~ImGuiRenderDrawLists();

                //! used to save the current ImDrawData wich is used in the next @render call
                virtual void operator() (ImDrawData *draw_data) noexcept;

                //! upload & render the ImDrawData frame
                virtual void render(osg::RenderInfo& /*renderInfo*/) noexcept;

                //! init buffer object in gpu
                virtual bool init(osg::RenderInfo& /*renderInfo*/);

            public:
                GLint  g_ShaderHandle, g_VertHandle, g_FragHandle;
                GLint  g_AttribLocationTex, g_AttribLocationProjMtx;
                GLint  g_AttribLocationPosition, g_AttribLocationUV, g_AttribLocationColor;
                GLuint g_VboHandle, g_VaoHandle, g_ElementsHandle;

            public:
                ImDrawData *draw_data;
        };
    }
}

#endif
