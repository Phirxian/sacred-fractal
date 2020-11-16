#ifndef __SACRED_GRAPHICS_PROGRAM__
#define __SACRED_GRAPHICS_PROGRAM__

#include <osg/Camera>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Vec3>

#include <osg/Program>
#include <osg/Shader>
#include <osg/Uniform>
#include <osg/Texture2DArray>

#include <osgDB/FileUtils>

#include "../sounds/SignalSource.h"

namespace sacred
{
    namespace graphics
    {
        enum ShaderType
        {
            EST_FRAGMENT,
            EST_VERTEX,
            EST_COUNT
        };

        enum UniformType
        {
            EUT_TIME,
            EUT_ELAPSED,
            EUT_SIZE,
            EUT_VIEW,
            EUT_SAMPLES,
            EUT_STEREO,
            EUT_COUNT
        };

        class Program : public sounds::SignalSource
        {
            public:
                Program(osg::Camera *, int input_size = 0) noexcept;
                virtual ~Program() noexcept;

                virtual const char* getTypeName() const { return "Render"; }

                bool setShader(const char *filename, ShaderType t = EST_FRAGMENT);

                osg::Geode *getNode() const noexcept
                {
                    return p_node;
                }

                inline const float *getData(unsigned i) const { return m_data[i]; }

                virtual void process() noexcept;

                void update(float) noexcept;
            protected:
                osg::Geode   *p_node;
                osg::Program *p_program;
                osg::Shader  *p_shad[EST_COUNT];
                osg::Image   *p_data[10];
                osg::Texture2DArray *p_text;
            protected:
                osg::Uniform *p_uniform[EUT_COUNT];
                float m_data[10][2*sounds::FRAME_SIZE];
        };
    }
}

#endif
