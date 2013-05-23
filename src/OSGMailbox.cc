#include <metagam3d/OSGMailbox>
#include <metagam3d/OSGMessage>
#include <osg/OperationThread>
#include <osgViewer/Viewer>

namespace mg {

OSGMailbox::OSGMailbox():
    m_engine(0)
{
}

OSGMailbox::~OSGMailbox()
{
}

void OSGMailbox::send(OSGMessage *msg)
{
    mutex.lock();
    queue.push_back(msg);
    mutex.unlock();
}

void OSGMailbox::process()
{
    mutex.lock();
    std::list<OSGMessage*>::iterator ent = queue.begin();
    while (ent != queue.end()) {
        OSGMessage *msg = *ent;
        mutex.unlock();
        msg->deliver(m_engine);
        delete msg;
        mutex.lock();
        ent = queue.erase(ent);
    }
    mutex.unlock();
}

void OSGMailbox::setEngine(Engine *engine)
{
    m_engine = engine;
}

class ProcessOSGMessages: public osg::Operation {
public:
    ProcessOSGMessages(OSGMailbox *mailbox):
        osg::Operation("ProcessOSGMessages", true),
        m_mailbox(mailbox)
    {
    }

    virtual void operator () (osg::Object *callingObject)
    {
        m_mailbox->process();
    }

private:
    OSGMailbox *m_mailbox;
};

void OSGMailbox::reg(osgViewer::Viewer *viewer)
{
    viewer->addUpdateOperation(new ProcessOSGMessages(this));
}

OSGMailbox *OSGMailbox::instance()
{
    static OSGMailbox inst;
    return &inst;
}

} // namespace mg

