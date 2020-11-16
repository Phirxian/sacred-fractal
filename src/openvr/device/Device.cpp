#include "Device.h"

#include "../callback/PreDraw.h"
#include "../callback/PostDraw.h"
#include "Texture.h"

#include <osg/Geometry>
#include <osgViewer/Renderer>
#include <osgViewer/GraphicsWindow>
#include <thread>

#ifdef __WIN32
# include <windows.h>
#endif

static osg::Matrix convertMatrix34(const vr::HmdMatrix34_t &mat34)
{
    osg::Matrix matrix(
        mat34.m[0][0], mat34.m[1][0], mat34.m[2][0], 0.0,
        mat34.m[0][1], mat34.m[1][1], mat34.m[2][1], 0.0,
        mat34.m[0][2], mat34.m[1][2], mat34.m[2][2], 0.0,
        mat34.m[0][3], mat34.m[1][3], mat34.m[2][3], 1.0f
    );
    return matrix;
}

static osg::Matrix convertMatrix44(const vr::HmdMatrix44_t &mat44)
{
    osg::Matrix matrix(
        mat44.m[0][0], mat44.m[1][0], mat44.m[2][0], mat44.m[3][0],
        mat44.m[0][1], mat44.m[1][1], mat44.m[2][1], mat44.m[3][1],
        mat44.m[0][2], mat44.m[1][2], mat44.m[2][2], mat44.m[3][2],
        mat44.m[0][3], mat44.m[1][3], mat44.m[2][3], mat44.m[3][3]
    );
    return matrix;
}

namespace sacred
{
    namespace openvr
    {
        namespace device
        {
            Device::Device(float nearClip, float farClip, const float worldUnitsPerMetre, const int samples) :
                m_vrSystem(nullptr),
                m_vrRenderModels(nullptr),
                m_worldUnitsPerMetre(worldUnitsPerMetre),
                m_mirrorTexture(nullptr),
                m_position(osg::Vec3(0.0f, 0.0f, 0.0f)),
                m_orientation(osg::Quat(0.0f, 0.0f, 0.0f, 1.0f)),
                m_nearClip(nearClip), m_farClip(farClip),
                m_samples(samples)
            {
                for(int i = 0; i < 2; i++)
                    m_textureBuffer[i] = nullptr;

                trySetProcessAsHighPriority();
                // Loading the SteamVR Runtime
                vr::EVRInitError eError = vr::VRInitError_None;
                m_vrSystem = vr::VR_Init(&eError, vr::VRApplication_Scene);

                if(eError != vr::VRInitError_None)
                {
                    m_vrSystem = nullptr;
                    osg::notify(osg::WARN)
                            << "Error: Unable to initialize the OpenVR library.\n"
                            << "Reason: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
                    return;
                }

                if(!vr::VRCompositor())
                {
                    m_vrSystem = nullptr;
                    vr::VR_Shutdown();
                    osg::notify(osg::WARN) << "Error: Compositor initialization failed" << std::endl;
                    return;
                }

                m_vrRenderModels = (vr::IVRRenderModels *)vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &eError);

                if(m_vrRenderModels == nullptr)
                {
                    m_vrSystem = nullptr;
                    vr::VR_Shutdown();
                    osg::notify(osg::WARN)
                            << "Error: Unable to get render model interface!\n"
                            << "Reason: " << vr::VR_GetVRInitErrorAsEnglishDescription(eError) << std::endl;
                    return;
                }

                std::string driverName = GetDeviceProperty(vr::Prop_TrackingSystemName_String);
                std::string deviceSerialNumber = GetDeviceProperty(vr::Prop_SerialNumber_String);
                osg::notify(osg::NOTICE) << "HMD driver name: "<< driverName << std::endl;
                osg::notify(osg::NOTICE) << "HMD device serial number: " << deviceSerialNumber << std::endl;
                osg::notify(osg::NOTICE) << "runtime path: " << vr::VR_RuntimePath() << std::endl;
            }

            std::string Device::GetDeviceProperty(vr::TrackedDeviceProperty prop)
            {
                uint32_t bufferLen = m_vrSystem->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, prop, NULL, 0);

                if(bufferLen == 0)
                    return "";

                char *buffer = new char[bufferLen];
                bufferLen = m_vrSystem->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, prop, buffer, bufferLen);
                std::string result = buffer;
                delete [] buffer;
                return result;
            }

            void Device::createRenderBuffers(osg::ref_ptr<osg::State> state)
            {
                uint32_t renderWidth = 0;
                uint32_t renderHeight = 0;
				
				osg::notify(osg::FATAL) << "GetRecommendedRenderTargetSize" << std::endl;
                m_vrSystem->GetRecommendedRenderTargetSize(&renderWidth, &renderHeight);
				
				osg::notify(osg::FATAL) << "GetRecommendedRenderTargetSize" << std::endl;

                for(int i = 0; i < 2; i++)
                    m_textureBuffer[i] = new TextureBuffer(state, renderWidth, renderHeight, m_samples);

                int mirrorWidth = 800;
                int mirrorHeight = 450;
                m_mirrorTexture = new MirrorTexture(state, mirrorWidth, mirrorHeight);
            }

            void Device::init()
            {
                calculateEyeAdjustment();
                calculateProjectionMatrices();
            }

            bool Device::hmdPresent()
            {
                return vr::VR_IsHmdPresent();
            }

            bool Device::hmdInitialized() const
            {
                return m_vrSystem != nullptr && m_vrRenderModels != nullptr;
            }

            osg::Matrix Device::projectionMatrixCenter() const
            {
                osg::Matrix projectionMatrixCenter;
                projectionMatrixCenter = m_leftEyeProjectionMatrix.operator*(0.5) + m_rightEyeProjectionMatrix.operator*(0.5);
                return projectionMatrixCenter;
            }

            osg::Matrix Device::projectionMatrixLeft() const
            {
                return m_leftEyeProjectionMatrix;
            }

            osg::Matrix Device::projectionMatrixRight() const
            {
                return m_rightEyeProjectionMatrix;
            }

            osg::Matrix Device::projectionOffsetMatrixLeft() const
            {
                osg::Matrix projectionOffsetMatrix;
                float offset = m_leftEyeProjectionMatrix(2, 0);
                projectionOffsetMatrix.makeTranslate(osg::Vec3(-offset, 0.0, 0.0));
                return projectionOffsetMatrix;
            }

            osg::Matrix Device::projectionOffsetMatrixRight() const
            {
                osg::Matrix projectionOffsetMatrix;
                float offset = m_rightEyeProjectionMatrix(2, 0);
                projectionOffsetMatrix.makeTranslate(osg::Vec3(-offset, 0.0, 0.0));
                return projectionOffsetMatrix;
            }

            osg::Matrix Device::viewMatrixLeft() const
            {
                osg::Matrix viewMatrix;
                viewMatrix.makeTranslate(-m_leftEyeAdjust);
                return viewMatrix;
            }

            osg::Matrix Device::viewMatrixRight() const
            {
                osg::Matrix viewMatrix;
                viewMatrix.makeTranslate(-m_rightEyeAdjust);
                return viewMatrix;
            }

            void Device::resetSensorOrientation() const
            {
                m_vrSystem->ResetSeatedZeroPose();
            }

            void Device::updatePose()
            {
                vr::VRCompositor()->SetTrackingSpace(vr::TrackingUniverseSeated);
                vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];

                for(int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i) poses[i].bPoseIsValid = false;

                vr::VRCompositor()->WaitGetPoses(poses, vr::k_unMaxTrackedDeviceCount, NULL, 0);
                // Not sure why, but the openvr hellovr_opengl example only seems interested in the
                // pose transform from the first pose tracking device in the array.
                // i.e. this seems to be the only one that is used to affect the view transform matrix.
                // So, here we do the same.
                const vr::TrackedDevicePose_t &pose = poses[vr::k_unTrackedDeviceIndex_Hmd];

                if(pose.bPoseIsValid)
                {
                    osg::Matrix matrix = convertMatrix34(pose.mDeviceToAbsoluteTracking);
                    osg::Matrix poseTransform = osg::Matrix::inverse(matrix);
                    m_position = poseTransform.getTrans() * m_worldUnitsPerMetre;
                    m_orientation = poseTransform.getRotate();
                }
            }

            class InitialDrawCallback : public osg::Camera::DrawCallback
            {
                public:
                    virtual void operator()(osg::RenderInfo &renderInfo) const
                    {
                        osg::GraphicsOperation *graphicsOperation = renderInfo.getCurrentCamera()->getRenderer();
                        osgViewer::Renderer *renderer = dynamic_cast<osgViewer::Renderer *>(graphicsOperation);

                        if(renderer != nullptr)
                        {
                            // Disable normal OSG FBO camera setup because it will undo the MSAA FBO configuration.
                            renderer->setCameraRequiresSetUp(false);
                        }
                    }
            };

            osg::Camera *Device::createRTTCamera(Device::Eye eye, osg::Transform::ReferenceFrame referenceFrame, const osg::Vec4 &clearColor, osg::GraphicsContext *gc) const
            {
                TextureBuffer *buffer = m_textureBuffer[eye];
                osg::ref_ptr<osg::Camera> camera = new osg::Camera();

                camera->setClearColor(clearColor);
                camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);
                camera->setRenderOrder(osg::Camera::PRE_RENDER, eye);
                camera->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
                camera->setAllowEventFocus(false);
                camera->setReferenceFrame(referenceFrame);
                camera->setViewport(0, 0, buffer->textureWidth(), buffer->textureHeight());
                camera->setGraphicsContext(gc);

                // Here we avoid doing anything regarding OSG camera RTT attachment.
                // Ideally we would use automatic methods within OSG for handling RTT but in this
                // case it seemed simpler to handle FBO creation and selection within this class.
                // This initial draw callback is used to disable normal OSG camera setup which
                // would undo our RTT FBO configuration.

                camera->setInitialDrawCallback(new InitialDrawCallback());
                camera->setPreDrawCallback(new callback::PreDraw(camera.get(), buffer));
                camera->setFinalDrawCallback(new callback::PostDraw(camera.get(), buffer));

                return camera.release();
            }

            bool Device::submitFrame()
            {
                vr::Texture_t leftEyeTexture = {(void *)(intptr_t)m_textureBuffer[0]->getTexture(), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
                vr::Texture_t rightEyeTexture = {(void *)(intptr_t)m_textureBuffer[1]->getTexture(), vr::TextureType_OpenGL, vr::ColorSpace_Gamma };
                
                vr::EVRCompositorError lError = vr::VRCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
                vr::EVRCompositorError rError = vr::VRCompositor()->Submit(vr::Eye_Right, &rightEyeTexture);
                
                return lError == vr::VRCompositorError_None && rError == vr::VRCompositorError_None;
            }

            void Device::blitMirrorTexture(osg::GraphicsContext *gc)
            {
                m_mirrorTexture->blitTexture(gc, m_textureBuffer[0], m_textureBuffer[1]);
            }

            osg::GraphicsContext::Traits *Device::graphicsContextTraits() const
            {
                osg::GraphicsContext::WindowingSystemInterface *wsi = osg::GraphicsContext::getWindowingSystemInterface();

                if(!wsi)
                {
                    osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
                    return 0;
                }

                // Get the screen identifiers set in environment variable DISPLAY
                osg::GraphicsContext::ScreenIdentifier si;
                si.readDISPLAY();

                // If displayNum has not been set, reset it to 0.
                if(si.displayNum < 0)
                {
                    si.displayNum = 0;
                    osg::notify(osg::INFO) << "Couldn't get display number, setting to 0" << std::endl;
                }

                // If screenNum has not been set, reset it to 0.
                if(si.screenNum < 0)
                {
                    si.screenNum = 0;
                    osg::notify(osg::INFO) << "Couldn't get screen number, setting to 0" << std::endl;
                }

                unsigned int width, height;
                wsi->getScreenResolution(si, width, height);

                osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
                traits->hostName = si.hostName;
                traits->screenNum = si.screenNum;
                traits->displayNum = si.displayNum;
                traits->windowDecoration = true;
                traits->x = 50;
                traits->y = 50;
                traits->width = 800;
                traits->height = 450;
                traits->doubleBuffer = true;
                traits->sharedContext = nullptr;
                traits->vsync = false; // VSync should always be disabled for because HMD submit handles the timing of the swap.

                return traits.release();
            }

            void Device::shutdown(osg::GraphicsContext *gc)
            {
                // Delete mirror texture
                //if(m_mirrorTexture.valid())
                {
                    m_mirrorTexture->destroy(gc);
                    m_mirrorTexture = nullptr;
                }

                // Delete texture and depth buffers
                for(int i = 0; i < 2; i++)
                {
                    //if(m_textureBuffer[i].valid())
                    {
                        m_textureBuffer[i]->destroy(gc);
                        m_textureBuffer[i] = nullptr;
                    }
                }

                if(m_vrSystem != nullptr)
                {
                    vr::VR_Shutdown();
                    m_vrSystem = nullptr;
                }
            }

            Device::~Device()
            {
                // shutdown(gc);
            }

            void Device::calculateEyeAdjustment()
            {
                vr::HmdMatrix34_t mat;

                mat = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Left);
                m_leftEyeAdjust = convertMatrix34(mat).getTrans();

                mat = m_vrSystem->GetEyeToHeadTransform(vr::Eye_Right);
                m_rightEyeAdjust = convertMatrix34(mat).getTrans();

                // Display IPD
                float ipd = (m_leftEyeAdjust - m_rightEyeAdjust).length();
                osg::notify(osg::ALWAYS) << "Interpupillary Distance (IPD): " << ipd * 1000.0f << " mm" << std::endl;

                // Scale to world units
                m_leftEyeAdjust *= m_worldUnitsPerMetre;
                m_rightEyeAdjust *= m_worldUnitsPerMetre;
            }

            void Device::calculateProjectionMatrices()
            {
                vr::HmdMatrix44_t mat;

                mat = m_vrSystem->GetProjectionMatrix(vr::Eye_Left, m_nearClip, m_farClip);
                m_leftEyeProjectionMatrix = convertMatrix44(mat);

                mat = m_vrSystem->GetProjectionMatrix(vr::Eye_Right, m_nearClip, m_farClip);
                m_rightEyeProjectionMatrix = convertMatrix44(mat);
            }

            void Device::trySetProcessAsHighPriority() const
            {
                // Require at least 4 processors, otherwise the process could occupy the machine.
                //if(OpenThreads::GetNumberOfProcessors() >= 4) undefined
                if(std::thread::hardware_concurrency() >= 4)
                {
                    #ifdef _WIN32
                    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
                    #endif
                }
            }

            void RealizeOperation::operator()(osg::GraphicsContext *gc)
            {
                if(!m_realized)
                {
                    std::lock_guard<std::mutex> lock(_mutex);
                    gc->makeCurrent();

                    if(osgViewer::GraphicsWindow *window = dynamic_cast<osgViewer::GraphicsWindow *>(gc))
                    {
                        // Run wglSwapIntervalEXT(0) to force VSync Off
                        window->setSyncToVBlank(false);
                    }

                    osg::ref_ptr<osg::State> state = gc->getState();
                    m_device->createRenderBuffers(state);
                    // Init the openvr system
                    m_device->init();
                }

                m_realized = true;
            }
        }
    }
}


