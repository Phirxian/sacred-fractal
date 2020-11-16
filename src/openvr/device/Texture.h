#ifndef __SACRED_OPENVR_GRAPHICS__
#define __SACRED_OPENVR_GRAPHICS__

#include <osg/Texture2D>
#include <osg/Version>
#include <osg/FrameBufferObject>

#if(OSG_VERSION_GREATER_OR_EQUAL(3, 4, 0))
    typedef osg::GLExtensions OSG_GLExtensions;
    typedef osg::GLExtensions OSG_Texture_Extensions;
#else
    typedef osg::FBOExtensions OSG_GLExtensions;
    typedef osg::Texture::Extensions OSG_Texture_Extensions;
#endif

inline const OSG_GLExtensions *getGLExtensions(const osg::State &state)
{
    #if(OSG_VERSION_GREATER_OR_EQUAL(3, 4, 0))
        return state.get<osg::GLExtensions>();
    #else
        return osg::FBOExtensions::instance(state.getContextID(), true);
    #endif
}

inline const OSG_Texture_Extensions *getTextureExtensions(const osg::State &state)
{
    #if(OSG_VERSION_GREATER_OR_EQUAL(3, 4, 0))
        return state.get<osg::GLExtensions>();
    #else
        return osg::Texture::getExtensions(state.getContextID(), true);
    #endif
}

namespace sacred
{
    namespace openvr
    {
        namespace device
        {
            class TextureBuffer : public osg::Referenced
            {
                public:
                    TextureBuffer(osg::ref_ptr<osg::State> state, int width, int height, int msaaSamples);
                    void destroy(osg::GraphicsContext *gc);

                    GLuint getTexture()
                    {
                        return m_Resolve_ColorTex;
                    }
                    int textureWidth() const
                    {
                        return m_width;
                    }
                    int textureHeight() const
                    {
                        return m_height;
                    }
                    int samples() const
                    {
                        return m_samples;
                    }
                    void onPreRender(osg::RenderInfo &renderInfo);
                    void onPostRender(osg::RenderInfo &renderInfo);

                protected:
                    ~TextureBuffer() {}

                    friend class MirrorTexture;

                    GLuint m_Resolve_FBO;       // MSAA FBO is copied to this FBO after render.
                    GLuint m_Resolve_ColorTex;  // color texture for above FBO.
                    GLuint m_MSAA_FBO;          // framebuffer for MSAA RTT
                    GLuint m_MSAA_ColorTex;     // color texture for MSAA RTT
                    GLuint m_MSAA_DepthTex;     // depth texture for MSAA RTT
                    GLint m_width;              // width of texture in pixels
                    GLint m_height;             // height of texture in pixels
                    int m_samples;              // sample width for MSAA

            };

            class MirrorTexture : public osg::Referenced
            {
                public:
                    MirrorTexture(osg::ref_ptr<osg::State> state, GLint width, GLint height);

                    void destroy(osg::GraphicsContext *gc);
                    void blitTexture(osg::GraphicsContext *gc, TextureBuffer *leftEye,  TextureBuffer *rightEye);
                protected:
                    ~MirrorTexture() {}

                    GLuint m_mirrorFBO;
                    GLuint m_mirrorTex;
                    GLint m_width;
                    GLint m_height;
            };
        }
    }
}

#endif
