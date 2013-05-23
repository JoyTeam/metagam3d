#include <metagam3d/Variant>
#include <cassert>

using namespace mg;
using namespace osg;

void testGetters()
{
    {
        Variant v;
        assert(v.type() == Variant::NONE);
        assert(v.integerValue() == 0);
        assert(v.doubleValue() == 0.0);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
        assert(v.stringValue() == "");
    }
    {
        Variant v(1);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 1);
        assert(v.doubleValue() == 1.0);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
        assert(v.stringValue() == "1");
    }
    {
        Variant v(1.5);
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == 1);
        assert(v.doubleValue() == 1.5);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
        assert(v.stringValue() == "1.5");
    }
    {
        Variant v(Vec3d(1.0, 2.0, 3.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.integerValue() == 0);
        assert(v.doubleValue() == 0.0);
        assert(v.vec3dValue() == Vec3d(1.0, 2.0, 3.0));
        assert(v.stringValue() == "(1, 2, 3)");
    }
    {
        Variant v("19.2");
        assert(v.type() == Variant::STRING);
        assert(v.integerValue() == 19);
        assert(v.doubleValue() == 19.2);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
        assert(v.stringValue() == "19.2");
    }
}

void testCopying()
{
    {
        Variant proto;
        Variant copy(proto);
        assert(copy.type() == Variant::NONE);
    }
    {
        Variant proto(5);
        Variant copy(proto);
        assert(copy.type() == Variant::INTEGER);
        assert(copy.integerValue() == 5);
    }
    {
        Variant proto(5.8);
        Variant copy(proto);
        assert(copy.type() == Variant::DOUBLE);
        assert(copy.doubleValue() == 5.8);
    }
    {
        Variant proto(Vec3d(1.1, 2.2, 3.3));
        Variant copy(proto);
        assert(copy.type() == Variant::VEC3D);
        assert(copy.vec3dValue() == Vec3d(1.1, 2.2, 3.3));
    }
    {
        Variant proto("bar");
        Variant copy(proto);
        assert(copy.type() == Variant::STRING);
        assert(copy.stringValue() == "bar");
    }
}

void testAssignment()
{
    {
        Variant proto;
        Variant copy;
        copy = proto;
        assert(copy.type() == Variant::NONE);
    }
    {
        Variant proto(5);
        Variant copy;
        copy = proto;
        assert(copy.type() == Variant::INTEGER);
        assert(copy.integerValue() == 5);
    }
    {
        Variant proto(5.8);
        Variant copy;
        copy = proto;
        assert(copy.type() == Variant::DOUBLE);
        assert(copy.doubleValue() == 5.8);
    }
    {
        Variant proto(Vec3d(1.1, 2.2, 3.3));
        Variant copy;
        copy = proto;
        assert(copy.type() == Variant::VEC3D);
        assert(copy.vec3dValue() == Vec3d(1.1, 2.2, 3.3));
    }
    {
        Variant proto("bar");
        Variant copy;
        copy = proto;
        assert(copy.type() == Variant::STRING);
        assert(copy.stringValue() == "bar");
    }
}

void testAddition()
{
    /* None + ? */
    {
        Variant v;
        v += Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v += Variant(1);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 1);
    }
    {
        Variant v;
        v += Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 5.0);
    }
    {
        Variant v;
        v += Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(4.0, 5.0, 6.0));
    }
    {
        Variant v;
        v += Variant("Hello");
        assert(v.type() == Variant::STRING);
        assert(v.stringValue() == "Hello");
    }
    /* Integer + ? */
    {
        Variant v(1);
        v += Variant();
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 1);
    }
    {
        Variant v(1);
        v += Variant(3);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 4);
    }
    {
        Variant v(1);
        v += Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 6.0);
    }
    {
        Variant v(1);
        v += Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(1);
        v += Variant("Hello");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 1.0);
    }
    /* Double + ? */
    {
        Variant v(1.0);
        v += Variant();
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == 1.0);
    }
    {
        Variant v(1.0);
        v += Variant(3);
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == 4.0);
    }
    {
        Variant v(1.0);
        v += Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 6.0);
    }
    {
        Variant v(1.0);
        v += Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(1.5);
        v += Variant("Hello");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 1.5);
    }
    /* Vec3d + ? */
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v += Variant();
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(10.0, 11.0, 12.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v += Variant(3);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v += Variant(5.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v += Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(14.0, 16.0, 18.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v += Variant("Hello");
        assert(v.type() == Variant::NONE);
    }
    /* String + ? */
    {
        Variant v("1.5");
        v += Variant();
        assert(v.type() == Variant::STRING);
        assert(v.stringValue() == "1.5");
    }
    {
        Variant v("1.5");
        v += Variant(1);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 2.5);
    }
    {
        Variant v("1.5");
        v += Variant(1.5);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 3.0);
    }
    {
        Variant v("1.5");
        v += Variant(Vec3d(1.0, 2.0, 3.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("foo");
        v += Variant("bar");
        assert(v.type() == Variant::STRING);
        assert(v.stringValue() == "foobar");
    }
}

void testSubstraction()
{
    /* None - ? */
    {
        Variant v;
        v -= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v -= Variant(1);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == -1);
    }
    {
        Variant v;
        v -= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -5.0);
    }
    {
        Variant v;
        v -= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(-4.0, -5.0, -6.0));
    }
    {
        Variant v;
        v -= Variant("Hello");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.0);
    }
    /* Integer - ? */
    {
        Variant v(1);
        v -= Variant();
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 1);
    }
    {
        Variant v(1);
        v -= Variant(3);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == -2);
    }
    {
        Variant v(1);
        v -= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -4.0);
    }
    {
        Variant v(1);
        v -= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(1);
        v -= Variant("8");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -7.0);
    }
    /* Double - ? */
    {
        Variant v(1.0);
        v -= Variant();
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == 1.0);
    }
    {
        Variant v(1.0);
        v -= Variant(3);
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == -2.0);
    }
    {
        Variant v(1.0);
        v -= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -4.0);
    }
    {
        Variant v(1.0);
        v -= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(1.5);
        v -= Variant("4");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -2.5);
    }
    /* Vec3d - ? */
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v -= Variant();
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(10.0, 11.0, 12.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v -= Variant(3);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v -= Variant(5.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v -= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(6.0, 6.0, 6.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v -= Variant("Hello");
        assert(v.type() == Variant::NONE);
    }
    /* String - ? */
    {
        Variant v("5");
        v -= Variant();
        assert(v.type() == Variant::STRING);
        assert(v.stringValue() == "5");
    }
    {
        Variant v("5.5");
        v -= Variant(1);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 4.5);
    }
    {
        Variant v("4.5");
        v -= Variant(1.5);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 3.0);
    }
    {
        Variant v("foo");
        v -= Variant(Vec3d(1.0, 2.0, 3.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("8.5");
        v -= Variant("1.5");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 7.0);
    }
}

void testMultiplication()
{
    /* None * ? */
    {
        Variant v;
        v *= Variant();
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 0);
    }
    {
        Variant v;
        v *= Variant(1);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 0);
    }
    {
        Variant v;
        v *= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.0);
    }
    {
        Variant v;
        v *= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
    }
    {
        Variant v;
        v *= Variant("Hello");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.0);
    }
    /* Integer * ? */
    {
        Variant v(1);
        v *= Variant();
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 0);
    }
    {
        Variant v(2);
        v *= Variant(3);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 6);
    }
    {
        Variant v(3);
        v *= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 15.0);
    }
    {
        Variant v(2);
        v *= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(8.0, 10.0, 12.0));
    }
    {
        Variant v(2);
        v *= Variant("8");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 16.0);
    }
    /* Double * ? */
    {
        Variant v(1.0);
        v *= Variant();
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == 0.0);
    }
    {
        Variant v(3.0);
        v *= Variant(3);
        assert(v.type() == Variant::DOUBLE);
        assert(v.integerValue() == 9.0);
    }
    {
        Variant v(3.0);
        v *= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 15.0);
    }
    {
        Variant v(3.0);
        v *= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(12.0, 15.0, 18.0));
    }
    {
        Variant v(1.5);
        v *= Variant("4");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 6.0);
    }
    /* Vec3d * ? */
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v *= Variant();
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v *= Variant(3);
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(30.0, 33.0, 36.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v *= Variant(5.0);
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(50.0, 55.0, 60.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v *= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(0.0, 0.0, 0.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v *= Variant("2.0");
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(20.0, 22.0, 24.0));
    }
    /* String * ? */
    {
        Variant v("5");
        v *= Variant();
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.0);
    }
    {
        Variant v("5.5");
        v *= Variant(2);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 11.0);
    }
    {
        Variant v("4.5");
        v *= Variant(1.5);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 6.75);
    }
    {
        Variant v("2.0");
        v *= Variant(Vec3d(1.0, 2.0, 3.0));
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(2.0, 4.0, 6.0));
    }
    {
        Variant v("8.5");
        v *= Variant("1.5");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 12.75);
    }
}

void testDivision()
{
    /* None / ? */
    {
        Variant v;
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v /= Variant(1);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 0);
    }
    {
        Variant v;
        v /= Variant(5.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.0);
    }
    {
        Variant v;
        v /= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v /= Variant("1.5");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.0);
    }
    /* Integer / ? */
    {
        Variant v(1);
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(7);
        v /= Variant(3);
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == 2);
    }
    {
        Variant v(3);
        v /= Variant(2.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 1.5);
    }
    {
        Variant v(2);
        v /= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(2);
        v /= Variant("4");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.5);
    }
    /* Double / ? */
    {
        Variant v(1.0);
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(3.0);
        v /= Variant(2);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 1.5);
    }
    {
        Variant v(3.0);
        v /= Variant(4.0);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 0.75);
    }
    {
        Variant v(3.0);
        v /= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(10);
        v /= Variant("4");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 2.5);
    }
    /* Vec3d / ? */
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant(2);
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(5.0, 5.5, 6.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant(4.0);
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(2.5, 2.75, 3.0));
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant(Vec3d(4.0, 5.0, 6.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant("2.0");
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(5.0, 5.5, 6.0));
    }
    /* String / ? */
    {
        Variant v("5");
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("5.5");
        v /= Variant(2);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 2.75);
    }
    {
        Variant v("4.5");
        v /= Variant(1.5);
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 3.0);
    }
    {
        Variant v("2.0");
        v /= Variant(Vec3d(1.0, 2.0, 3.0));
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("7.5");
        v /= Variant("1.5");
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == 5.0);
    }
}

void testDivisionByZero()
{
    /* None / ? */
    {
        Variant v;
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v /= Variant(0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v /= Variant(0.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v;
        v /= Variant("0");
        assert(v.type() == Variant::NONE);
    }
    /* Integer / ? */
    {
        Variant v(1);
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(7);
        v /= Variant(0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(3);
        v /= Variant(0.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(2);
        v /= Variant("0");
        assert(v.type() == Variant::NONE);
    }
    /* Double / ? */
    {
        Variant v(1.0);
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(3.0);
        v /= Variant(0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(3.0);
        v /= Variant(0.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(10);
        v /= Variant("0");
        assert(v.type() == Variant::NONE);
    }
    /* Vec3d / ? */
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant(0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant(0.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(Vec3d(10.0, 11.0, 12.0));
        v /= Variant("0");
        assert(v.type() == Variant::NONE);
    }
    /* String / ? */
    {
        Variant v("5");
        v /= Variant();
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("5.5");
        v /= Variant(0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("4.5");
        v /= Variant(0.0);
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v("7.5");
        v /= Variant("0");
        assert(v.type() == Variant::NONE);
    }
}

void testNegation()
{
    {
        Variant v;
        v = -v;
        assert(v.type() == Variant::NONE);
    }
    {
        Variant v(1);
        v = -v;
        assert(v.type() == Variant::INTEGER);
        assert(v.integerValue() == -1);
    }
    {
        Variant v(1.0);
        v = -v;
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -1.0);
    }
    {
        Variant v(Vec3d(1.0, 2.0, 3.0));
        v = -v;
        assert(v.type() == Variant::VEC3D);
        assert(v.vec3dValue() == Vec3d(-1.0, -2.0, -3.0));
    }
    {
        Variant v("1.5");
        v = -v;
        assert(v.type() == Variant::DOUBLE);
        assert(v.doubleValue() == -1.5);
    }
}

void testComparsion()
{
    /* None vs ? */
    {
        Variant v1;
        Variant v2;
        assert(v1 == v2);
        assert(!(v1 < v2));
        assert(!(v1 > v2));
        assert(!(v1 != v2));
    }
    {
        Variant v1;
        Variant v2(0);
        assert(v1 != v2);
        assert(v1 >= v2);
        assert(v1 <= v2);
        assert(!(v1 > v2));
        assert(!(v1 < v2));
        assert(!(v2 > v1));
        assert(!(v2 < v1));
    }
    {
        Variant v1;
        Variant v2(1);
        assert(v1 < v2);
        assert(v1 != v2);
    }
    {
        Variant v1;
        Variant v2(-1);
        assert(v1 > v2);
        assert(v1 != v2);
    }
    {
        Variant v1;
        Variant v2(0.0);
        assert(v1 != v2);
        assert(v1 >= v2);
        assert(v1 <= v2);
        assert(!(v1 > v2));
        assert(!(v1 < v2));
        assert(!(v2 > v1));
        assert(!(v2 < v1));
    }
    {
        Variant v1;
        Variant v2(1.0);
        assert(v1 < v2);
        assert(v1 != v2);
    }
    {
        Variant v1;
        Variant v2(-1.0);
        assert(v1 > v2);
        assert(v1 != v2);
    }
    {
        Variant v1;
        Variant v2("1.0");
        assert(v1 < v2);
        assert(v1 != v2);
    }
    {
        Variant v1;
        Variant v2("-1.0");
        assert(v1 > v2);
        assert(v1 != v2);
    }
    {
        Variant v1;
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 != v2);
    }
    /* Integer vs ? */
    {
        Variant v1(1);
        Variant v2(1);
        assert(v1 == v2);
    }
    {
        Variant v1(1);
        Variant v2(2);
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1(1);
        Variant v2;
        assert(v1 > v2);
    }
    {
        Variant v1(-1);
        Variant v2;
        assert(v1 < v2);
    }
    {
        Variant v1(1);
        Variant v2(1.0);
        assert(v1 == v2);
    }
    {
        Variant v1(1);
        Variant v2(2.0);
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1(1);
        Variant v2(-2.0);
        assert(v1 > v2);
        assert(v2 < v1);
    }
    {
        Variant v1(1);
        Variant v2("1.0");
        assert(v1 != v2);
        assert(v1 >= v2);
        assert(v1 <= v2);
    }
    {
        Variant v1(1);
        Variant v2("2.0");
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1(1);
        Variant v2("-2.0");
        assert(v1 > v2);
        assert(v2 < v1);
    }
    {
        Variant v1(0);
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 != v2);
        assert(v1 >= v2);
        assert(v1 <= v2);
    }
    {
        Variant v1(1);
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 > v2);
    }
    {
        Variant v1(-1);
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 < v2);
    }
    /* Double vs ? */
    {
        Variant v1(1.0);
        Variant v2(1);
        assert(v1 == v2);
    }
    {
        Variant v1(1.0);
        Variant v2(2);
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1(1.0);
        Variant v2;
        assert(v1 > v2);
    }
    {
        Variant v1(-1.0);
        Variant v2;
        assert(v1 < v2);
    }
    {
        Variant v1(1.0);
        Variant v2(1.0);
        assert(v1 == v2);
    }
    {
        Variant v1(1.0);
        Variant v2(2.0);
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1(1.0);
        Variant v2(-2.0);
        assert(v1 > v2);
        assert(v2 < v1);
    }
    {
        Variant v1(1.0);
        Variant v2("1.0");
        assert(v1 != v2);
        assert(v1 >= v2);
        assert(v1 <= v2);
    }
    {
        Variant v1(1.0);
        Variant v2("2.0");
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1(1.0);
        Variant v2("-2.0");
        assert(v1 > v2);
        assert(v2 < v1);
    }
    {
        Variant v1(0.0);
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1(1.0);
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 > v2);
    }
    {
        Variant v1(-1.0);
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 < v2);
    }
    /* String vs ? */
    {
        Variant v1("1.0");
        Variant v2(1);
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1("1.0");
        Variant v2(2);
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1("1.0");
        Variant v2;
        assert(v1 > v2);
    }
    {
        Variant v1("-1.0");
        Variant v2;
        assert(v1 < v2);
    }
    {
        Variant v1("1.0");
        Variant v2(1.0);
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1("1.0");
        Variant v2(2.0);
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1("1.0");
        Variant v2(-2.0);
        assert(v1 > v2);
        assert(v2 < v1);
    }
    {
        Variant v1("1.0");
        Variant v2("1.0");
        assert(v1 == v2);
    }
    {
        Variant v1("1.0");
        Variant v2("2.0");
        assert(v1 < v2);
        assert(v2 > v1);
    }
    {
        Variant v1("foo");
        Variant v2("bar");
        assert(v1 > v2);
        assert(v2 < v1);
    }
    {
        Variant v1("0.0");
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 != v2);
        assert(v1 >= v2);
        assert(v1 <= v2);
    }
    {
        Variant v1("1.0");
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 > v2);
    }
    {
        Variant v1("-1.0");
        Variant v2(Vec3d(0.0, 0.0, 0.0));
        assert(v1 < v2);
    }
    /* Vector vs ? */
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2;
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(0);
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(1);
        assert(v1 < v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(-1);
        assert(v1 > v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(0.0);
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(1.0);
        assert(v1 < v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(-1.0);
        assert(v1 > v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2("0");
        assert(v1 != v2);
        assert(v1 <= v2);
        assert(v1 >= v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2("1");
        assert(v1 < v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2("-1");
        assert(v1 > v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.4));
        Variant v2(Vec3d(1.2, 1.3, 1.4));
        assert(v1 == v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.5));
        Variant v2(Vec3d(1.2, 1.3, 1.4));
        assert(v1 > v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.3, 1.3));
        Variant v2(Vec3d(1.2, 1.3, 1.4));
        assert(v1 < v2);
    }
    {
        Variant v1(Vec3d(1.2, 1.4, 1.4));
        Variant v2(Vec3d(1.2, 1.3, 1.5));
        assert(v1 > v2);
    }
}

void testBool()
{
    {
        Variant v;
        assert(!v);
    }
    {
        Variant v(0);
        assert(!v);
    }
    {
        Variant v(1);
        assert(v);
    }
    {
        Variant v(0.0);
        assert(!v);
    }
    {
        Variant v(1.0);
        assert(v);
    }
    {
        Variant v(Vec3d(0.0, 0.0, 0.0));
        assert(v);
    }
    {
        Variant v(Vec3d(1.0, 1.0, 1.0));
        assert(v);
    }
    {
        Variant v("");
        assert(!v);
    }
    {
        Variant v("0");
        assert(v);
    }
}

void testMod()
{
    {
        Variant v1(7);
        Variant v2(4);
        assert(v1 % v2 == Variant(3));
    }
    {
        Variant v1(7);
        Variant v2(0);
        assert(v1 % v2 == Variant());
    }
}

int main(int argc, char **argv)
{
    testGetters();
    testCopying();
    testAssignment();
    testAddition();
    testSubstraction();
    testMultiplication();
    testDivision();
    testDivisionByZero();
    testNegation();
    testComparsion();
    testBool();
    testMod();
    return 0;
}
