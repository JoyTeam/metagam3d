#include <boost/python.hpp>
#include <metagam3d/types>
#include <metagam3d/Engine>
#include <metagam3d/Object>
#include <metagam3d/ObjectManager>
#include <metagam3d/ObjectVectorHandler>
#include <metagam3d/OSGMessage>
#include <osg/Geode>
#include <osg/Geometry>

using namespace boost;

namespace mg {

/* Get new object identifier */
static ObjectId getObjectId()
{
    static ObjectId id = OBJECT_ID_LINES;
    return ++id;
}

class CreateLineMessage: public OSGMessage {
public:
    CreateLineMessage(ObjectId id, ObjectId parentId):
        m_id(id),
        m_parentId(parentId)
    {
    }

    virtual void deliver(Engine *engine)
    {
        /* Vertices */
        osg::Vec3Array *vertices = new osg::Vec3Array(2);

        /* Colors */
        osg::Vec4Array *colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));

        /* Normals */
        osg::Vec3Array *normals = new osg::Vec3Array;
        normals->push_back(osg::Vec3(0.0, -1.0, 0.0));

        /* Geometry */
        osg::Geometry *geom = new osg::Geometry();
        geom->setVertexArray(vertices);
        geom->setColorArray(colors);
        geom->setColorBinding(osg::Geometry::BIND_OVERALL);
        geom->setNormalArray(normals);
        geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
        geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));

        /* Geode */
        osg::Geode *geode = new osg::Geode();
        geode->addDrawable(geom);

        /* Find parent node */
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
        parentNode->addChild(geode);

        /* Create and register stackless object */
        Object *obj = new mg::Object(engine, m_id, geode);
        obj->addParamHandler(new ObjectVectorHandler(obj, 1, vertices, 0, geom));
        obj->addParamHandler(new ObjectVectorHandler(obj, 2, vertices, 1, geom));
        engine->objectManager()->add(obj);
    }

private:
    ObjectId m_id;
    ObjectId m_parentId;
};

/* Stackless command to create a text object */
static python::object createLine(ObjectId parentId)
{
    /* Send message to OSG thread to create the object */
    ObjectId id = getObjectId();
    CreateLineMessage *msg = new CreateLineMessage(id, parentId);
    msg->send();
    
    return python::object(id);
}

/* Register Stackless objects */
void export_line()
{
    python::def("_createLine", &createLine);
}

} // namespace mg
