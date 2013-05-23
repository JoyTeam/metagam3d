#include <boost/python.hpp>
#include <metagam3d/types>
#include <metagam3d/Engine>
#include <metagam3d/ObjectManager>
#include <metagam3d/Object>
#include <metagam3d/ObjectMotionHandler>
#include <metagam3d/OSGMessage>
#include <osgDB/ReadFile>
#include <osg/MatrixTransform>
#include <string>

using namespace boost;

namespace mg {

/* Get new object identifier */
static ObjectId getObjectId()
{
    static ObjectId id = OBJECT_ID_AUTOLOAD;
    return ++id;
}

class CreateObjectMessage: public OSGMessage {
public:
    CreateObjectMessage(osg::Node *model, ObjectId id, ObjectId parentId,
            unsigned int flags):
        m_model(model),
        m_id(id),
        m_parentId(parentId),
        m_flags(flags)
    {
    }

    virtual void deliver(Engine *engine)
    {
        /* Create transformation matrix */
        osg::ref_ptr<osg::MatrixTransform> trans = new osg::MatrixTransform();
        trans->addChild(m_model);

        osg::Group *parentNode = NULL;
        if (m_parentId) {
            /* Add object to the scene as a subobject */
            Object *obj = engine->objectManager()->find(m_parentId);
            if (obj) {
                parentNode = dynamic_cast<osg::Group*>(obj->node());
            }
        }
        if (!parentNode) {
            /* Add object to the scene as a top level object */
            parentNode = engine->scene();
        }
        parentNode->addChild(trans);

        /* Create and register stackless object */
        Object *obj = new mg::Object(engine, m_id, trans);
        obj->addParamHandler(new ObjectMotionHandler(obj, 1, 2, 3, trans));
        engine->objectManager()->add(obj);
    }

private:
    osg::ref_ptr<osg::Node> m_model;
    ObjectId m_id;
    ObjectId m_parentId;
    unsigned int m_flags;
};

/* Stackless command to load object from file */
static python::object loadObject(const std::string &filename, ObjectId parentId,
        unsigned int flags)
{
    static std::map<std::string, osg::ref_ptr<osg::Node> > objectCache;
    osg::Node *loadedModel = 0;

    /* Search cache */
    std::map<std::string, osg::ref_ptr<osg::Node> >::iterator it = objectCache.find(filename);
    if (it != objectCache.end()) {
        /* Cache hit */
        loadedModel = it->second;
    }
    
    if (!loadedModel) {
        /* Cache miss. Load file */
        loadedModel = osgDB::readNodeFile(filename);
        if (!loadedModel)
            return python::object();

        loadedModel->setThreadSafeRefUnref(true);
        objectCache[filename] = loadedModel;
    }

    /* If any flags are specified, clone the object */
    if (flags) {
        loadedModel = dynamic_cast<osg::Node*>(loadedModel->clone(osg::CopyOp(
            osg::CopyOp::DEEP_COPY_NODES |
            osg::CopyOp::DEEP_COPY_DRAWABLES |
            osg::CopyOp::DEEP_COPY_STATESETS |
            osg::CopyOp::DEEP_COPY_STATEATTRIBUTES |
            osg::CopyOp::DEEP_COPY_ALL
        )));
    }

    /* Send message to OSG thread to create the object */
    ObjectId id = getObjectId();
    (new CreateObjectMessage(loadedModel, id, parentId, flags))->send();
    
    return python::object(id);
}

/* Register Stackless objects */
void export_object_loader()
{
    python::def("_loadObject", &loadObject);
}

} // namespace mg
