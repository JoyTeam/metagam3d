#include <metagam3d/OSGMessage>
#include <metagam3d/OSGMailbox>

namespace mg {

OSGMessage::OSGMessage()
{
}

OSGMessage::~OSGMessage()
{
}

void OSGMessage::send()
{
    OSGMailbox::instance()->send(this);
}

} // namespace mg
