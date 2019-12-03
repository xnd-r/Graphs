from PyQt5 import QtCore, QtWidgets
import nodz_main
import json
import time
import math
try:
    app = QtWidgets.QApplication([])
except:
    # I guess we're running somewhere that already has a QApp created
    app = None

nodz = nodz_main.Nodz(None)
# nodz.loadConfig(filePath='')
nodz.initialize()
nodz.show()

pi = 3.1415926

with open("/home/xndr/devel/c++/Graphs/graph.json", "r") as f:
    text_json = f.read()
    rgg_graph = json.loads(text_json)
    node_names = []
    for i in rgg_graph["graph"]["nodes"]:
        node_names.append(int(i["id"]))
    edges = []
    weights = []
    for i in rgg_graph["graph"]["edges"]:
        if int(i["weight"])!=-1 and int(i["source"]) != int(i["target"]):
            edges.append([int(i["source"]), int(i["target"])])
            weights.append(int(i["weight"]))
print(node_names)
nvertics = len(node_names) 
print(len(edges))

nodes = []
x_c, y_c = 1000, 1000

for i, node in enumerate(node_names):
    x = math.cos((pi/2 + 2*pi*i)/nvertics) * 150
    y = math.sin((pi/2 + 2*pi*i)/nvertics) *150
    nodes.append(nodz.createNode(name=node, preset='node_default', position=QtCore.QPoint(x_c+x, y_c+y)))
for i, node in enumerate(node_names):
    nodz.createAttribute(node=nodes[i], name='attr', index=-1, preset='attr_preset_1', plug=True, 
    socket=True, dataType=int, plugMaxConnections=10000, socketMaxConnections=10000)

for i, edge in enumerate(edges):
    nodz.createConnection(edge[0], 'attr', edge[1], 'attr',is_opt=True)


# Graph
print( nodz.evaluateGraph())

nodz.saveGraph(filePath='./saved_graph.json')

nodz.clearGraph()

nodz.loadGraph(filePath='./saved_graph.json')

if app:
    # command line stand alone test... run our own event loop
    app.exec_()
