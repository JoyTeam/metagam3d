#include <metagam3d/ScriptExpression>
#include <metagam3d/ScriptEnvironment>

namespace mg {

void export_script_expression()
{
}

/*
 * ScriptExpression
 */

ScriptExpression::ScriptExpression()
{
    setThreadSafeRefUnref(true);
}

ScriptExpression::~ScriptExpression()
{
}

/*
 * ScriptUnaryExpression
 */

ScriptUnaryExpression::ScriptUnaryExpression(Type type, ScriptExpression *arg1):
    m_type(type),
    m_arg1(arg1)
{
}

ScriptUnaryExpression::~ScriptUnaryExpression()
{
}

Variant ScriptUnaryExpression::evaluate(const ScriptEnvironment &env) const
{
    switch (m_type) {
    case MINUS:
        return -m_arg1->evaluate(env);
    case NOT:
        return Variant(m_arg1->evaluate(env) ? 0 : 1);
    };
}

/*
 * ScriptBinaryExpression
 */

ScriptBinaryExpression::ScriptBinaryExpression(Type type, ScriptExpression *arg1,
        ScriptExpression *arg2):
    m_type(type),
    m_arg1(arg1),
    m_arg2(arg2)
{
}

ScriptBinaryExpression::~ScriptBinaryExpression()
{
}

Variant ScriptBinaryExpression::evaluate(const ScriptEnvironment &env) const
{
    switch (m_type) {
    case PLUS:
        return m_arg1->evaluate(env) + m_arg2->evaluate(env);
    case MINUS:
        return m_arg1->evaluate(env) - m_arg2->evaluate(env);
    case MULT:
        return m_arg1->evaluate(env) * m_arg2->evaluate(env);
    case DIV:
        return m_arg1->evaluate(env) / m_arg2->evaluate(env);
    case EQUAL:
        return Variant(m_arg1->evaluate(env) == m_arg2->evaluate(env) ? 1 : 0);
    case NOTEQUAL:
        return Variant(m_arg1->evaluate(env) != m_arg2->evaluate(env) ? 1 : 0);
    case LT:
        return Variant(m_arg1->evaluate(env) < m_arg2->evaluate(env) ? 1 : 0);
    case LE:
        return Variant(m_arg1->evaluate(env) <= m_arg2->evaluate(env) ? 1 : 0);
    case GT:
        return Variant(m_arg1->evaluate(env) > m_arg2->evaluate(env) ? 1 : 0);
    case GE:
        return Variant(m_arg1->evaluate(env) >= m_arg2->evaluate(env) ? 1 : 0);
    case AND:
        {
            Variant v1 = m_arg1->evaluate(env);
            if (!v1)
                return v1;
            return m_arg2->evaluate(env);
        }
    case OR:
        {
            Variant v1 = m_arg1->evaluate(env);
            if (v1)
                return v1;
            return m_arg2->evaluate(env);
        }
    case MOD:
        return m_arg1->evaluate(env) % m_arg2->evaluate(env);
    };
}

/*
 * ScriptTernaryExpression
 */

ScriptTernaryExpression::ScriptTernaryExpression(Type type, ScriptExpression *arg1,
        ScriptExpression *arg2, ScriptExpression *arg3):
    m_type(type),
    m_arg1(arg1),
    m_arg2(arg2),
    m_arg3(arg3)
{
}

ScriptTernaryExpression::~ScriptTernaryExpression()
{
}

Variant ScriptTernaryExpression::evaluate(const ScriptEnvironment &env) const
{
    switch (m_type) {
    case QUESTION:
        if (m_arg1->evaluate(env))
            return m_arg2->evaluate(env);
        else
            return m_arg3->evaluate(env);
    };
}

/*
 * ScriptConstExpression
 */

ScriptConstExpression::ScriptConstExpression(const Variant &val):
    m_val(val)
{
}

ScriptConstExpression::~ScriptConstExpression()
{
}

Variant ScriptConstExpression::evaluate(const ScriptEnvironment &env) const
{
    return m_val;
}

/*
 * ScriptGlobExpression
 */

ScriptGlobExpression::ScriptGlobExpression(const std::string &name):
    m_name(name)
{
}

ScriptGlobExpression::~ScriptGlobExpression()
{
}

Variant ScriptGlobExpression::evaluate(const ScriptEnvironment &env) const
{
    return env.getGlobal(m_name);
}

} // namespace mg
