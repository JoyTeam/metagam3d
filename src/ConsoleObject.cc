#include <metagam3d/ConsoleObject>
#include <metagam3d/OSGConsole>

namespace mg {

ConsoleObject::ConsoleObject(Engine *engine, ObjectId id, osg::Node *node,
        OSGConsole *console):
    Object(engine, id, node),
    m_console(console)
{
}

ConsoleObject::LineElement::LineElement(const osg::Vec3d &_color, const std::string &_str):
    color(_color.x(), _color.y(), _color.z(), 1.0),
    str(_str)
{
}

ConsoleObject::~ConsoleObject()
{
}

void ConsoleObject::print(const Line &line)
{
    m_console->print(line);
}

} // namespace mg
