#include <boost/python.hpp>
#include <metagam3d/StacklessObjectParam>
#include <metagam3d/StacklessChannel>
#include <metagam3d/ObjectParamSetMessage>

using namespace boost::python;

namespace mg {

static ParamId get_id(const StacklessObjectParam &param)
{
    return param.paramid();
}

void export_object_params()
{
    class_<StacklessObjectParam>("ObjectParam", no_init).
        def(init<ObjectId, ParamId>()).
        add_property("id", &get_id).
        def("setValue", &StacklessObjectParam::setValue).
        def("getValue", &StacklessObjectParam::getValue);
}

StacklessObjectParam::StacklessObjectParam(ObjectId objid, ParamId paramid):
    m_objid(objid),
    m_paramid(paramid)
{
}

StacklessObjectParam::~StacklessObjectParam()
{
}

ObjectId StacklessObjectParam::objid() const
{
    return m_objid;
}

ParamId StacklessObjectParam::paramid() const
{
    return m_paramid;
}

void StacklessObjectParam::setValue(const DynamicValue &val)
{
    ObjectParamSetMessage *msg = new ObjectParamSetMessage(m_objid);
    msg->addParam(m_paramid, val);
    msg->send();
    m_value = val;
}

Variant StacklessObjectParam::getValue(double t) const
{
    Variant var;
    m_value.evaluate(t, var);
    return var;
}

} // namespace mg
