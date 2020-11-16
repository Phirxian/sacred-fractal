#include <string>
#include <iostream>

#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Depth>

#include "gui/Editor.h"
#include "openvr/Viewer.h"

#include "graphics/Program.h"
#include "../../sounds/generator/BounceGenerator.h"

using namespace sacred;
struct EmptyShape : public osg::Shape
{
    virtual Object* cloneType() const { return new EmptyShape; }
    virtual Object* clone(const osg::CopyOp&) const { return new EmptyShape; }
    virtual void accept(osg::ShapeVisitor&){ }
    virtual void accept(osg::ConstShapeVisitor&) const { }
};

void fix(osg::ref_ptr<osg::Group> root, osg::GraphicsContext *gc)
{
    // simple trick to fix rendering when the scene is "empty"
    auto p_node = new osg::Geode();
    p_node->addDrawable(new osg::ShapeDrawable(new EmptyShape));
    root->addChild(p_node);
    // enable opengl 3.x shader mode
    gc->getState()->setUseModelViewAndProjectionUniforms(true);
    gc->getState()->setCheckForGLErrors(osg::State::CheckForGLErrors::ONCE_PER_ATTRIBUTE);
}

void loadScene(osg::Group *root, osg::Camera *camera)
{
	auto prg = new graphics::Program(camera);
	prg->setShader("shaders/frag/forcefield.frag", graphics::EST_FRAGMENT);
	prg->setShader("shaders/vert/test.vert", graphics::EST_VERTEX);
	prg->get(0) = std::make_shared<sounds::BounceGenerator>();
	root->addChild(prg->getNode());
}

int main(int argc, char **argv)
{
    osg::ArgumentParser arguments(&argc, argv);

    osgViewer::ViewerBase::Cameras cams;
    osg::ref_ptr<openvr::device::Device> device = nullptr;
    osg::GraphicsContext *gc = nullptr;
    osg::ref_ptr<osgViewer::Viewer> viewer;
    osg::ref_ptr<osg::Group> root;

    if(openvr::device::Device::hmdPresent())
    {
        osg::notify(osg::FATAL) << "Valid HMD present!" << std::endl;
        device = new openvr::device::Device(0.01f, 10000.0f, 1.0f, 4);
        osg::notify(osg::FATAL) << "device initialized" << std::endl;

        if(!device->hmdInitialized()) { return 1; }

        osg::ref_ptr<osg::GraphicsContext::Traits> traits = device->graphicsContextTraits();
        traits->windowName = "OsgOpenVRViewerExample";

        gc = osg::GraphicsContext::createGraphicsContext(traits);

        if(!gc)
        {
            osg::notify(osg::NOTICE) << "Error, GraphicsWindow has not been created successfully" << std::endl;
            return 1;
        }

        gc->setClearColor(osg::Vec4(0.2f, 0.2f, 0.4f, 1.0f));
        gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        viewer = new openvr::Viewer(arguments, dynamic_cast<osgViewer::GraphicsWindow *>(gc));
        viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);
        viewer->getCamera()->setGraphicsContext(gc);
        viewer->getCamera()->setViewport(0, 0, traits->width, traits->height);
	
        viewer->getCamera()->setComputeNearFarMode(osg::CullSettings::DO_NOT_COMPUTE_NEAR_FAR);
        viewer->setCameraManipulator(new osgGA::TrackballManipulator);
        viewer->getCameras(cams, false);
		
		
        osg::notify(osg::FATAL) << "viewer initialized" << std::endl;

        osg::ref_ptr<openvr::device::RealizeOperation> vr_realize = new openvr::device::RealizeOperation(device);
        osg::notify(osg::FATAL) << "vr_realize initialized" << std::endl;
		
        //root = new osg::Group();
        root = new openvr::Group(viewer, device, vr_realize);
        fix(root, cams[0]->getGraphicsContext());
        loadScene(root, cams[0]);
		
        osg::notify(osg::FATAL) << "Group initialized" << std::endl;

        viewer->setRealizeOperation(vr_realize);
        viewer->addEventHandler(new osgViewer::StatsHandler);
        viewer->addEventHandler(new openvr::device::EventHandler(device));
		
        osg::notify(osg::FATAL) << "initialization done" << std::endl;
    }
    else
    {
        viewer = new osgViewer::Viewer();
        viewer->setUpViewInWindow(0, 0, 1912, 900);
        viewer->getCameras(cams, false);
		
        root = new osg::Group();
        fix(root, cams[0]->getGraphicsContext());
	
        {
          auto gui = gui::Editor(cams[0], root);
          auto handler = new gui::ImGuiHandler(&gui);
          
          handler->setCameraCallbacks(cams[0]);
          if(device)
          {
            osg::notify(osg::FATAL) << "initialize openvr gui" << std::endl;
            openvr::Group *vr = (openvr::Group*)root.get();
            vr->setImGuiHandler(handler);
          }
          
          viewer->addEventHandler(handler);
        }
    }
		

	// gui component

    viewer->setSceneData(root);
    viewer->run();

    if(device)
        device->shutdown(gc);

    return 0;
}
