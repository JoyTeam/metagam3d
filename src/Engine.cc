#include <metagam3d/Engine>
#include <metagam3d/OSGMailbox>
#include <metagam3d/ObjectManager>
#include <metagam3d/CameraManipulator>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Vec4d>
#include <unistd.h>

using namespace boost;

namespace mg {

Engine *Engine::m_engineInstance = NULL;

class EngineUpdater: public osgGA::GUIEventHandler {
public:
    EngineUpdater(Engine *engine):
        m_engine(engine)
    {
    }

    virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
    {
        switch (ea.getEventType()) {
        case osgGA::GUIEventAdapter::FRAME:
            m_engine->update(ea.getTime());
            return false;
        default:
            return false;
        }
    }

private:
    /* This is not ref_ptr intentionally */
    Engine *m_engine;
};

Engine::Engine(int argc, char **argv):
    m_stackless(argc, argv),
    m_initialized(false),
    m_time(0.0)
{
    m_viewer = new osgViewer::Viewer;
    m_scene = new osg::Group;
    m_scene->setDataVariance(osg::Object::DYNAMIC);
    m_viewer->setSceneData(m_scene.get());
    m_viewer->getCamera()->setClearColor(osg::Vec4d(0.0, 0.0, 0.0, 1.0)); 
    m_viewer->getLight()->setAmbient(osg::Vec4(0.0, 0.0, 0.0, 1.0));
    m_viewer->getLight()->setDiffuse(osg::Vec4(0.3, 0.3, 0.3, 1.0));
    m_viewer->getLight()->setSpecular(osg::Vec4(0.3, 0.3, 0.3, 1.0));
    m_viewer->setLightingMode(osg::View::SKY_LIGHT);
    m_viewer->addEventHandler(new EngineUpdater(this));
    m_objectManager = new ObjectManager;
    m_cameraManipulator = new CameraManipulator(this);
    m_viewer->setCameraManipulator(m_cameraManipulator);
    m_engineInstance = this;
}

Engine::~Engine()
{
    m_engineInstance = NULL;
}

void Engine::installStatsHandler()
{
    m_viewer->addEventHandler(new osgViewer::StatsHandler);
}

int Engine::run()
{
    if (!m_initialized) {
        OSGMailbox *mailbox = OSGMailbox::instance();
        mailbox->setEngine(this);
        mailbox->reg(m_viewer.get());
        m_stackless.start();
        m_initialized = true;
    }
    int res = m_viewer->run();
    _exit(res);
}

void Engine::update(double time)
{
    m_time = time;
}

Engine *Engine::instance()
{
    return m_engineInstance;
}

} // namespace mg
