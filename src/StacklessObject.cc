#include <boost/python.hpp>
#include <metagam3d/StacklessObject>
#include <metagam3d/ObjectDestroyMessage>

using namespace boost::python;

namespace mg {

static ObjectId get_id(const StacklessObject &obj)
{
    return obj.id();
}

void export_objects()
{
    class_<StacklessObject>("Object", no_init).
        def(init<ObjectId>()).
        add_property("id", &get_id).
        def("destroy", &StacklessObject::destroy);
}

StacklessObject::StacklessObject(ObjectId id):
    m_id(id)
{
}

StacklessObject::~StacklessObject()
{
}

ObjectId StacklessObject::id() const
{
    return m_id;
}

void StacklessObject::destroy()
{
    ObjectDestroyMessage *msg = new ObjectDestroyMessage(m_id);
    msg->send();
}

} // namespace mg
