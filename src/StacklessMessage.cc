#include <metagam3d/StacklessMessage>
#include <metagam3d/StacklessMailbox>

namespace mg {

StacklessMessage::StacklessMessage()
{
}

StacklessMessage::~StacklessMessage()
{
}

void StacklessMessage::send()
{
    StacklessMailbox::instance()->send(this);
}

} // namespace mg
