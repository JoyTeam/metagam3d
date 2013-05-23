#include <metagam3d/ScriptExpression>
#include <metagam3d/ScriptEnvironment>
#include <metagam3d/Variant>
#include <cassert>

using namespace mg;
using namespace osg;

void testSimple(const ScriptEnvironment &env)
{
    {
        ScriptConstExpression e(Variant(5));
        assert(e.evaluate(env) == Variant(5));
    }
    {
        ScriptGlobExpression e("v1");
        assert(e.evaluate(env) == Variant(1.0));
    }
    {
        ScriptGlobExpression e("v2");
        assert(e.evaluate(env) == Variant(2.0));
    }
    {
        ScriptGlobExpression e("v3");
        assert(e.evaluate(env) == Variant());
    }
}

void testArithmetic(const ScriptEnvironment &env)
{
    {
        ScriptUnaryExpression e(ScriptUnaryExpression::MINUS, new ScriptConstExpression(Variant(5.0)));
        assert(e.evaluate(env) == Variant(-5.0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(5.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(9.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::PLUS, e1, e2);
        assert(e.evaluate(env) == Variant(14.0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(5.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(9.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::MINUS, e1, e2);
        assert(e.evaluate(env) == Variant(-4.0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(5.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(9.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::MULT, e1, e2);
        assert(e.evaluate(env) == Variant(45.0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(9.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::DIV, e1, e2);
        assert(e.evaluate(env) == Variant(4.5));
    }
}

void testComparsions(const ScriptEnvironment &env)
{
    /* == */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::EQUAL, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::EQUAL, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
    /* != */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::NOTEQUAL, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::NOTEQUAL, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    /* < */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::LT, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::LT, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
    /* <= */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::LE, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::LE, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(2.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::LE, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
    /* > */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(3.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::GT, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::GT, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(3.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::GE, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::GE, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(2.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(3.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::GE, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
}

void testLogic(const ScriptEnvironment &env)
{
    /* && */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(0.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::AND, e1, e2);
        assert(e.evaluate(env) == Variant(0.0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::AND, e1, e2);
        assert(e.evaluate(env) == Variant(0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(3.5));
        ScriptBinaryExpression e(ScriptBinaryExpression::AND, e1, e2);
        assert(e.evaluate(env) == Variant(3.5));
    }
    /* || */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(0.0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1.0));
        ScriptBinaryExpression e(ScriptBinaryExpression::OR, e1, e2);
        assert(e.evaluate(env) == Variant(1.0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(1));
        ScriptBinaryExpression e(ScriptBinaryExpression::OR, e1, e2);
        assert(e.evaluate(env) == Variant(1));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(3.5));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2));
        ScriptBinaryExpression e(ScriptBinaryExpression::OR, e1, e2);
        assert(e.evaluate(env) == Variant(3.5));
    }
    /* ! */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1));
        ScriptUnaryExpression e(ScriptUnaryExpression::NOT, e1);
        assert(e.evaluate(env) == Variant(0));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(0));
        ScriptUnaryExpression e(ScriptUnaryExpression::NOT, e1);
        assert(e.evaluate(env) == Variant(1));
    }
    /* ? */
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(1));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2));
        ScriptExpression *e3 = new ScriptConstExpression(Variant(3));
        ScriptTernaryExpression e(ScriptTernaryExpression::QUESTION, e1, e2, e3);
        assert(e.evaluate(env) == Variant(2));
    }
    {
        ScriptExpression *e1 = new ScriptConstExpression(Variant(0));
        ScriptExpression *e2 = new ScriptConstExpression(Variant(2));
        ScriptExpression *e3 = new ScriptConstExpression(Variant(3));
        ScriptTernaryExpression e(ScriptTernaryExpression::QUESTION, e1, e2, e3);
        assert(e.evaluate(env) == Variant(3));
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
