#include <boost/python.hpp>
#include <metagam3d/Variant>
#include <cstdlib>
#include <sstream>
#include <cassert>

using namespace boost::python;

namespace mg {

void export_variant()
{
    scope in_Variant = class_<Variant>("Variant").
        def(init<short>()).
        def(init<int>()).
        def(init<long>()).
        def(init<float>()).
        def(init<double>()).
        def(init<osg::Vec3d>()).
        def(init<std::string>()).
        def("__str__", &Variant::stringValue).
        def(-self).
        def(self + self).
        def(self - self).
        def(self * self).
        def(self / self).
        def(self == self).
        def(self != self).
        def(self < self).
        def(self <= self).
        def(self > self).
        def(self >= self).
        def(self % self).
        add_property("type", &Variant::type).
        def("vec3dValue", &Variant::vec3dValue);

    enum_<Variant::Type>("VariantType");
    scope().attr("NONE") = Variant::NONE;
    scope().attr("INTEGER") = Variant::INTEGER;
    scope().attr("DOUBLE") = Variant::DOUBLE;
    scope().attr("VEC3D") = Variant::VEC3D;
    scope().attr("STRING") = Variant::STRING;
}

Variant::Variant():
    m_type(NONE)
{
}

Variant::Variant(short val):
    m_type(INTEGER),
    m_integerValue(val)
{
}

Variant::Variant(int val):
    m_type(INTEGER),
    m_integerValue(val)
{
}

Variant::Variant(long val):
    m_type(INTEGER),
    m_integerValue(val)
{
}

Variant::Variant(float val):
    m_type(DOUBLE),
    m_doubleValue(val)
{
}

Variant::Variant(double val):
    m_type(DOUBLE),
    m_doubleValue(val)
{
}

Variant::Variant(const osg::Vec3d &val):
    m_type(VEC3D),
    m_vec3dValue(val)
{
}

Variant::Variant(const std::string &val):
    m_type(STRING),
    m_stringValue(val)
{
}

Variant::Variant(const Variant &var)
{
    copy(var);
}

Variant::~Variant()
{
}

long Variant::integerValue() const
{
    switch (m_type) {
    case INTEGER:
        return m_integerValue;
    case DOUBLE:
        return (long)m_doubleValue;
    case STRING:
        return std::atol(m_stringValue.c_str());
    default:
        return 0;
    }
}

double Variant::doubleValue() const
{
    switch (m_type) {
    case INTEGER:
        return (double)m_integerValue;
    case DOUBLE:
        return m_doubleValue;
    case STRING:
        return std::atof(m_stringValue.c_str());
    default:
        return 0.0;
    }
}

osg::Vec3d Variant::vec3dValue() const
{
    if (m_type != VEC3D) {
        return osg::Vec3d(0.0, 0.0, 0.0);
    }
    return m_vec3dValue;
}

std::string Variant::stringValue() const
{
    switch (m_type) {
    case NONE:
        return std::string();
    case INTEGER:
        {
            std::ostringstream str;
            str << m_integerValue;
            return str.str();
        }
    case DOUBLE:
        {
            std::ostringstream str;
            str << m_doubleValue;
            return str.str();
        }
    case STRING:
        return m_stringValue;
    case VEC3D:
        {
            std::ostringstream str;
            str << '(' << m_vec3dValue.x() << ", " << m_vec3dValue.y() << ", " <<
                m_vec3dValue.z() << ')';
            return str.str();
        }
    }
}

Variant &Variant::operator=(const Variant &rhs)
{
    copy(rhs);
    return *this;
}

Variant &Variant::operator+=(const Variant &rhs)
{
    /* Strings are concatenated */
    if (m_type == STRING && rhs.m_type == STRING) {
        m_stringValue += rhs.stringValue();
        return *this;
    }

    /* Adding None does not affect the value */
    if (rhs.m_type == NONE)
        return *this;
    if (m_type == NONE) {
        copy(rhs);
        return *this;
    }

    /* Vectors can be added only to vectors */
    if (m_type == VEC3D) {
        if (rhs.m_type == VEC3D) {
            m_vec3dValue += rhs.m_vec3dValue;
        } else {
            m_type = NONE;
        }
        return *this;
    } else if (rhs.m_type == VEC3D) {
        m_type = NONE;
        return *this;
    }

    /* If both arguments are integer, perform integer addition */
    if (m_type == INTEGER && rhs.m_type == INTEGER) {
        m_integerValue += rhs.integerValue();
        return *this;
    }

    /* All other combinations interpret as double addition */
    m_doubleValue = doubleValue() + rhs.doubleValue();
    m_type = DOUBLE;
    return *this;
}

Variant &Variant::operator-=(const Variant &rhs)
{
    /* Substracting None does not affect the value */
    if (rhs.m_type == NONE)
        return *this;

    /* Vectors are substracted only from vectors */
    if (m_type == VEC3D) {
        if (rhs.m_type == VEC3D) {
            m_vec3dValue -= rhs.m_vec3dValue;
        } else {
            m_type = NONE;
        }
        return *this;
    } else if (rhs.m_type == VEC3D) {
        if (m_type == NONE) {
            m_vec3dValue = -rhs.m_vec3dValue;
            m_type = VEC3D;
        } else {
            m_type = NONE;
        }
        return *this;
    }

    /* If both arguments are integer, perform integer substraction
     * (None is treated as 0) */
    if ((m_type == INTEGER || m_type == NONE) && rhs.m_type == INTEGER) {
        m_integerValue = integerValue() - rhs.integerValue();
        m_type = INTEGER;
        return *this;
    }

    /* All other combinations interpret as double substraction */
    m_doubleValue = doubleValue() - rhs.doubleValue();
    m_type = DOUBLE;
    return *this;
}

Variant &Variant::operator*=(const Variant &rhs)
{
    /* Vectors may be multiplied by numbers only */
    if (m_type == VEC3D) {
        m_vec3dValue *= rhs.doubleValue();
        return *this;
    }
    if (rhs.m_type == VEC3D) {
        m_vec3dValue = rhs.m_vec3dValue * doubleValue();
        m_type = VEC3D;
        return *this;
    }

    /* If both arguments are integer, perform integer multiplication */
    if ((m_type == INTEGER || m_type == NONE) &&
            (rhs.m_type == INTEGER || rhs.m_type == NONE)) {
        m_integerValue = integerValue() * rhs.integerValue();
        m_type = INTEGER;
        return *this;
    }

    /* All other operations are performed on doubles */
    m_doubleValue = doubleValue() * rhs.doubleValue();
    m_type = DOUBLE;
    return *this;

}

Variant &Variant::operator/=(const Variant &rhs)
{
    /* Vectors may be divided by numbers only */
    if (m_type == VEC3D) {
        double val = rhs.doubleValue();
        if (val == 0.0) {
            m_type = NONE;
        } else {
            m_vec3dValue /= val;
        }
        return *this;
    }

    /* If both arguments are integer, perform integer division */
    if ((m_type == INTEGER || m_type == NONE) &&
            (rhs.m_type == INTEGER || rhs.m_type == NONE)) {
        long val = rhs.integerValue();
        if (val == 0) {
            m_type = NONE;
        } else {
            m_integerValue = integerValue() / val;
            m_type = INTEGER;
        }
        return *this;
    }

    /* All other combinatinos are treated as double */
    double val = rhs.doubleValue();
    if (val == 0.0) {
        m_type = NONE;
    } else {
        m_doubleValue = doubleValue() / val;
        m_type = DOUBLE;
    }
    return *this;
}

Variant &Variant::operator%=(const Variant &rhs)
{
    long v1 = integerValue();
    long v2 = rhs.integerValue();

    if (v2 == 0) {
        m_type = NONE;
    } else {
        m_integerValue = v1 % v2;
        m_type = INTEGER;
    }
    return *this;
}

Variant Variant::operator+(const Variant &rhs) const
{
    Variant lhs(*this);
    lhs += rhs;
    return lhs;
}

Variant Variant::operator-(const Variant &rhs) const
{
    Variant lhs(*this);
    lhs -= rhs;
    return lhs;
}

Variant Variant::operator*(const Variant &rhs) const
{
    Variant lhs(*this);
    lhs *= rhs;
    return lhs;
}

Variant Variant::operator/(const Variant &rhs) const
{
    Variant lhs(*this);
    lhs /= rhs;
    return lhs;
}

Variant Variant::operator%(const Variant &rhs) const
{
    Variant lhs(*this);
    lhs %= rhs;
    return lhs;
}

Variant Variant::operator-() const
{
    Variant lhs;
    lhs -= *this;
    return lhs;
}

bool Variant::operator==(const Variant &rhs) const
{
    if (!compatibleType(rhs))
        return false;
    return compare(rhs) == 0;
}

bool Variant::operator!=(const Variant &rhs) const
{
    if (!compatibleType(rhs))
        return true;
    return compare(rhs) != 0;
}

bool Variant::operator>(const Variant &rhs) const
{
    return compare(rhs) > 0;
}

bool Variant::operator>=(const Variant &rhs) const
{
    return compare(rhs) >= 0;
}

bool Variant::operator<(const Variant &rhs) const
{
    return compare(rhs) < 0;
}

bool Variant::operator<=(const Variant &rhs) const
{
    return compare(rhs) <= 0;
}

void Variant::copy(const Variant &rhs)
{
    m_type = rhs.m_type;
    switch (m_type) {
    case NONE:
        break;
    case INTEGER:
        m_integerValue = rhs.m_integerValue;
        break;
    case DOUBLE:
        m_doubleValue = rhs.m_doubleValue;
        break;
    case VEC3D:
        m_vec3dValue = rhs.m_vec3dValue;
        break;
    case STRING:
        m_stringValue = rhs.m_stringValue;
        break;
    }
}

int Variant::compare(const Variant &var) const
{
    /* Strings are compared with special string rules */
    if (m_type == STRING && var.m_type == STRING)
        return m_stringValue.compare(var.m_stringValue);

    /* Vectors are compared with special vector rules */
    if (m_type == VEC3D && var.m_type == VEC3D) {
        if (m_vec3dValue < var.m_vec3dValue)
            return -1;
        if (var.m_vec3dValue < m_vec3dValue)
            return 1;
        return 0;
    }

    /* Integers (None is considered ad 0) are compared directly */
    if ((m_type == INTEGER || m_type == NONE) &&
            (var.m_type == INTEGER || var.m_type == NONE)) {
        int v1 = integerValue();
        int v2 = var.integerValue();
        if (v1 < v2)
            return -1;
        if (v1 > v2)
            return 1;
        return 0;
    }

    /* All other combinations is compared as double (None and vectors are considered
     * as 0.0) */
    double v1 = doubleValue();
    double v2 = var.doubleValue();
    if (v1 < v2)
        return -1;
    if (v1 > v2)
        return 1;
    return 0;
}

Variant::operator bool() const
{
    switch (m_type) {
    case NONE:
        return false;
    case INTEGER:
        return m_integerValue ? true : false;
    case DOUBLE:
        return (m_doubleValue != 0.0) ? true : false;
    case VEC3D:
        return true;
    case STRING:
        return !m_stringValue.empty();
    }
}

} // namespace mg
