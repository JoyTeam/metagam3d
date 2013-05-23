#include <metagam3d/StacklessMailbox>
#include <metagam3d/StacklessMessage>
#include <metagam3d/errors>
#include <unistd.h>
#include <fcntl.h>

namespace mg {

StacklessMailbox::StacklessMailbox()
{
    if (pipe(fd) == -1)
        throw os_error("Create Stackless pipe");
    if (fcntl(fd[0], F_SETFL, O_NONBLOCK) == -1)
        throw os_error("Nonblock for Stackless pipe");
    if (fcntl(fd[1], F_SETFL, O_NONBLOCK) == -1)
        throw os_error("Nonblock for Stackless pipe");
}

StacklessMailbox::~StacklessMailbox()
{
    if (close(fd[0]) == -1)
        throw os_error("Close Stackless pipe");
    if (close(fd[1]) == -1)
        throw os_error("Close Stackless pipe");
}

int StacklessMailbox::readFD()
{
    return fd[0];
}

void StacklessMailbox::send(StacklessMessage *msg)
{
    mutex.lock();
    queue.push_back(msg);
    mutex.unlock();
    wakeupSend();
}

void StacklessMailbox::process()
{
    wakeupAck();
    mutex.lock();
    std::list<StacklessMessage*>::iterator ent = queue.begin();
    while (ent != queue.end()) {
        StacklessMessage *msg = *ent;
        mutex.unlock();
        msg->deliver();
        delete msg;
        mutex.lock();
        ent = queue.erase(ent);
    }
    mutex.unlock();
}

void StacklessMailbox::wakeupSend()
{
    char buf = 0;
    write(fd[1], &buf, 1);
}

void StacklessMailbox::wakeupAck()
{
    char buffer[1024];
    read(fd[0], buffer, 1024);
}

StacklessMailbox *StacklessMailbox::instance()
{
    static StacklessMailbox inst;
    return &inst;
}

} // namespace mg
