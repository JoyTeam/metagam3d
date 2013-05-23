#include <metagam3d/ObjectMotionHandler>
#include <metagam3d/Object>
#include <metagam3d/Engine>
#include <osg/MatrixTransform>
#include <osg/Matrixd>

namespace mg {

ObjectMotionHandler::ObjectMotionHandler(Object *object, ParamId translateParam,
        ParamId rotateParam, ParamId scaleParam, osg::MatrixTransform *trans):
    ObjectParamHandler(object),
    m_dirty(true),
    m_trans(trans),
    m_translateParam(translateParam),
    m_rotateParam(rotateParam),
    m_scaleParam(scaleParam),
    m_translate(osg::Vec3d(0.0, 0.0, 0.0)),
    m_rotate(osg::Vec3d(0.0, 0.0, 0.0)),
    m_scale(osg::Vec3d(1.0, 1.0, 1.0))
{
}

ObjectMotionHandler::~ObjectMotionHandler()
{
}

void ObjectMotionHandler::setParam(ParamId paramid, const DynamicValue &val)
{
    if (paramid == m_translateParam) {
        m_translate = val;
        m_dirty = true;
    } else if (paramid == m_rotateParam) {
        m_rotate = val;
        m_dirty = true;
    } else if (paramid == m_scaleParam) {
        m_scale = val;
        m_dirty = true;
    }
}

void ObjectMotionHandler::update()
{
    if (!m_trans)
        return;
    if (!m_dirty)
        return;

    /* Initial transformation (identity matrix) */
    osg::Matrixd mat;

    /* Current model time */
    double t = m_engine->time();
    Variant var;

    /* Scale */
    m_scale.evaluateAndForget(t, var);
    if (var.type() == Variant::INTEGER || var.type() == Variant::DOUBLE) {
        double val = var.doubleValue();
        mat.postMultScale(osg::Vec3d(val, val, val));
    } else if (var.type() == Variant::VEC3D) {
        mat.postMultScale(var.vec3dValue());
    }

    /* Rotate */
    m_rotate.evaluateAndForget(t, var);
    if (var.type() == Variant::INTEGER || var.type() == Variant::DOUBLE) {
        double angle = var.doubleValue();
        mat.postMult(osg::Matrixd::rotate(angle, 0.0, 0.0, 1.0));
    } else if (var.type() == Variant::VEC3D) {
        osg::Vec3d vec = var.vec3dValue();
        mat.postMult(osg::Matrixd::rotate(vec.x(), 1.0, 0.0, 0.0));
        mat.postMult(osg::Matrixd::rotate(vec.y(), 0.0, 1.0, 0.0));
        mat.postMult(osg::Matrixd::rotate(vec.z(), 0.0, 0.0, 1.0));
    }

    /* Translate */
    m_translate.evaluateAndForget(t, var);
    if (var.type() == Variant::VEC3D) {
        mat.postMultTranslate(var.vec3dValue());
    }

    /* Apply the transformation */
    m_trans->setMatrix(mat);

    /* If any parameter contains dynamic expression, don't clear
     * dirty flag */
    m_dirty = m_scale.dynamic() || m_translate.dynamic() || m_rotate.dynamic();
}

} // namespace mg
