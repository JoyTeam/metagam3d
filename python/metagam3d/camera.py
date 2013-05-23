from _metagam3d import _slideCamera
from concurrence import Channel
from metagam3d.channels import blocking

def slideCameraStart(interval, eye, lookat):
    return _slideCamera(Channel(), interval, eye, lookat)

def slideCamera(interval, eye, lookat):
    return blocking(_slideCamera, interval, eye, lookat)
