#include <metagam3d/ObjectMaterialHandler>
#include <metagam3d/Engine>
#include <osg/Material>

namespace mg {

ObjectMaterialHandler::ObjectMaterialHandler(Object *object, ParamId ambientParam,
        ParamId diffuseParam, ParamId specularParam, ParamId emissionParam,
        ParamId shininessParam, osg::Material *material):
    ObjectParamHandler(object),
    m_dirty(true),
    m_ambientParam(ambientParam),
    m_diffuseParam(diffuseParam),
    m_specularParam(specularParam),
    m_emissionParam(emissionParam),
    m_shininessParam(shininessParam),
    m_material(material)
{
    material->setDataVariance(osg::Object::DYNAMIC);
}

ObjectMaterialHandler::~ObjectMaterialHandler()
{
}

void ObjectMaterialHandler::setParam(ParamId paramid, const DynamicValue &val)
{
    if (paramid == m_ambientParam) {
        m_ambient = val;
        m_dirty = true;
    } else if (paramid == m_diffuseParam) {
        m_diffuse = val;
        m_dirty = true;
    } else if (paramid == m_specularParam) {
        m_specular = val;
        m_dirty = true;
    } else if (paramid == m_emissionParam) {
        m_emission = val;
        m_dirty = true;
    } else if (paramid == m_shininessParam) {
        m_shininess = val;
        m_dirty = true;
    }
}

void ObjectMaterialHandler::update()
{
    if (!m_dirty)
        return;

    double t = m_engine->time();
    Variant var;

    m_ambient.evaluateAndForget(t, var);
    if (var.type() == Variant::VEC3D) {
        osg::Vec3d color = var.vec3dValue();
        m_material->setAmbient(osg::Material::FRONT_AND_BACK,
                osg::Vec4(color.x(), color.y(), color.z(), 1.0));
    }

    m_diffuse.evaluateAndForget(t, var);
    if (var.type() == Variant::VEC3D) {
        osg::Vec3d color = var.vec3dValue();
        m_material->setDiffuse(osg::Material::FRONT_AND_BACK,
                osg::Vec4(color.x(), color.y(), color.z(), 1.0));
    }

    m_specular.evaluateAndForget(t, var);
    if (var.type() == Variant::VEC3D) {
        osg::Vec3d color = var.vec3dValue();
        m_material->setSpecular(osg::Material::FRONT_AND_BACK,
                osg::Vec4(color.x(), color.y(), color.z(), 1.0));
    }

    m_emission.evaluateAndForget(t, var);
    if (var.type() == Variant::VEC3D) {
        osg::Vec3d color = var.vec3dValue();
        m_material->setEmission(osg::Material::FRONT_AND_BACK,
                osg::Vec4(color.x(), color.y(), color.z(), 1.0));
    }

    m_shininess.evaluateAndForget(t, var);
    if (var.type() != Variant::NONE) {
        m_material->setShininess(osg::Material::FRONT_AND_BACK,
                var.doubleValue());
    }

    m_dirty = m_ambient.dynamic() || m_diffuse.dynamic() ||
        m_specular.dynamic() || m_emission.dynamic() ||
        m_shininess.dynamic();
}

} // namespace mg
