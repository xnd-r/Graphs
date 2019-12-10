from PyQt5 import QtCore, QtWidgets
import nodz_main
import json
import time
import math
# TODO: add sys

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
# conf = nodz_utils.loadConfig("./default_config.json")

with open("../../test.json", "r") as f:
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
print("**"*20)
nvertics = len(node_names) 
print(edges)
print(weights)

adj_dict = []
for i in range(len(node_names)):
    adj_dict.append([])

for i, nd in enumerate(node_names):
    for j, ed in enumerate(edges):
        if ed[0] == nd:
            adj_dict[i].append([ed[1], weights[j]])
            #adj_dict[adj_dict.index(ed[1])].append([nd, weights[j]])    

print(adj_dict)
nodes = []
x_c, y_c = 1000, 1000
max_node_size = max([len(i) for i in adj_dict])
for i, node in enumerate(node_names):
    x = math.cos((pi/2 + 2*pi*i)/nvertics) * max_node_size * 100
    y = math.sin((pi/2 + 2*pi*i)/nvertics) * max_node_size * 100
    nodes.append(nodz.createNode(name=node, preset='node_default', position=QtCore.QPoint(x_c+x, y_c+y)))

for i, node in enumerate(node_names):
    for j in adj_dict[i]:
        nodz.createAttribute(node=nodes[i], 
        name='node{}:{}'.format(j[0], j[1]), 
        index=-1, preset='attr_preset_1', plug=True, 
        socket=True, dataType=int, 
        plugMaxConnections=10000, socketMaxConnections=10000)

for i, edge in enumerate(edges):
    nodz.createConnection(edge[0], 
    'node{}:{}'.format(edge[1], weights[i]), edge[1], 
    'node{}:{}'.format(edge[0], weights[i]), 
    distance=weights[i], is_opt=False)

# Graph
print( nodz.evaluateGraph())

nodz.saveGraph(filePath='./saved_graph.json')

nodz.clearGraph()

nodz.loadGraph(filePath='./saved_graph.json')

if app:
    # command line stand alone test... run our own event loop
    app.exec_()
