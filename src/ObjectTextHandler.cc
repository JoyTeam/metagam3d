#include <metagam3d/ObjectTextHandler>
#include <metagam3d/Engine>
#include <osgText/Text>

namespace mg {

ObjectTextHandler::ObjectTextHandler(Object *object, ParamId textParam,
        ParamId sizeParam, ParamId colorParam, osgText::Text *text):
    ObjectParamHandler(object),
    m_dirty(true),
    m_textParam(textParam),
    m_sizeParam(sizeParam),
    m_colorParam(colorParam),
    m_text(text)
{
    text->setDataVariance(osg::Object::DYNAMIC);
}

ObjectTextHandler::~ObjectTextHandler()
{
}

void ObjectTextHandler::setParam(ParamId paramid, const DynamicValue &val)
{
    if (paramid == m_textParam) {
        m_textVal = val;
        m_dirty = true;
    }
    if (paramid == m_sizeParam) {
        m_sizeVal = val;
        m_dirty = true;
    }
    if (paramid == m_colorParam) {
        m_colorVal = val;
        m_dirty = true;
    }
}

void ObjectTextHandler::update()
{
    if (!m_dirty)
        return;
    double t = m_engine->time();
    Variant var;
    m_textVal.evaluateAndForget(t, var);
    m_text->setText(var.stringValue());
    m_sizeVal.evaluateAndForget(t, var);
    if (var.type() != Variant::NONE) {
        m_text->setCharacterSize(var.doubleValue());
    }
    m_colorVal.evaluateAndForget(t, var);
    if (var.type() != Variant::NONE) {
        osg::Vec3d color = var.vec3dValue();
        m_text->setColor(osg::Vec4d(color.x(), color.y(), color.z(), 1.0));
    }
    m_dirty = m_textVal.dynamic() || m_sizeVal.dynamic() || m_colorVal.dynamic();
}

} // namespace mg
