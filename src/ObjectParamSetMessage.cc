#include <metagam3d/ObjectParamSetMessage>
#include <metagam3d/Engine>
#include <metagam3d/ObjectManager>
#include <metagam3d/Object>

namespace mg {

ObjectParamSetMessage::ObjectParamSetMessage(ObjectId objid):
    m_objid(objid)
{
}

ObjectParamSetMessage::~ObjectParamSetMessage()
{
}

void ObjectParamSetMessage::addParam(ParamId paramid, const DynamicValue &val)
{
    m_values[paramid] = val;
}

void ObjectParamSetMessage::deliver(Engine *engine)
{
    ObjectManager *oman = engine->objectManager();
    Object *obj = oman->find(m_objid);
    if (obj) {
        Values::iterator it = m_values.begin();
        while (it != m_values.end()) {
            obj->setParam(it->first, it->second);
            ++it;
        }
    }
}

} // namespace mg
