#include <metagam3d/Object>
#include <metagam3d/ObjectParamHandler>
#include <osg/NodeCallback>
#include <osg/Node>
#include <osg/Group>

namespace mg {

class ObjectUpdater: public osg::NodeCallback {
public:
    ObjectUpdater(mg::Object *obj):
        m_obj(obj)
    {
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        m_obj->update();
        traverse(node, nv);
    }

private:
    osg::ref_ptr<mg::Object> m_obj;
};

Object::Object(Engine *engine, ObjectId id, osg::Node *node):
    m_engine(engine),
    m_id(id),
    m_node(node)
{
    m_updater = new ObjectUpdater(this);
    m_node->setUpdateCallback(m_updater);
    m_node->setDataVariance(osg::Object::DYNAMIC);
}

Object::~Object()
{
}

void Object::destroy()
{
    osg::Node::ParentList parents = m_node->getParents();
    for (int i = parents.size() - 1; i >= 0; i--)
        parents[i]->removeChild(m_node);
    m_handlers.clear();
    m_node->removeUpdateCallback(m_updater);
}

void Object::addParamHandler(ObjectParamHandler *handler)
{
    m_handlers.push_back(handler);
}

void Object::setParam(ParamId paramid, const DynamicValue &val)
{
    ObjectParamHandlers::iterator it = m_handlers.begin();
    while (it != m_handlers.end()) {
        (*it)->setParam(paramid, val);
        ++it;
    }
}

void Object::update()
{
    ObjectParamHandlers::iterator it = m_handlers.begin();
    while (it != m_handlers.end()) {
        (*it)->update();
        ++it;
    }
}

} // namespace mg
