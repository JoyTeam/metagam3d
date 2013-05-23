#include <metagam3d/ObjectVectorHandler>
#include <metagam3d/Object>
#include <metagam3d/Engine>
#include <osg/Geometry>

namespace mg {

ObjectVectorHandler::ObjectVectorHandler(Object *object, ParamId param,
        osg::Vec3Array *vectors, unsigned int index, osg::Geometry *geom):
    ObjectParamHandler(object),
    m_dirty(true),
    m_param(param),
    m_vectors(vectors),
    m_index(index),
    m_geom(geom)
{
    vectors->setDataVariance(osg::Object::DYNAMIC);
}

ObjectVectorHandler::~ObjectVectorHandler()
{
}

void ObjectVectorHandler::setParam(ParamId paramid, const DynamicValue &val)
{
    if (paramid == m_param) {
        m_value = val;
        m_dirty = true;
    }
}

void ObjectVectorHandler::update()
{
    if (!m_vectors || m_index >= m_vectors->size())
        return;
    if (!m_dirty)
        return;

    /* Current model time */
    double t = m_engine->time();
    Variant var;

    /* Parameter value */
    m_value.evaluateAndForget(t, var);
    if (var.type() == Variant::VEC3D) {
        (*m_vectors)[m_index] = var.vec3dValue();
    }

    /* Invalidate geometry cache */
    if (m_geom) {
        m_geom->dirtyDisplayList();
        m_geom->dirtyBound();
    }

    /* If the parameter contains dynamic expression, don't clear
     * dirty flag */
    m_dirty = m_value.dynamic();
}

} // namespace mg
