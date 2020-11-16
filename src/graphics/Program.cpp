#include "Program.h"

#include <osg/Material>
#include <iostream>

#include "callback/UniformResolutionCallback.h"
#include "callback/UniformMatrixViewCallback.h"
#include "callback/UniformTimeCallback.h"
#include "callback/UniformStereoCallback.h"

#include "shape/Sphere.h"

namespace sacred
{
    namespace graphics
    {
        static osg::Shader::Type OsgShaderLink[] = {
            osg::Shader::FRAGMENT,
            osg::Shader::VERTEX,
            osg::Shader::UNDEFINED
        };

        Program::Program(osg::Camera *camera, int frame_size) noexcept
            : sounds::SignalSource(false, 1)
        {
            for(int i = 0; i<10; ++i)
            {
                p_data[i] = new osg::Image();
                p_data[i]->setImage(
                    sounds::FRAME_SIZE, 2, 1,
                    GL_R32F, GL_RED, GL_FLOAT,
                    (unsigned char *) m_data[i],
                    osg::Image::NO_DELETE
                );
            }

            p_text = new osg::Texture2DArray();
            p_text->setInternalFormat(GL_R32F);
            p_text->setSourceFormat(GL_RED);
            p_text->setSourceType(GL_FLOAT);
            p_text->setTextureSize(sounds::FRAME_SIZE, 2, 10);
            p_text->setDataVariance(osg::Object::DYNAMIC);

            for(int i =0; i<10; ++i)
                p_text->setImage(i, p_data[i]);

            p_text->setWrap(osg::Texture2DArray::WRAP_R, osg::Texture2DArray::REPEAT);
            p_text->setWrap(osg::Texture2DArray::WRAP_S, osg::Texture2DArray::REPEAT);
            p_text->setWrap(osg::Texture2DArray::WRAP_T, osg::Texture2DArray::REPEAT);
            p_text->setUseHardwareMipMapGeneration(false);
            p_text->setFilter(osg::Texture::MIN_FILTER , osg::Texture::LINEAR);
            p_text->setFilter(osg::Texture::MAG_FILTER , osg::Texture::LINEAR);

            for(int i = 0; i<EST_COUNT; ++i)
                p_shad[i] = new osg::Shader(OsgShaderLink[i]);

            p_program = new osg::Program;
            p_program->setName("blocky");

            p_uniform[EUT_SIZE]    = new osg::Uniform("resolution", osg::Vec2f(800, 600));
            p_uniform[EUT_VIEW]    = new osg::Uniform("view",       osg::Matrixd::identity());
            p_uniform[EUT_TIME]    = new osg::Uniform("time",       0.f);
            p_uniform[EUT_ELAPSED] = new osg::Uniform("elapsed",    0.f);
            p_uniform[EUT_STEREO]  = new osg::Uniform("stereo",    (float)isStereo());

            p_uniform[EUT_SAMPLES] = new osg::Uniform(osg::Uniform::SAMPLER_2D_ARRAY, "samples");
            p_uniform[EUT_SAMPLES]->set(0);

            p_uniform[EUT_SIZE]->setUpdateCallback(new UniformResolutionCallback(camera));
            p_uniform[EUT_VIEW]->setUpdateCallback(new UniformMatrixViewCallback(camera));
            p_uniform[EUT_TIME]->setUpdateCallback(new UniformTimeCallback(this));
            p_uniform[EUT_STEREO]->setUpdateCallback(new UniformStereoCallback(this));

            p_node = new osg::Geode();
//            p_node->addDrawable(new shape::IcoSphere(3));
            p_node->addDrawable(new osg::ShapeDrawable(new osg::Sphere()));
        }

        Program::~Program() noexcept
        {
        }

        void Program::process() noexcept
        {
            if(m_inputs[0] != nullptr)
            {
                auto frame = m_inputs[0]->getFrame();

                // save the last signal
                for(int i = 0; i<2*sounds::FRAME_SIZE; ++i)
                    m_data[0][i] = m_data[1][i];

                for(int i = 0; i<sounds::FRAME_SIZE; ++i)
                    m_data[1][i] = frame->data[i];

                for(int i = 0; i<sounds::FRAME_SIZE; ++i)
                    m_data[1][i + sounds::FRAME_SIZE] = frame->data[i+ frame->stereo*sounds::FRAME_SIZE];
            }
            else
            {
                std::fill(m_data[0], m_data[1]+2*sounds::FRAME_SIZE, 1.f);
                std::fill(m_data[1], m_data[1]+2*sounds::FRAME_SIZE, 1.f);
            }
        }

        bool Program::setShader(const char *filename, ShaderType t)
        {
            auto ss = p_node->getOrCreateStateSet();
            std::string fqFileName = osgDB::findDataFile(filename);

            if(fqFileName.length() == 0)
            {
                std::cout << "File \"" << filename << "\" not found." << std::endl;
                return false;
            }

            bool success = p_shad[t]->loadShaderSourceFromFile(fqFileName.c_str());

            if(!success)
            {
                std::cout << "Couldn't load file: " << filename << std::endl;
                return false;
            }

            p_shad[t]->dirtyShader();
            p_program->addShader(p_shad[t]);

            for(int i = 0; i<EUT_COUNT; ++i)
                ss->addUniform(p_uniform[i]);

            ss->ref();
            ss->setAttribute(new osg::Material());
            ss->setTextureAttributeAndModes(0, p_text, osg::StateAttribute::ON);
            ss->setAttributeAndModes(p_program, osg::StateAttribute::ON);

            return true;
        }

        void Program::update(float time) noexcept
        {
            float freq = 1.0 / 30;
            float elapsed = time/freq - int(time / freq);
            p_uniform[EUT_ELAPSED]->set(elapsed);
            getProcessedFrame(time/freq);

            for(int i = 0; i<10; ++i)
                p_data[i]->dirty();
        }
    }
}
