#include <boost/python.hpp>
#include <metagam3d/types>
#include <metagam3d/Engine>
#include <metagam3d/ConsoleObject>
#include <metagam3d/ObjectManager>
#include <metagam3d/ObjectMotionHandler>
#include <metagam3d/OSGMessage>
#include <metagam3d/OSGConsole>
#include <osg/MatrixTransform>
#include <osg/Vec4d>
#include <string>

using namespace boost;

namespace mg {

/* Get new object identifier */
static ObjectId getObjectId()
{
    static ObjectId id = OBJECT_ID_CONSOLE;
    return ++id;
}

class CreateConsoleMessage: public OSGMessage {
public:
    CreateConsoleMessage(ObjectId id, ObjectId parentId,
            size_t cols, size_t rows, double fontSize):
        m_id(id),
        m_parentId(parentId),
        m_cols(cols),
        m_rows(rows),
        m_fontSize(fontSize)
    {
    }

    virtual void deliver(Engine *engine)
    {
        /* Create console */
        OSGConsole *console = new OSGConsole(m_cols, m_rows, m_fontSize);

        /* Create transformation matrix */
        osg::MatrixTransform *trans = new osg::MatrixTransform();
        trans->addChild(console);

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
        Object *obj = new ConsoleObject(engine, m_id, trans, console);
        obj->addParamHandler(new ObjectMotionHandler(obj, 1, 2, 3, trans));
        engine->objectManager()->add(obj);
    }

private:
    ObjectId m_id;
    ObjectId m_parentId;
    size_t m_cols;
    size_t m_rows;
    double m_fontSize;
};

class PrintConsoleMessage: public OSGMessage {
public:
    PrintConsoleMessage(ObjectId id, const ConsoleObject::Line &line):
        m_id(id),
        m_line(line)
    {
    }

    virtual void deliver(Engine *engine)
    {
        mg::Object *obj = engine->objectManager()->find(m_id);
        if (!obj)
            return;
        ConsoleObject *console = dynamic_cast<ConsoleObject*>(obj);
        if (!console)
            return;
        console->print(m_line);
    }

private:
    ObjectId m_id;
    ConsoleObject::Line m_line;
};

/* Stackless command to create a console */
static python::object createConsole(ObjectId parentId,
        size_t cols, size_t rows, double fontSize)
{
    /* Send message to OSG thread to create the object */
    ObjectId id = getObjectId();
    CreateConsoleMessage *msg = new CreateConsoleMessage(id, parentId,
            cols, rows, fontSize);
    msg->send();
    
    return python::object(id);
}

/* Stackless command to print a line on the console */
void printConsole(ObjectId objid, const ConsoleObject::Line &line)
{
    PrintConsoleMessage *msg = new PrintConsoleMessage(objid, line);
    msg->send();
}

/* Register Stackless objects */
void export_console()
{
    python::class_<ConsoleObject::LineElement>("ConsoleLineElement", python::no_init).
        def(python::init<osg::Vec3d, std::string>());
    python::class_<ConsoleObject::Line>("ConsoleLine").
        def("add", &ConsoleObject::Line::push_back);
    python::def("_createConsole", &createConsole);
    python::def("_printConsole", &printConsole);
}

} // namespace mg
