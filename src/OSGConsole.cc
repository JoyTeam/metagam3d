#include <metagam3d/OSGConsole>
#include <cassert>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PolygonMode>
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgText/Font>
#include <osgText/Text>

namespace mg {

const double FRAME_WEIGHT = 0.15;

OSGConsole::OSGConsole(size_t cols, size_t rows, double fontSize):
    m_maxCols(cols),
    m_maxRows(rows),
    m_rows(0),
    m_fontSize(fontSize)
{
    m_font = osgText::readFontFile("fonts/FreeMono.ttf");
    osg::ref_ptr<osgText::Text> text = new osgText::Text;
    text->setPosition(osg::Vec3(0.0, 0.0, 0.0));
    text->setAxisAlignment(osgText::Text::XZ_PLANE);
    text->setAlignment(osgText::Text::LEFT_TOP);
    text->setCharacterSize(m_fontSize);
    text->setText("#");
    osg::BoundingBox bb = text->computeBound();
    m_charWidth = fontSize * 0.6;
    m_charHeight = bb.zMax() - bb.zMin();
    setDataVariance(osg::Object::DYNAMIC);

    /* Vertices */
    osg::Vec3Array *vertices = new osg::Vec3Array();
    vertices->push_back(osg::Vec3(-m_charWidth, 0.01, m_charHeight));
    vertices->push_back(osg::Vec3((cols + 1) * m_charWidth, 0.01, m_charHeight));
    vertices->push_back(osg::Vec3((cols + 1) * m_charWidth, 0.01, -((int)rows + 0.5) * m_charHeight));
    vertices->push_back(osg::Vec3(-m_charWidth, 0.01, -((int)rows + 0.5) * m_charHeight));

    /* Colors */
    osg::Vec4Array *backgroundColors = new osg::Vec4Array;
    backgroundColors->push_back(osg::Vec4(0.0, 0.0, 0.0, 0.7));

    /* Normals */
    osg::Vec3Array *normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0, -1.0, 0.0));

    /* Create console background */
    osg::Geode *background = new osg::Geode();
    osg::Geometry *geom = new osg::Geometry();
    geom->setVertexArray(vertices);
    geom->setColorArray(backgroundColors);
    geom->setColorBinding(osg::Geometry::BIND_OVERALL);
    geom->setNormalArray(normals);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
    osg::StateSet *stateset = geom->getOrCreateStateSet();
    stateset->setMode(GL_BLEND, osg::StateAttribute::ON);
    background->addDrawable(geom);
    addChild(background);

    /* Create console frame */
    osg::Geode *frame = new osg::Geode;
    frame->addDrawable(
        new osg::ShapeDrawable(
            new osg::Cylinder(
                osg::Vec3(-m_charWidth, 0.0f, m_charHeight * (rows * -0.5 + 0.25)),
                m_charWidth * FRAME_WEIGHT,
                (rows + 1.5) * m_charHeight
            )
        )
    );
    frame->addDrawable(
        new osg::ShapeDrawable(
            new osg::Cylinder(
                osg::Vec3(m_charWidth * (cols + 1), 0.0f, m_charHeight * (rows * -0.5 + 0.25)),
                m_charWidth * FRAME_WEIGHT,
                (rows + 1.5) * m_charHeight
            )
        )
    );
    osg::Cylinder *cyl = new osg::Cylinder(
        osg::Vec3(m_charWidth * cols * 0.5, 0.0f, m_charHeight * 1.0),
        m_charWidth * FRAME_WEIGHT,
        (cols + 2) * m_charWidth
    );
    cyl->setRotation(osg::Quat(M_PI / 2, osg::Vec3d(0.0, 1.0, 0.0)));
    frame->addDrawable(new osg::ShapeDrawable(cyl));
    cyl = new osg::Cylinder(
        osg::Vec3(m_charWidth * cols * 0.5, 0.0f, m_charHeight * (-1.0 * rows - 0.5)),
        m_charWidth * FRAME_WEIGHT,
        (cols + 2) * m_charWidth
    );
    cyl->setRotation(osg::Quat(M_PI / 2, osg::Vec3d(0.0, 1.0, 0.0)));
    frame->addDrawable(new osg::ShapeDrawable(cyl));
    /* Frame corners */
    frame->addDrawable(
        new osg::ShapeDrawable(
            new osg::Sphere(
                osg::Vec3(m_charWidth * -1.0, 0.0f, m_charHeight * (rows * -1.0 - 0.5)),
                m_charWidth * FRAME_WEIGHT
            )
        )
    );
    frame->addDrawable(
        new osg::ShapeDrawable(
            new osg::Sphere(
                osg::Vec3(m_charWidth * (cols + 1), 0.0f, m_charHeight * (rows * -1.0 - 0.5)),
                m_charWidth * FRAME_WEIGHT
            )
        )
    );
    frame->addDrawable(
        new osg::ShapeDrawable(
            new osg::Sphere(
                osg::Vec3(m_charWidth * -1.0, 0.0f, m_charHeight),
                m_charWidth * FRAME_WEIGHT
            )
        )
    );
    frame->addDrawable(
        new osg::ShapeDrawable(
            new osg::Sphere(
                osg::Vec3(m_charWidth * (cols + 1), 0.0f, m_charHeight),
                m_charWidth * FRAME_WEIGHT
            )
        )
    );
    stateset = frame->getOrCreateStateSet();
    osg::Material *material = new osg::Material;
    material->setAmbient(osg::Material::FRONT_AND_BACK, osg::Vec4(0.1, 0.1, 0.1, 1.0));
    material->setDiffuse(osg::Material::FRONT_AND_BACK, osg::Vec4(0.1, 0.1, 0.1, 1.0));
    material->setSpecular(osg::Material::FRONT_AND_BACK, osg::Vec4(0.5, 0.5, 0.5, 1.0));
    material->setShininess(osg::Material::FRONT_AND_BACK, 30.0);
    stateset->setAttribute(material);
    addChild(frame);
}

OSGConsole::~OSGConsole()
{
}

void OSGConsole::print(const ConsoleObject::Line &line)
{
    osg::Geode *geode = new osg::Geode;
    ConsoleObject::Line::const_iterator it = line.begin();
    size_t cols = 0;
    while (it != line.end()) {
        osgText::Text *text = new osgText::Text();
        text->setFont(m_font);
        text->setPosition(m_cursor);
        int remaining = m_maxCols - cols;
        if ((int)it->str.size() <= remaining) {
            text->setText(it->str);
            cols += it->str.size();
        } else {
            std::string s(it->str, 0, remaining);
            text->setText(s);
            cols = m_maxCols;
        }
        text->setColor(it->color);
        text->setCharacterSize(m_fontSize);
        text->setAxisAlignment(osgText::Text::XZ_PLANE);
        text->setAlignment(osgText::Text::LEFT_TOP);
        text->setDataVariance(osg::Object::DYNAMIC);
        m_cursor.x() += m_charWidth * it->str.size();
        geode->addDrawable(text);
        if (cols >= m_maxCols)
            break;
        ++it;
    }
    /* Add line to the scene */
    addChild(geode);
    m_lines.push_back(geode);
    /* Update counters */
    m_cursor.z() -= m_charHeight;
    m_cursor.x() = 0.0;
    m_rows++;
    while (m_rows > m_maxRows) {
        Lines::iterator it = m_lines.begin();
        /* Remove first line */
        assert(it != m_lines.end());
        removeChild(*it);
        it = m_lines.erase(it);
        /* Shift other lines up */
        osg::Vec3 shift(0.0, 0.0, m_charHeight);
        while (it != m_lines.end()) {
            osg::Geode *gd = *it;
            for (unsigned int i = 0; i < gd->getNumDrawables(); i++) {
                osgText::Text *text = dynamic_cast<osgText::Text*>(gd->getDrawable(i));
                if (text) {
                    text->setPosition(text->getPosition() + shift);
                }
            }
            ++it;
        }
        /* Update counters */
        m_rows--;
        m_cursor += shift;
    }
}

} // namespace mg
