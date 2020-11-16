#ifndef _OSG_OPENVRDEVICE_H_
#define _OSG_OPENVRDEVICE_H_

#include <openvr/openvr.h>

#include <osg/Geode>
#include <osg/Texture2D>
#include <osg/Version>
#include <osg/FrameBufferObject>
#include <mutex>

#include "EventHandler.h"
#include "Texture.h"

namespace sacred
{
    namespace openvr
    {
        namespace device
        {
            class Device : public osg::Referenced
            {
                public:
                    typedef enum Eye_
                    {
                        LEFT = 0,
                        RIGHT = 1,
                        COUNT = 2
                    } Eye;

                    Device(float nearClip, float farClip, const float worldUnitsPerMetre = 1.0f, const int samples = 0);

                    void createRenderBuffers(osg::ref_ptr<osg::State> state);

                    void init();
                    void shutdown(osg::GraphicsContext *gc);

                    static bool hmdPresent();
                    bool hmdInitialized() const;

                    osg::Matrix projectionMatrixCenter() const;
                    osg::Matrix projectionMatrixLeft() const;
                    osg::Matrix projectionMatrixRight() const;

                    osg::Matrix projectionOffsetMatrixLeft() const;
                    osg::Matrix projectionOffsetMatrixRight() const;

                    osg::Matrix viewMatrixLeft() const;
                    osg::Matrix viewMatrixRight() const;

                    float nearClip() const
                    {
                        return m_nearClip;
                    }
                    float farClip() const
                    {
                        return m_farClip;
                    }

                    void resetSensorOrientation() const;
                    void updatePose();

                    osg::Vec3 position() const
                    {
                        return m_position;
                    }
                    osg::Quat orientation() const
                    {
                        return m_orientation;
                    }

                    osg::Camera *createRTTCamera(Device::Eye eye, osg::Transform::ReferenceFrame referenceFrame, const osg::Vec4 &clearColor, osg::GraphicsContext *gc = 0) const;

                    bool submitFrame();
                    void blitMirrorTexture(osg::GraphicsContext *gc);

                    osg::GraphicsContext::Traits *graphicsContextTraits() const;
                protected:
                    ~Device(); // Since we inherit from osg::Referenced we must make destructor protected

                    void calculateEyeAdjustment();
                    void calculateProjectionMatrices();

                    void trySetProcessAsHighPriority() const;

                    vr::IVRSystem *m_vrSystem;
                    vr::IVRRenderModels *m_vrRenderModels;
                    const float m_worldUnitsPerMetre;

                    //osg::ref_ptr<TextureBuffer> m_textureBuffer[2];
                    //osg::ref_ptr<MirrorTexture> m_mirrorTexture;
                    TextureBuffer *m_textureBuffer[2];
                    MirrorTexture *m_mirrorTexture;

                    osg::Matrixf m_leftEyeProjectionMatrix;
                    osg::Matrixf m_rightEyeProjectionMatrix;
                    osg::Vec3f m_leftEyeAdjust;
                    osg::Vec3f m_rightEyeAdjust;

                    osg::Vec3 m_position;
                    osg::Quat m_orientation;

                    float m_nearClip;
                    float m_farClip;
                    int m_samples;
                private:
                    std::string GetDeviceProperty(vr::TrackedDeviceProperty prop);
                    Device(const Device &); // Do not allow copy
                    Device &operator=(const Device &); // Do not allow assignment operator.
            };

            class RealizeOperation : public osg::GraphicsOperation
            {
                public:
                    explicit RealizeOperation(osg::ref_ptr<Device> device) :
                        osg::GraphicsOperation("OpenVRRealizeOperation", false), m_device(device), m_realized(false) {}

                    virtual void operator()(osg::GraphicsContext *gc);

                    bool realized() const
                    {
                        return m_realized;
                    }
                protected:
                    std::mutex  _mutex;
                    osg::observer_ptr<Device> m_device;
                    bool m_realized;
            };
        }
    }
}

#endif /* _OSG_OPENVRDEVICE_H_ */
