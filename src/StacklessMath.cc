#include <boost/python.hpp>
#include <osg/Vec3d>
#include <osg/Quat>

using namespace boost::python;

namespace mg {

class Vec3 {
public:
    static double getx(osg::Vec3d &vec) { return vec.x(); }
    static double gety(osg::Vec3d &vec) { return vec.y(); }
    static double getz(osg::Vec3d &vec) { return vec.z(); }

    static void setx(osg::Vec3d &vec, double val) { vec.x() = val; }
    static void sety(osg::Vec3d &vec, double val) { vec.y() = val; }
    static void setz(osg::Vec3d &vec, double val) { vec.z() = val; }

    static void setxyz(osg::Vec3d& vec, double x, double y, double z)
    {
        vec.set(x, y, z);
    }
};

void export_math()
{
    /* Register math classes */
    class_<osg::Vec3d>("Vec3d").
        def(init<double, double, double>()).
        def(init<osg::Vec3d>()).
        add_property("x", Vec3::getx, Vec3::setx).
        add_property("y", Vec3::gety, Vec3::sety).
        add_property("z", Vec3::getz, Vec3::setz).
        def("length", &osg::Vec3d::length).
        def("normalize", &osg::Vec3d::normalize).
        def("set", Vec3::setxyz).
        def(-self).
        def(self + self).
        def(self - self).
        def(self * double()).
        def(self / double()).
        def(self == self).
        def(self != self).
        def(self < self).
        def("__getitem__", (double& (osg::Vec3d::*)(int)) &osg::Vec3d::operator[],
                return_value_policy<copy_non_const_reference>()).
        def("cross", &osg::Vec3d::operator^);

    class_<osg::Quat>("Quat").
        def(init<>()).
        def(init<double, double, double, double>()).
        def(init<double, osg::Vec3d>()).
        def(init<double, osg::Vec3d, double, osg::Vec3d, double, osg::Vec3d>());
}

} // namespace mg
