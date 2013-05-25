from metagam3d.scripts import m3d_compile, getTime
from metagam3d.math import Vec3d
from metagam3d.objects import createLine

class GraphNode(object):
    def __init__(self, model):
        self.model = model
        self.edges = []

    def addEdge(self, line, index, offset):
        self.edges.append([line, index, offset])
        line.setParam(index, self.model.getParam(1, getTime()).vec3dValue() + offset)

    def slide(self, now, target, speed=1.0):
        pos = self.model.getParam(1, now).vec3dValue()
        dist = (pos.x - target.x) * (pos.x - target.x) + (pos.y - target.y) * (pos.y - target.y) + (pos.z - target.z) * (pos.z - target.z)
        if dist == 0:
            self.model.setParam(1, target)
            for edge in self.edges:
                edge[0].setParam(edge[1], target + edge[2])
        else:
            interval = 1.0 / speed
            self.model.setParam(1, m3d_compile([
                "+",
                [
                    "*",
                    [
                        "-",
                        ["glob", "t"],
                        now
                    ],
                    (target - pos) / interval
                ],
                pos
            ], now + interval))
            for edge in self.edges:
                edge[0].setParam(edge[1], m3d_compile([
                    "+",
                    [
                        "*",
                        [
                            "-",
                            ["glob", "t"],
                            now
                        ],
                        (target - pos) / interval
                    ],
                    pos + edge[2]
                ], now + interval))

    def destroy(self):
        for edge in self.edges:
            edge[0].destroy()
        self.edges = []
        self.model.destroy()

    def setPosition(self, target):
        self.model.setParam(1, target)
        for edge in self.edges:
            edge[0].setParam(edge[1], target + edge[2])

def edge(node1, node2, offset1=Vec3d(0, 0, 0), offset2=Vec3d(0, 0, 0)):
    line = createLine()
    node1.addEdge(line, 1, offset1)
    node2.addEdge(line, 2, offset2)
