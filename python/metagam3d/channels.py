from concurrence import Channel

def blocking(func, *args, **kwargs):
    ch = Channel()
    func(ch, *args, **kwargs)
    return ch.receive()
