#include <metagam3d/ObjectParamHandler>
#include <metagam3d/Object>

namespace mg {

ObjectParamHandler::ObjectParamHandler(Object *object):
    m_object(object),
    m_engine(object->engine())
{
}

ObjectParamHandler::~ObjectParamHandler()
{
}

} // namespace mg
