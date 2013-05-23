#include <metagam3d/StacklessChannel>
#include <metagam3d/StacklessMessage>

using namespace boost::python;

namespace mg {

/* Message sent to python side to send specific message to
 * the channel */
class StacklessChannelMessage: public StacklessMessage {
public:
    StacklessChannelMessage(StacklessChannel *channel):
        m_channel(channel)
    {
    }

    void deliver()
    {
        m_channel->deliver();
        delete m_channel;
    }

private:
    StacklessChannel *m_channel;
};

StacklessChannel::StacklessChannel(object channel):
    m_channel(channel),
    m_sent(false)
{
}

StacklessChannel::~StacklessChannel()
{
}

void StacklessChannel::signal()
{
    if (m_sent)
        return;
    send();
}

void StacklessChannel::signal(const StacklessChannelValue &val)
{
    if (m_sent)
        return;
    m_value = val;
    send();
}

void StacklessChannel::send()
{
    m_sent = true;
    (new StacklessChannelMessage(this))->send();
}

void StacklessChannel::deliver()
{
    object val = m_value.getPythonObject();
    while (extract<int>(m_channel.attr("balance")) < 0) {
        m_channel.attr("send")(val);
    }
}

} // namespace mg
