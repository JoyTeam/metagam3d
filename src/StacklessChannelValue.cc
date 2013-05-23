#include <metagam3d/StacklessChannelValue>

using namespace boost::python;

namespace mg {

StacklessChannelValue::StacklessChannelValue():
    m_type(NONE)
{
}

StacklessChannelValue::StacklessChannelValue(long val):
    m_type(LONG),
    m_long_val(val)
{
}

StacklessChannelValue::StacklessChannelValue(const StacklessChannelValue &v)
{
    *this = v;
}

StacklessChannelValue &StacklessChannelValue::operator=(const StacklessChannelValue &v)
{
    m_type = v.m_type;
    switch (m_type) {
    case NONE:
        break;
    case LONG:
        m_long_val = v.m_long_val;
        break;
    }
    return *this;
}

object StacklessChannelValue::getPythonObject() const
{
    switch (m_type) {
    case NONE:
        return object();
    case LONG:
        return object(m_long_val);
    }
}

} // namespace mg
