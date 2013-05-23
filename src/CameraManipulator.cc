#include <metagam3d/CameraManipulator>
#include <metagam3d/StacklessChannel>
#include <metagam3d/OSGMessage>
#include <metagam3d/Engine>
#include <metagam3d/Variant>
#include <osgGA/CameraManipulator>

using namespace boost;

namespace mg {

CameraManipulator::CameraManipulator(Engine *engine):
    m_engine(engine),
    m_channel(NULL),
    m_moving(false)
{
    slide(0.0, osg::Vec3d(0.0, 1.0, 0.0), osg::Vec3d(0.0, 0.0, 0.0));
}

/* Position at specified time */
void CameraManipulator::getDistance(double &distance, double time) const
{
    if (m_moving) {
        double ratio = (time - m_startTime) / (m_stopTime - m_startTime);
        if (ratio < 0.0)
            ratio = 0.0;
        else if (ratio > 1.0)
            ratio = 1.0;
        distance = m_startDistance * (1.0 - ratio) + m_stopDistance * ratio;
    } else {
        distance = m_stopDistance;
    }
}

/* Rotation at specified time */
void CameraManipulator::getRotation(osg::Quat &rotation, double time) const
{
    if (m_moving) {
        double ratio = (time - m_startTime) / (m_stopTime - m_startTime);
        if (ratio < 0.0)
            ratio = 0.0;
        else if (ratio > 1.0)
            ratio = 1.0;
        rotation.slerp(ratio, m_startRotation, m_stopRotation);
    } else {
        rotation = m_stopRotation;
    }
}

/* LookAt at specified time */
void CameraManipulator::getLookAt(osg::Vec3d &lookAt, double time) const
{
    if (m_moving) {
        double ratio = (time - m_startTime) / (m_stopTime - m_startTime);
        if (ratio < 0.0)
            ratio = 0.0;
        else if (ratio > 1.0)
            ratio = 1.0;
        lookAt = m_startLookAt * (1.0 - ratio) + m_stopLookAt * ratio;
    } else {
        lookAt = m_stopLookAt;
    }
}

/* Set notification channel */
void CameraManipulator::setChannel(StacklessChannel *channel)
{
    m_channel = channel;
}

/* Start sliding */
void CameraManipulator::slide(double interval, const osg::Vec3d &endPos, const osg::Vec3d &endLookAt)
{
    double time = m_engine->time();

    if (interval > 0.0) {
        /* Set starting position */
        getDistance(m_startDistance, time);
        getRotation(m_startRotation, time);
        getLookAt(m_startLookAt, time);
        m_startTime = time;
    }

    /* Set target position */
    osg::Vec3d stopDirection = endPos;
    stopDirection -= endLookAt;
    m_stopDistance = stopDirection.length();
    m_stopRotation.makeRotate(osg::Vec3d(1.0, 0.0, 0.0), stopDirection);
    m_stopLookAt = endLookAt;

    if (interval > 0.0) {
        m_moving = true;
        m_stopTime = time + interval;
    } else {
        m_moving = false;
        updateMatrix(time);
        motionCompleted();
    }
}

/* Handle GUI events (including "FRAME" event) */
bool CameraManipulator::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &us)
{
    switch (ea.getEventType()) {
    case osgGA::GUIEventAdapter::FRAME:
        if (m_moving) {
            double time = m_engine->time();
            updateMatrix(time);
            if (time >= m_stopTime) {
                m_moving = false;
                motionCompleted();
            }
        }
        return false;
    default:
        return false;
    }
}

/* Calculate camera matrix */
void CameraManipulator::updateMatrix(double time)
{
    osg::Vec3d lookAt;
    osg::Quat rotation;
    double distance;

    getLookAt(lookAt, time);
    getRotation(rotation, time);
    getDistance(distance, time);

    osg::Vec3d position = rotation * osg::Vec3d(distance, 0.0, 0.0) + lookAt;
    static osg::Vec3d up(0.0, 0.0, 1.0);
    m_matrix.makeLookAt(position, lookAt, up);
}

/* Notify script that motion has completed */
void CameraManipulator::motionCompleted()
{
    if (m_channel) {
        m_channel->signal();
        m_channel = NULL;
    }
}

/* Message notifying OSG program to start camera motion */
class CameraSlideMessage: public OSGMessage {
public:
    CameraSlideMessage(StacklessChannel *channel, double interval,
            const osg::Vec3d &position, const osg::Vec3d &lookAt):
        m_channel(channel),
        m_interval(interval),
        m_position(position),
        m_lookAt(lookAt)
    {
    }

    void deliver(Engine *engine)
    {
        CameraManipulator *manip = engine->cameraManipulator();
        manip->motionCompleted();
        manip->setChannel(m_channel);
        manip->slide(m_interval, m_position, m_lookAt);
    }

private:
    StacklessChannel *m_channel;
    double m_interval;
    osg::Vec3d m_position;
    osg::Vec3d m_lookAt;
};

/* Stackless command to slide the camera */
static void slideCamera(python::object channel, double interval,
        const Variant &position, const Variant &lookAt)
{
    StacklessChannel *ch = new StacklessChannel(channel);
    (new CameraSlideMessage(ch, interval, position.vec3dValue(), lookAt.vec3dValue()))->send();
}

/* Register Stackless objects */
void export_camera()
{
    python::def("_slideCamera", &slideCamera);
}

} // namespace mg
