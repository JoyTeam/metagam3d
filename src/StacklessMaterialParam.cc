#include <boost/python.hpp>
#include <metagam3d/Engine>
#include <metagam3d/ObjectManager>
#include <metagam3d/Object>
#include <metagam3d/ObjectMaterialHandler>
#include <metagam3d/OSGMessage>
#include <osg/Material>
#include <osg/NodeVisitor>
#include <osg/Geode>
#include <string>

using namespace boost;

namespace mg {

class AssignMaterialMessage: public OSGMessage {
public:
    AssignMaterialMessage(ObjectId id, const std::string &materialName,
            ParamId ambient, ParamId diffuse, ParamId specular,
            ParamId emission, ParamId shininess):
        m_id(id),
        m_materialName(materialName),
        m_ambient(ambient),
        m_diffuse(diffuse),
        m_specular(specular),
        m_emission(emission),
        m_shininess(shininess)
    {
    }

    class MaterialSetterVisitor: public osg::NodeVisitor {
    public:
        MaterialSetterVisitor(mg::Object *object, ParamId ambient, ParamId diffuse,
                ParamId specular, ParamId emission, ParamId shininess,
                const std::string &materialName):
            osg::NodeVisitor(TRAVERSE_ALL_CHILDREN),
            m_object(object),
            m_ambient(ambient),
            m_diffuse(diffuse),
            m_specular(specular),
            m_emission(emission),
            m_shininess(shininess),
            m_materialName(materialName)
        {
        }

        void apply(osg::Geode &node)
        {
            osg::Geode::DrawableList drawables = node.getDrawableList();
            for (unsigned int i = 0; i < drawables.size(); i++) {
                osg::Drawable *drawable = drawables[i];
                osg::StateSet *stateset = drawable->getStateSet();
                if (!stateset)
                    continue;
                osg::Material *material = dynamic_cast<osg::Material*>(
                    stateset->getAttribute(osg::StateAttribute::MATERIAL)
                );
                if (!material)
                    continue;
                if (material->getName() != m_materialName)
                    continue;
                m_object->addParamHandler(new ObjectMaterialHandler(m_object,
                        m_ambient, m_diffuse, m_specular, m_emission,
                        m_shininess, material));
            }
            traverse(node);
        }

    protected:
        mg::Object *m_object;
        ParamId m_ambient;
        ParamId m_diffuse;
        ParamId m_specular;
        ParamId m_emission;
        ParamId m_shininess;
        std::string m_materialName;
    };

    virtual void deliver(Engine *engine)
    {
        /* Find object */
        Object *obj = engine->objectManager()->find(m_id);
        if (!obj)
            return;

        /* Assign parameters to requested material within the object */
        MaterialSetterVisitor mfinder(obj, m_ambient, m_diffuse,
                m_specular, m_emission, m_shininess, m_materialName);
        obj->node()->accept(mfinder);
    }

private:
    ObjectId m_id;
    std::string m_materialName;
    ParamId m_ambient;
    ParamId m_diffuse;
    ParamId m_specular;
    ParamId m_emission;
    ParamId m_shininess;
};

/* Stackless command to assign material parameters to the object */
static void assignMaterial(ObjectId objid, std::string materialName,
        ParamId ambient, ParamId diffuse, ParamId specular, ParamId emission,
        ParamId shininess)
{
    /* Send message to OSG thread to assign the material */
    AssignMaterialMessage *msg = new AssignMaterialMessage(objid, materialName,
            ambient, diffuse, specular, emission, shininess);
    msg->send();
}

/* Register Stackless objects */
void export_material_param()
{
    python::def("_assignMaterial", &assignMaterial);
}

} // namespace mg
