#include <metagam3d/DynamicValue>
#include <metagam3d/Variant>
#include <metagam3d/ScriptExpression>
#include <cassert>

using namespace mg;
using namespace osg;

void testStatic()
{
    {
        DynamicValue v(Variant(5.0));
        Variant output;
        v.evaluate(1.0, output);
        assert(output == Variant(5.0));
    }
}

void testDynamic()
{
    {
        osg::ref_ptr<ScriptExpression> e1 = new ScriptGlobExpression("t");
        osg::ref_ptr<ScriptExpression> e2 = new ScriptConstExpression(Variant(9.0));
        osg::ref_ptr<ScriptBinaryExpression> e = new ScriptBinaryExpression(ScriptBinaryExpression::PLUS, e1, e2);
        DynamicValue v(e);
        Variant output;
        v.evaluate(1.0, output);
        assert(output == Variant(10.0));
        v.evaluate(2.0, output);
        assert(output == Variant(11.0));
    }
}

int main(int argc, char **argv)
{
    testStatic();
    testDynamic();
    return 0;
}
