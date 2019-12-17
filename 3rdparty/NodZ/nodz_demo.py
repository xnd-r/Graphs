from PyQt5 import QtCore, QtWidgets
import nodz_main, nodz_utils
import json
import time
import math
import argparse

parser = argparse.ArgumentParser(description='Shortest path in graph demo')
parser.add_argument('--graph_type', type=str, default="dense", \
    help="may be dense(m = n * n / 4), sparse (m = 2n) and real (unn map)")
parser.add_argument('--graph_path', type=str, default="", \
    help="full path to json file")    
args = parser.parse_args()

try:
    app = QtWidgets.QApplication([])
except:
    app = None

nodz = nodz_main.Nodz(None)
nodz.initialize()
nodz.show()

if args.graph_type != "real":
    nodz_utils.show_demo(args.graph_path, nodz)
else:
    # don't forget dirty hack in default config
    minlat=56.2967600
    minlon=43.9766800
    pi = 3.1415926
    new_y, new_x = nodz_main.georg2decar(minlat, minlon)

    with open("/home/xndr/devel/c++/Graphs/demo_graphs/unn.json", "r") as f:
        text_json = f.read()
        rgg_graph = json.loads(text_json)
        node_names, dlat, dlon = [], [], []
        for i in rgg_graph["graph"]["nodes"]:
            node_names.append(int(i["id"]))
            dlat.append(float(i["lat"]))
            dlon.append(float(i["lon"]))
        edges, weights = [], []
        for i in rgg_graph["graph"]["edges"]:
            if float(i["weight"])!=-1 and float(i["source"]) != float(i["target"]):
                edges.append([float(i["source"]), float(i["target"])])
                weights.append(float(i["weight"]))

    nvertics = len(node_names) 
    print("nvertics: {}, nedges: {}".format(nvertics, len(edges)))
    adj_dict = []
    for i in range(len(node_names)):
        adj_dict.append([])

    for i, nd in enumerate(node_names):
        for j, ed in enumerate(edges):
            if ed[0] == nd:
                adj_dict[i].append([ed[1], int(weights[j])])
                adj_dict[node_names.index(int(ed[1]))].append([nd, int(weights[j])])    

    for i in range(len(adj_dict)):
        if len(adj_dict[i]) == 0:
                adj_dict[i].append([float(i), 0])
    nodes = []
    x_c, y_c = 0, 5000
    max_node_size = max([len(i) for i in adj_dict])
    for i, node in enumerate(node_names):
        y, x = nodz_main.georg2decar(dlat[i], dlon[i])
        y, x = (y-new_y) * 25, (x-new_x) * 15
        nodes.append(nodz.createNode(name=node, preset='node_default', position=QtCore.QPoint(x_c+x, y_c-y)))

    for i, node in enumerate(node_names):
        for j in adj_dict[i]:
            nodz.createAttribute(node=nodes[i], 
            name='node{}:{}'.format(int(j[0]), int(j[1])), 
            index=-1, preset='attr_default', plug=True, 
            socket=True, dataType=int, 
            plugMaxConnections=10000, socketMaxConnections=10000)

    for i, edge in enumerate(edges):
        nodz.createConnection(int(edge[0]), 
        'node{}:{}'.format(int(edge[1]), int(weights[i])), int(edge[1]), 
        'node{}:{}'.format(int(edge[0]), int(weights[i])), 
        distance=weights[i], is_opt=False)


# Graph
nodz.saveGraph(filePath='./saved_graph.json')
nodz.clearGraph()
nodz.loadGraph(filePath='./saved_graph.json')

if app:
    app.exec_()
