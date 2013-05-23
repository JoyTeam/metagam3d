#include <boost/python.hpp>
#include <metagam3d/ScriptAssembler>

using namespace boost::python;

namespace mg {

static void addInstruction(ScriptExpressionProgram &prog, const ScriptInstruction &inst)
{
    prog.addInstruction(inst.clone());
}

void export_script_assembler()
{
    class_<ScriptInstruction>("ScriptInstruction", no_init);
    class_<ScriptExpressionProgram>("ScriptExpressionProgram").
        def("addInstruction", &addInstruction);
    class_<ScriptConstInstruction, bases<ScriptInstruction> >("ScriptConstInstruction", no_init).
        def(init<Variant>());
    class_<ScriptGlobInstruction, bases<ScriptInstruction> >("ScriptGlobInstruction", no_init).
        def(init<std::string>());
    {
        scope sc = class_<ScriptUnaryInstruction, bases<ScriptInstruction> >("ScriptUnaryInstruction", no_init).
            def(init<ScriptUnaryExpression::Type>());
        enum_<ScriptUnaryExpression::Type>("Type");
        scope().attr("MINUS") = ScriptUnaryExpression::MINUS;
        scope().attr("NOT") = ScriptUnaryExpression::NOT;
    }
    {
        scope sc = class_<ScriptBinaryInstruction, bases<ScriptInstruction> >("ScriptBinaryInstruction", no_init).
            def(init<ScriptBinaryExpression::Type>());
        enum_<ScriptBinaryExpression::Type>("Type");
        scope().attr("PLUS") = ScriptBinaryExpression::PLUS;
        scope().attr("MINUS") = ScriptBinaryExpression::MINUS;
        scope().attr("MULT") = ScriptBinaryExpression::MULT;
        scope().attr("DIV") = ScriptBinaryExpression::DIV;
        scope().attr("EQUAL") = ScriptBinaryExpression::EQUAL;
        scope().attr("NOTEQUAL") = ScriptBinaryExpression::NOTEQUAL;
        scope().attr("LT") = ScriptBinaryExpression::LT;
        scope().attr("LE") = ScriptBinaryExpression::LE;
        scope().attr("GT") = ScriptBinaryExpression::GT;
        scope().attr("GE") = ScriptBinaryExpression::GE;
        scope().attr("AND") = ScriptBinaryExpression::AND;
        scope().attr("OR") = ScriptBinaryExpression::OR;
        scope().attr("MOD") = ScriptBinaryExpression::MOD;
    }
    {
        scope sc = class_<ScriptTernaryInstruction, bases<ScriptInstruction> >("ScriptTernaryInstruction", no_init).
            def(init<ScriptTernaryExpression::Type>());
        enum_<ScriptTernaryExpression::Type>("Type");
        scope().attr("QUESTION") = ScriptTernaryExpression::QUESTION;
    }
}

/* ScriptInstruction */

ScriptInstruction::ScriptInstruction()
{
    setThreadSafeRefUnref(true);
}

ScriptInstruction::~ScriptInstruction()
{
}

void ScriptInstruction::execute(ScriptExpressionStack &stack) const
{
}

ScriptInstruction *ScriptInstruction::clone() const
{
    return new ScriptInstruction();
}

/* ScriptExpressionProgram */

ScriptExpressionProgram::ScriptExpressionProgram()
{
}

ScriptExpressionProgram::~ScriptExpressionProgram()
{
}

void ScriptExpressionProgram::addInstruction(ScriptInstruction *inst)
{
    m_instructions.push_back(inst);
}

osg::ref_ptr<ScriptExpression> ScriptExpressionProgram::evaluate() const
{
    ScriptExpressionStack stack;

    /* Execute all instructions in order */
    ScriptInstructions::const_iterator it = m_instructions.begin();
    while (it != m_instructions.end()) {
        (*it)->execute(stack);
        ++it;
    }

    /* Incorrect assembler program left no expression on the stack */
    if (stack.empty())
        return new ScriptConstExpression(Variant());

    /* This value is the result */
    osg::ref_ptr<ScriptExpression> result = stack.top();
    stack.pop();

    /* Incorrect assembler program left excessive expressions on the stack */
    while (!stack.empty()) {
        stack.pop();
    }
    return result;
}

/* ScriptConstInstruction */

ScriptConstInstruction::ScriptConstInstruction(const Variant &var):
    m_var(var)
{
}

void ScriptConstInstruction::execute(ScriptExpressionStack &stack) const
{
    stack.push(new ScriptConstExpression(m_var));
}

ScriptInstruction *ScriptConstInstruction::clone() const
{
    return new ScriptConstInstruction(m_var);
}

/* ScriptGlobInstruction */

ScriptGlobInstruction::ScriptGlobInstruction(const std::string &name):
    m_name(name)
{
}

void ScriptGlobInstruction::execute(ScriptExpressionStack &stack) const
{
    stack.push(new ScriptGlobExpression(m_name));
}

ScriptInstruction *ScriptGlobInstruction::clone() const
{
    return new ScriptGlobInstruction(m_name);
}

/* ScriptUnaryInstruction */

ScriptUnaryInstruction::ScriptUnaryInstruction(ScriptUnaryExpression::Type type):
    m_type(type)
{
}

void ScriptUnaryInstruction::execute(ScriptExpressionStack &stack) const
{
    if (stack.empty())
        return;
    osg::ref_ptr<ScriptExpression> e1 = stack.top();
    stack.pop();
    stack.push(new ScriptUnaryExpression(m_type, e1));
}

ScriptInstruction *ScriptUnaryInstruction::clone() const
{
    return new ScriptUnaryInstruction(m_type);
}

/* ScriptBinaryInstruction */

ScriptBinaryInstruction::ScriptBinaryInstruction(ScriptBinaryExpression::Type type):
    m_type(type)
{
}

void ScriptBinaryInstruction::execute(ScriptExpressionStack &stack) const
{
    if (stack.empty())
        return;
    osg::ref_ptr<ScriptExpression> e2 = stack.top();
    stack.pop();
    if (stack.empty()) {
        return;
    }
    osg::ref_ptr<ScriptExpression> e1 = stack.top();
    stack.pop();
    stack.push(new ScriptBinaryExpression(m_type, e1, e2));
}

ScriptInstruction *ScriptBinaryInstruction::clone() const
{
    return new ScriptBinaryInstruction(m_type);
}

/* ScriptTernaryInstruction */

ScriptTernaryInstruction::ScriptTernaryInstruction(ScriptTernaryExpression::Type type):
    m_type(type)
{
}

void ScriptTernaryInstruction::execute(ScriptExpressionStack &stack) const
{
    if (stack.empty())
        return;
    osg::ref_ptr<ScriptExpression> e3 = stack.top();
    stack.pop();
    if (stack.empty()) {
        return;
    }
    osg::ref_ptr<ScriptExpression> e2 = stack.top();
    stack.pop();
    if (stack.empty()) {
        return;
    }
    osg::ref_ptr<ScriptExpression> e1 = stack.top();
    stack.pop();
    stack.push(new ScriptTernaryExpression(m_type, e1, e2, e3));
}

ScriptInstruction *ScriptTernaryInstruction::clone() const
{
    return new ScriptTernaryInstruction(m_type);
}

} // namespace mg
