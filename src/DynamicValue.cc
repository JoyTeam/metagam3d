#include <boost/python.hpp>
#include <metagam3d/DynamicValue>
#include <metagam3d/Variant>
#include <metagam3d/ScriptExpression>
#include <metagam3d/ScriptEnvironment>
#include <metagam3d/ScriptAssembler>

using namespace boost::python;

namespace mg {

static Variant evaluate(const DynamicValue &dynval, double t)
{
    Variant result;
    dynval.evaluate(t, result);
    return result;
}

void export_dynamic_value()
{
    class_<DynamicValue>("DynamicValue").
        def(init<Variant>()).
        def(init<ScriptExpressionProgram&>()).
        def("evaluate", &evaluate).
        def("setTill", &DynamicValue::setTill);
}

DynamicValue::DynamicValue():
    m_dynamic(false),
    m_tillDefined(false)
{
}

DynamicValue::DynamicValue(const Variant &value):
    m_dynamic(false),
    m_tillDefined(false),
    m_staticValue(value)
{
}

DynamicValue::DynamicValue(ScriptExpression *expr):
    m_dynamic(true),
    m_tillDefined(false),
    m_expr(expr)
{
}

DynamicValue::DynamicValue(const ScriptExpressionProgram &prog):
    m_dynamic(true),
    m_tillDefined(false)
{
    m_expr = prog.evaluate();
}

DynamicValue::~DynamicValue()
{
}

void DynamicValue::evaluate(double t, Variant &output) const
{
    if (m_dynamic) {
        if (m_tillDefined && t > m_till) {
            t = m_till;
        }
        ScriptEnvironment env;
        env.setGlobal("t", Variant(t));
        output = m_expr->evaluate(env);
    } else {
        output = m_staticValue;
    }
}

void DynamicValue::forget(double before)
{
    if (m_dynamic && m_tillDefined && m_till <= before) {
        evaluate(m_till, m_staticValue);
        m_dynamic = false;
    }
}

void DynamicValue::evaluateAndForget(double t, Variant &output)
{
    forget(t);
    evaluate(t, output);
}

void DynamicValue::setTill(double till)
{
    m_tillDefined = true;
    m_till = till;
}

} // namespace mg
