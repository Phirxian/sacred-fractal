#include "ImGuiHandler.h"
#include "ImGuiCallback.h"
#include "ImGuiDrawer.h"

#include <iostream>

namespace sacred
{
    namespace gui
    {
        ImGuiHandler::ImGuiHandler(GuiCallback *theGuicallback) noexcept
            : m_callback(theGuicallback), g_Time(0.f),
              g_MouseWheel(0.f), g_FontTexture(0),
              draw(new ImGuiRenderDrawLists())
        {
            for(int i = 0; i < 3; ++i)
                g_MousePressed[i] = false;
        }

        void ImGuiHandler::init(osg::RenderInfo &theRenderInfo) noexcept
        {
            ImGuiIO& io = ImGui::GetIO();
            unsigned char* pixels;
            int width, height;

            // Load as RGBA 32-bits for OpenGL3 demo because it is more likely to be compatible with user's existing shader.
            io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

            // Upload texture to graphics system
            GLint last_texture;
            glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
            glGenTextures(1, &g_FontTexture);
            glBindTexture(GL_TEXTURE_2D, g_FontTexture);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

            // Store our identifier
            io.Fonts->TexID = (void *)(intptr_t)g_FontTexture;

            // Restore state
            glBindTexture(GL_TEXTURE_2D, last_texture);

            draw->init(theRenderInfo);
            io.RenderDrawListsFn = draw;
        }

        void ImGuiHandler::setCameraCallbacks(osg::Camera *theCamera) noexcept
        {
            ImGuiNewFrameCallback *aPreDrawCallback = new ImGuiNewFrameCallback(*this);
            theCamera->setPreDrawCallback(aPreDrawCallback);
            ImGuiDrawCallback *aPostDrawCallback = new ImGuiDrawCallback(*this);
            theCamera->setPostDrawCallback(aPostDrawCallback);
        }

        void ImGuiHandler::newFrame(osg::RenderInfo &theRenderInfo) noexcept
        {
            if(!g_FontTexture)
                init(theRenderInfo);

            ImGuiIO &io = ImGui::GetIO();
            osg::Viewport *aViewport = theRenderInfo.getCurrentCamera()->getViewport();
            io.DisplaySize = ImVec2(aViewport->width(), aViewport->height());

            double aCurrentTime = theRenderInfo.getView()->getFrameStamp()->getSimulationTime();
            io.DeltaTime = g_Time > 0.0 ? (float)(aCurrentTime - g_Time) : 1.0f/60.0f;
            g_Time = aCurrentTime;

            for(int i = 0; i < 3; i++)
                io.MouseDown[i] = g_MousePressed[i];

            io.MouseWheel = g_MouseWheel;
            g_MouseWheel = 0.0f;
			
            ImGui::NewFrame();
        }

        bool ImGuiHandler::handle(
            const osgGA::GUIEventAdapter &theEventAdapter,
            osgGA::GUIActionAdapter &theActionAdapter) noexcept
        {
            bool wantCapureMouse = ImGui::GetIO().WantCaptureMouse;
            bool wantCapureKeyboard = ImGui::GetIO().WantCaptureKeyboard;

            switch(theEventAdapter.getEventType())
            {
                case osgGA::GUIEventAdapter::KEYDOWN:
                    {
                        ImGuiIO &io = ImGui::GetIO();
                        int c = theEventAdapter.getKey();

                        if(c > 0 && c < 0x10000)
                            io.AddInputCharacter((unsigned short)c);

                        return wantCapureKeyboard;
                    }

                case(osgGA::GUIEventAdapter::PUSH):
                    {
                        ImGuiIO &io = ImGui::GetIO();
                        io.MousePos = ImVec2(theEventAdapter.getX(), io.DisplaySize.y - theEventAdapter.getY());
                        g_MousePressed[0] = true;
                        return wantCapureMouse;
                    }

                case(osgGA::GUIEventAdapter::DRAG):
                case(osgGA::GUIEventAdapter::MOVE):
                    {
                        ImGuiIO &io = ImGui::GetIO();
                        io.MousePos = ImVec2(theEventAdapter.getX(), io.DisplaySize.y - theEventAdapter.getY());
                        return wantCapureMouse;
                    }

                case(osgGA::GUIEventAdapter::RELEASE):
                    {
                        g_MousePressed[0] = false;
                        return wantCapureMouse;
                    }

                case(osgGA::GUIEventAdapter::SCROLL):
                    {
                        g_MouseWheel = theEventAdapter.getScrollingDeltaY();
                        return wantCapureMouse;
                    }

                default:
                    return false;
            }

            return false;
        }

        void ImGuiHandler::render(osg::RenderInfo &render_info) noexcept
        {
            if(m_callback)
                (*m_callback)();

			osg::notify(osg::FATAL) << "imgui render" << std::endl;
            ImGui::Render();

            draw->render(render_info);
        }
    }
}
