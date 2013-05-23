from _metagam3d import processQueue
from concurrence import Tasklet, FileDescriptorEvent, TimeoutError
from concurrence.event import EventError
import logging

def eventloop(fd):
    event = FileDescriptorEvent(fd, 'r')
    while True:
        try:
            event.wait()
        except EventError:
            return
        processQueue()

def init(fd):
    Tasklet.new(eventloop)(fd)

def _exec_file(filename):
    try:
        globs = {}
        execfile(filename, globs, globs)
    except Exception as e:
        logging.exception(e)

def exec_file(filename):
    Tasklet.new(_exec_file)(filename)
