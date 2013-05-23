import _metagam3d
from _metagam3d import AxisAlignment, AlignmentType
from metagam3d.channels import blocking
from metagam3d.scripts import m3d_expr
from concurrence import Tasklet

class LoadError(Exception):
    pass

class Object(_metagam3d.Object):
    def __init__(self, objid):
        _metagam3d.Object.__init__(self, objid)
        self._params = {}

    def param(self, paramid):
        "Get parameter object for given parameter id"
        try:
            return self._params[paramid]
        except KeyError:
            pass
        param = ObjectParam(self, paramid)
        self._params[paramid] = param
        return param

    def load(self, filename, flags=0):
        "Load and return new subobject from file"
        objid = _metagam3d._loadObject(filename, self.id, flags)
        if objid is None:
            raise LoadError("Error loading %s" % filename)
        return Object(objid)

    def createText(self, axisAlignment=AxisAlignment.XY_PLANE, alignment=AlignmentType.CENTER_CENTER):
        "Create text object"
        return Object(_metagam3d._createText(self.id, axisAlignment, alignment))

    def getParam(self, paramid, t):
        return self.param(paramid).getValue(t)

    def setParam(self, paramid, val):
        if type(val) is not _metagam3d.DynamicValue:
            if type(val) is not _metagam3d.Variant:
                val = _metagam3d.Variant(val)
            val = _metagam3d.DynamicValue(val)
        self.param(paramid).setValue(val)

    def setParam3(self, paramid, x, y, z):
        self.setParam(paramid, _metagam3d.Vec3d(x, y, z))

    def setParamExpr(self, paramid, expr, till=None):
        self.param(paramid).setValue(m3d_expr(expr, till))

    def assignMaterial(self, geodeName, ambient=0, diffuse=0, specular=0, emission=0, shininess=0):
        _metagam3d._assignMaterial(self.id, geodeName, ambient, diffuse, specular, emission, shininess)

    def createConsole(self, cols=80, rows=25, fontSize=1.0):
        return Console(_metagam3d._createConsole(self.id, cols, rows, fontSize))

    def createLine(self):
        return Object(_metagam3d._createLine(self.id))

    def destroyAfter(self, t):
        Tasklet.new(self._destroyAfter)(t)

    def _destroyAfter(self, t):
        Tasklet.sleep(t)
        self.destroy()

class Console(Object):
    def println(self, elements):
        line = _metagam3d.ConsoleLine()
        for el in elements:
             line.add(_metagam3d.ConsoleLineElement(el[0], el[1]))
        _metagam3d._printConsole(self.id, line)

class ObjectParam(_metagam3d.ObjectParam):
    def __init__(self, obj, paramid):
        _metagam3d.ObjectParam.__init__(self, obj.id, paramid)
        self._obj = obj

    @property
    def obj(self):
        return self._obj

def load(filename, flags=0):
    "Load root level object from file"
    objid = _metagam3d._loadObject(filename, 0, flags)
    if objid is None:
        raise LoadError("Error loading %s" % filename)
    return Object(objid)

def createText(axisAlignment=AxisAlignment.XY_PLANE, alignment=AlignmentType.CENTER_CENTER):
    "Create text object"
    return Object(_metagam3d._createText(0, axisAlignment, alignment))

def createConsole(cols=80, rows=25, fontSize=1.0):
    return Console(_metagam3d._createConsole(0, cols, rows, fontSize))

def createLine():
    return Object(_metagam3d._createLine(0))
