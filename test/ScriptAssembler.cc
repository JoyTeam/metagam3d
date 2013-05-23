#include <metagam3d/ScriptAssembler>
#include <metagam3d/ScriptExpression>
#include <metagam3d/ScriptEnvironment>
#include <metagam3d/Variant>
#include <cassert>

using namespace mg;
using namespace osg;

void testSimple(const ScriptEnvironment &env)
{
    {
        ScriptExpressionProgram prog;
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant());
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(1)));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(1));
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptGlobInstruction("v1"));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(1.0));
    }
}

void testArithmetic(const ScriptEnvironment &env)
{
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(5.5)));
        prog.addInstruction(new ScriptUnaryInstruction(ScriptUnaryExpression::MINUS));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(-5.5));
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(1)));
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptBinaryInstruction(ScriptBinaryExpression::PLUS));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(4));
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptConstInstruction(Variant(5)));
        prog.addInstruction(new ScriptBinaryInstruction(ScriptBinaryExpression::MINUS));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(-2));
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(1)));
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptConstInstruction(Variant(5)));
        prog.addInstruction(new ScriptBinaryInstruction(ScriptBinaryExpression::MINUS));
        prog.addInstruction(new ScriptBinaryInstruction(ScriptBinaryExpression::MINUS));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(3));
    }
}

void testComparsions(const ScriptEnvironment &env)
{
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptConstInstruction(Variant(5)));
        prog.addInstruction(new ScriptBinaryInstruction(ScriptBinaryExpression::LT));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(1));
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptConstInstruction(Variant(5)));
        prog.addInstruction(new ScriptBinaryInstruction(ScriptBinaryExpression::GT));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(0));
    }
}

void testLogic(const ScriptEnvironment &env)
{
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(1)));
        prog.addInstruction(new ScriptConstInstruction(Variant(2)));
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptTernaryInstruction(ScriptTernaryExpression::QUESTION));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(2));
    }
    {
        ScriptExpressionProgram prog;
        prog.addInstruction(new ScriptConstInstruction(Variant(0)));
        prog.addInstruction(new ScriptConstInstruction(Variant(2)));
        prog.addInstruction(new ScriptConstInstruction(Variant(3)));
        prog.addInstruction(new ScriptTernaryInstruction(ScriptTernaryExpression::QUESTION));
        osg::ref_ptr<ScriptExpression> e = prog.evaluate();
        assert(e->evaluate(env) == Variant(3));
    }
}

int main(int argc, char **argv)
{
    ScriptEnvironment env;
    env.setGlobal("v1", Variant(1.0));
    env.setGlobal("v2", Variant(2.0));

    testSimple(env);
    testArithmetic(env);
    testComparsions(env);
    testLogic(env);

    return 0;
}
