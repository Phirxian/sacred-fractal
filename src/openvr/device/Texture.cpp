#include "Texture.h"

#include <osg/GLExtensions>
#include <osg/Multisample>
#include <osg/State>
#include <osg/Notify>
#include <osg/buffered_value>
#include <osg/RenderInfo>

#ifndef GL_TEXTURE_MAX_LEVEL
    #define GL_TEXTURE_MAX_LEVEL 0x813D
#endif

namespace sacred
{
    namespace openvr
    {
        namespace device
        {
            TextureBuffer::TextureBuffer(osg::ref_ptr<osg::State> state, int width, int height, int samples)
                : m_Resolve_FBO(0), m_Resolve_ColorTex(0),
                  m_MSAA_FBO(0),    m_MSAA_ColorTex(0), m_MSAA_DepthTex(0),
                  m_width(width),   m_height(height),   m_samples(samples)
            {
                const auto *fbo_ext = getGLExtensions(*state);
                const int maxTextureLevel = 0;

                fbo_ext->glGenFramebuffers(1, &m_Resolve_FBO);

                glGenTextures(1, &m_Resolve_ColorTex);
                glBindTexture(GL_TEXTURE_2D, m_Resolve_ColorTex);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, maxTextureLevel);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

                // Create an FBO for primary render target.
                fbo_ext->glGenFramebuffers(1, &m_MSAA_FBO);
                const auto *extensions = getTextureExtensions(*state);

                // Create MSAA colour buffer
                glGenTextures(1, &m_MSAA_ColorTex);
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA_ColorTex);
                extensions->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_RGBA, m_width, m_height, false);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER_ARB);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER_ARB);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_LEVEL, maxTextureLevel);

                // Create MSAA depth buffer
                glGenTextures(1, &m_MSAA_DepthTex);
                glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_MSAA_DepthTex);
                extensions->glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_samples, GL_DEPTH_COMPONENT, m_width, m_height, false);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAX_LEVEL, maxTextureLevel);

                // check FBO status
                GLenum status = fbo_ext->glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);

                if(status != GL_FRAMEBUFFER_COMPLETE_EXT)
                    osg::notify(osg::WARN) << "Error setting up frame buffer object." << std::endl;
            }

            void TextureBuffer::onPreRender(osg::RenderInfo &renderInfo)
            {
                osg::State &state = *renderInfo.getState();
                const auto *fbo_ext = getGLExtensions(state);
                
                fbo_ext->glBindFramebuffer(GL_FRAMEBUFFER_EXT, m_MSAA_FBO);
                fbo_ext->glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D_MULTISAMPLE, m_MSAA_ColorTex, 0);
                fbo_ext->glFramebufferTexture2D(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D_MULTISAMPLE, m_MSAA_DepthTex, 0);
            }

            void TextureBuffer::onPostRender(osg::RenderInfo &renderInfo)
            {
                osg::State &state = *renderInfo.getState();
                const auto *fbo_ext = getGLExtensions(state);

                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, m_MSAA_FBO);
                fbo_ext->glFramebufferTexture2D(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D_MULTISAMPLE, m_MSAA_ColorTex, 0);
                fbo_ext->glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);

                fbo_ext->glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, m_Resolve_FBO);
                fbo_ext->glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_Resolve_ColorTex, 0);
                fbo_ext->glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);

                // Copy MSAA_FBO texture tŜo Resolve_FBO
                fbo_ext->glBlitFramebuffer(0, 0, m_width, m_height, 0, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
                fbo_ext->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
            }

            void TextureBuffer::destroy(osg::GraphicsContext *gc)
            {
                const auto *fbo_ext = getGLExtensions(*gc->getState());

                if(fbo_ext)
                {
                    fbo_ext->glDeleteFramebuffers(1, &m_MSAA_FBO);
                    fbo_ext->glDeleteFramebuffers(1, &m_Resolve_FBO);
                }
            }

            MirrorTexture::MirrorTexture(osg::ref_ptr<osg::State> state, GLint width, GLint height)
                : m_width(width), m_height(height)
            {
                const auto *fbo_ext = getGLExtensions(*state);

                fbo_ext->glGenFramebuffers(1, &m_mirrorFBO);
                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, m_mirrorFBO);

                glGenTextures(1, &m_mirrorTex);
                glBindTexture(GL_TEXTURE_2D, m_mirrorTex);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

                fbo_ext->glFramebufferTexture2D(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_mirrorTex, 0);
                fbo_ext->glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, 0);
            }

            void MirrorTexture::blitTexture(osg::GraphicsContext *gc, TextureBuffer *leftEye, TextureBuffer *rightEye)
            {
                const auto *fbo_ext = getGLExtensions(*(gc->getState()));

                fbo_ext->glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, m_mirrorFBO);
                fbo_ext->glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_mirrorTex, 0);
                fbo_ext->glFramebufferRenderbuffer(GL_DRAW_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);

                glClearColor(1, 0, 0, 1);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Copy left eye image to mirror
                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, leftEye->m_Resolve_FBO);
                fbo_ext->glFramebufferTexture2D(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,leftEye->m_Resolve_ColorTex, 0);
                fbo_ext->glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
                fbo_ext->glBlitFramebuffer(0, 0, leftEye->m_width, leftEye->m_height, 0, 0, m_width / 2, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

                // Copy right eye image to mirror
                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, rightEye->m_Resolve_FBO);
                fbo_ext->glFramebufferTexture2D(GL_READ_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, rightEye->m_Resolve_ColorTex, 0);
                fbo_ext->glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, 0);
                fbo_ext->glBlitFramebuffer(0, 0, rightEye->m_width, rightEye->m_height, m_width / 2, 0, m_width, m_height, GL_COLOR_BUFFER_BIT, GL_NEAREST);

                fbo_ext->glBindFramebuffer(GL_FRAMEBUFFER_EXT, 0);
                // Blit mirror texture to back buffer
                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, m_mirrorFBO);
                fbo_ext->glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, 0);

                GLint w = m_width;
                GLint h = m_height;

                fbo_ext->glBlitFramebuffer(0, 0, w, h, 0, 0, w, h, GL_COLOR_BUFFER_BIT, GL_NEAREST);
                fbo_ext->glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, 0);
            }

            void MirrorTexture::destroy(osg::GraphicsContext *gc)
            {
                const auto *fbo_ext = getGLExtensions(*gc->getState());

                if(fbo_ext)
                    fbo_ext->glDeleteFramebuffers(1, &m_mirrorFBO);
            }
        }
    }
}
