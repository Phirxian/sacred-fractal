#ifndef __SACRED_GUI_IM_CALLBACK__
#define __SACRED_GUI_IM_CALLBACK__

namespace sacred
{
    namespace gui
    {
        struct ImGuiNewFrameCallback : public osg::Camera::DrawCallback
        {
            public:
                ImGuiNewFrameCallback(ImGuiHandler &theHandler) noexcept
                    : m_handler(theHandler)
                {
                }

                virtual void operator()(osg::RenderInfo &theRenderInfo) const noexcept
                {
                    m_handler.newFrame(theRenderInfo);
                }
            private:
                ImGuiHandler &m_handler;
        };

        struct ImGuiDrawCallback : public osg::Camera::DrawCallback
        {
            public:
                ImGuiDrawCallback(ImGuiHandler &theHandler) noexcept
                    : m_handler(theHandler)
                {
                }

                virtual void operator()(osg::RenderInfo &theRenderInfo) const noexcept
                {
                    m_handler.render(theRenderInfo);
                }
            private:
                ImGuiHandler &m_handler;
        };
    }
}

#endif
