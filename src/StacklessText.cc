#include <boost/python.hpp>
#include <metagam3d/types>
#include <metagam3d/Engine>
#include <metagam3d/Object>
#include <metagam3d/ObjectManager>
#include <metagam3d/ObjectMotionHandler>
#include <metagam3d/ObjectTextHandler>
#include <metagam3d/OSGMessage>
#include <osg/MatrixTransform>
#include <osgText/Text>
#include <osgText/Font>
#include <osg/Geode>
#include <osg/Vec4d>
#include <string>

using namespace boost;

namespace mg {

/* Get new object identifier */
static ObjectId getObjectId()
{
    static ObjectId id = OBJECT_ID_TEXT;
    return ++id;
}

class CreateTextObjectMessage: public OSGMessage {
public:
    CreateTextObjectMessage(ObjectId id, ObjectId parentId,
            osgText::Text::AxisAlignment axisAlignment,
            osgText::Text::AlignmentType alignment,
            osgText::Font *font):
        m_id(id),
        m_parentId(parentId),
        m_axisAlignment(axisAlignment),
        m_alignment(alignment),
        m_font(font)
    {
    }

    virtual void deliver(Engine *engine)
    {
        /* Create text */
        osgText::Text *text = new osgText::Text();
        text->setFont(m_font);
        text->setColor(osg::Vec4d(0.0, 0.0, 0.0, 1.0));
        text->setCharacterSize(1.0);
        text->setAxisAlignment(m_axisAlignment);
        text->setAlignment(m_alignment);

        osg::Geode *geode = new osg::Geode();
        geode->addDrawable(text);

        /* Create transformation matrix */
        osg::MatrixTransform *trans = new osg::MatrixTransform();
        trans->addChild(geode);

        osg::Group *parentNode = NULL;
        if (m_parentId) {
            /* Add object to the scene as a subobject */
            Object *obj = engine->objectManager()->find(m_parentId);
            if (obj) {
                parentNode = dynamic_cast<osg::Group*>(obj->node());
            }
        }
        if (!parentNode) {
            /* Add object to the scene as a top level object */
            parentNode = engine->scene();
        }
        parentNode->addChild(trans);

        /* Create and register stackless object */
        Object *obj = new mg::Object(engine, m_id, trans);
        obj->addParamHandler(new ObjectMotionHandler(obj, 1, 2, 3, trans));
        obj->addParamHandler(new ObjectTextHandler(obj, 4, 5, 6, text));
        engine->objectManager()->add(obj);
    }

private:
    ObjectId m_id;
    ObjectId m_parentId;
    osgText::Text::AxisAlignment m_axisAlignment;
    osgText::Text::AlignmentType m_alignment;
    osg::ref_ptr<osgText::Font> m_font;
};

/* Stackless command to create a text object */
static python::object createText(ObjectId parentId,
        osgText::Text::AxisAlignment axisAlignment,
        osgText::Text::AlignmentType alignment)
{
    static osgText::Font *font = osgText::readFontFile("fonts/arial.ttf");

    /* Send message to OSG thread to create the object */
    ObjectId id = getObjectId();
    CreateTextObjectMessage *msg = new CreateTextObjectMessage(id, parentId,
            axisAlignment, alignment, font);
    msg->send();
    
    return python::object(id);
}

/* Register Stackless objects */
void export_text()
{
    python::def("_createText", &createText);

    {
        python::scope in_AxisAlignment = python::class_<osg::Object, noncopyable>("AxisAlignment", python::no_init);
        python::enum_<osgText::Text::AxisAlignment>("AxisAlignment");
        python::scope().attr("XY_PLANE") = osgText::Text::XY_PLANE;
        python::scope().attr("REVERSED_XY_PLANE") = osgText::Text::REVERSED_XY_PLANE;
        python::scope().attr("XZ_PLANE") = osgText::Text::XZ_PLANE;
        python::scope().attr("REVERSED_XZ_PLANE") = osgText::Text::REVERSED_XZ_PLANE;
        python::scope().attr("YZ_PLANE") = osgText::Text::YZ_PLANE;
        python::scope().attr("REVERSED_YZ_PLANE") = osgText::Text::REVERSED_YZ_PLANE;
        python::scope().attr("SCREEN") = osgText::Text::SCREEN;
    }

    {
        python::scope in_AlignmentType = python::class_<osg::Object, noncopyable>("AlignmentType", python::no_init);
        python::enum_<osgText::Text::AlignmentType>("AlignmentType");
        python::scope().attr("LEFT_TOP") = osgText::Text::LEFT_TOP;
        python::scope().attr("LEFT_CENTER") = osgText::Text::LEFT_CENTER;
        python::scope().attr("LEFT_BOTTOM") = osgText::Text::LEFT_BOTTOM;
        python::scope().attr("CENTER_TOP") = osgText::Text::CENTER_TOP;
        python::scope().attr("CENTER_CENTER") = osgText::Text::CENTER_CENTER;
        python::scope().attr("CENTER_BOTTOM") = osgText::Text::CENTER_BOTTOM;
        python::scope().attr("RIGHT_TOP") = osgText::Text::RIGHT_TOP;
        python::scope().attr("RIGHT_CENTER") = osgText::Text::RIGHT_CENTER;
        python::scope().attr("RIGHT_BOTTOM") = osgText::Text::RIGHT_BOTTOM;
        python::scope().attr("LEFT_BASE_LINE") = osgText::Text::LEFT_BASE_LINE;
        python::scope().attr("CENTER_BASE_LINE") = osgText::Text::CENTER_BASE_LINE;
        python::scope().attr("RIGHT_BASE_LINE") = osgText::Text::RIGHT_BASE_LINE;
        python::scope().attr("LEFT_BOTTOM_BASE_LINE") = osgText::Text::LEFT_BOTTOM_BASE_LINE;
        python::scope().attr("CENTER_BOTTOM_BASE_LINE") = osgText::Text::CENTER_BOTTOM_BASE_LINE;
        python::scope().attr("RIGHT_BOTTOM_BASE_LINE") = osgText::Text::RIGHT_BOTTOM_BASE_LINE;
        python::scope().attr("BASE_LINE") = osgText::Text::BASE_LINE;
    }
}

} // namespace mg
