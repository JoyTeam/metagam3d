#include <metagam3d/ObjectDestroyMessage>
#include <metagam3d/Engine>
#include <metagam3d/ObjectManager>
#include <metagam3d/Object>

namespace mg {

ObjectDestroyMessage::ObjectDestroyMessage(ObjectId objid):
    m_objid(objid)
{
}

ObjectDestroyMessage::~ObjectDestroyMessage()
{
}

void ObjectDestroyMessage::deliver(Engine *engine)
{
    ObjectManager *oman = engine->objectManager();
    Object *obj = oman->find(m_objid);
    if (obj) {
        obj->destroy();
    }
}

} // namespace mg
