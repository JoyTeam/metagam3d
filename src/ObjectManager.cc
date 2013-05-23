#include <metagam3d/Object>
#include <metagam3d/ObjectManager>

namespace mg {

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::add(Object *obj)
{
    m_objects[obj->id()] = obj;
}

void ObjectManager::remove(ObjectId objId)
{
    ObjectMap::iterator obj = m_objects.find(objId);
    if (obj != m_objects.end())
        m_objects.erase(obj);
}

Object *ObjectManager::find(ObjectId objId)
{
    ObjectMap::const_iterator obj = m_objects.find(objId);
    Object *ret;
    if (obj == m_objects.end()) {
        ret = NULL;
    } else {
        ret = obj->second.get();
    }
    return ret;
}

} // namespace mg
