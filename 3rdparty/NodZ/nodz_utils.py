import os
import json
import re
from PyQt5 import QtCore, QtGui
import math

def show_demo(graph, nodz):
    with open(graph, "r") as f:
        text_json = f.read()
        rgg_graph = json.loads(text_json)
        node_names = []
        for i in rgg_graph["graph"]["nodes"]:
            node_names.append(int(i["id"]))
        edges, weights = [], []
        for i in rgg_graph["graph"]["edges"]:
            if float(i["weight"])!=-1:
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
    x_c, y_c = 1000, 1000
    max_node_size = max([len(i) for i in adj_dict])
    for i, node in enumerate(node_names):
        x = math.cos((math.pi/2 + 2*math.pi*i)/nvertics) * max_node_size * 100
        y = math.sin((math.pi/2 + 2*math.pi*i)/nvertics) * max_node_size * 100
        nodes.append(nodz.createNode(name=node, preset='node_preset_1', position=QtCore.QPoint(x_c+x, y_c-y)))

    for i, node in enumerate(node_names):
        for j in adj_dict[i]:
            nodz.createAttribute(node=nodes[i], 
            name='node{}:{}'.format(int(j[0]), int(j[1])), 
            index=-1, preset='attr_preset_1', plug=True, 
            socket=True, dataType=int, 
            plugMaxConnections=10000, socketMaxConnections=10000)

    for i, edge in enumerate(edges):
        nodz.createConnection(int(edge[0]), 
        'node{}:{}'.format(int(edge[1]), int(weights[i])), int(edge[1]), 
        'node{}:{}'.format(int(edge[0]), int(weights[i])), 
        distance=weights[i], is_opt=False)



def _convertDataToColor(data=None, alternate=False, av=20):
    """
    Convert a list of 3 (rgb) or 4(rgba) values from the configuration
    file into a QColor.

    :param data: Input color.
    :type  data: List.

    :param alternate: Whether or not this is an alternate color.
    :type  alternate: Bool.

    :param av: Alternate value.
    :type  av: Int.

    """
    # rgb
    if len(data) == 3:
        color = QtGui.QColor(data[0], data[1], data[2])
        if alternate:
            mult = _generateAlternateColorMultiplier(color, av)


            color = QtGui.QColor(max(0, data[0]-(av*mult)), max(0, data[1]-(av*mult)), max(0, data[2]-(av*mult)))
        return color

    # rgba
    elif len(data) == 4:
        color = QtGui.QColor(data[0], data[1], data[2], data[3])
        if alternate:
            mult = _generateAlternateColorMultiplier(color, av)
            color = QtGui.QColor(max(0, data[0]-(av*mult)), max(0, data[1]-(av*mult)), max(0, data[2]-(av*mult)), data[3])
        return color

    # wrong
    else:
        print('Color from configuration is not recognized : ', data)
        print('Can only be [R, G, B] or [R, G, B, A]')
        print('Using default color !')
        color = QtGui.QColor(120, 120, 120)
        if alternate:
            color = QtGui.QColor(120-av, 120-av, 120-av)
        return color

def _generateAlternateColorMultiplier(color, av):
    """
    Generate a multiplier based on the input color lighness to increase
    the alternate value for dark color or reduce it for bright colors.

    :param color: Input color.
    :type  color: QColor.

    :param av: Alternate value.
    :type  av: Int.

    """
    lightness = color.lightness()
    mult = float(lightness)/255

    return mult

def _createPointerBoundingBox(pointerPos, bbSize):
    """
    generate a bounding box around the pointer.

    :param pointerPos: Pointer position.
    :type  pointerPos: QPoint.

    :param bbSize: Width and Height of the bounding box.
    :type  bbSize: Int.

    """
    # Create pointer's bounding box.
    point = pointerPos

    mbbPos = point
    point.setX(point.x() - bbSize / 2)
    point.setY(point.y() - bbSize / 2)

    size = QtCore.QSize(bbSize, bbSize)
    bb = QtCore.QRect(mbbPos, size)
    bb = QtCore.QRectF(bb)

    return bb

def _swapListIndices(inputList, oldIndex, newIndex):
    """
    Simply swap 2 indices in a the specified list.

    :param inputList: List that contains the elements to swap.
    :type  inputList: List.

    :param oldIndex: Index of the element to move.
    :type  oldIndex: Int.

    :param newIndex: Destination index of the element.
    :type  newIndex: Int.

    """
    if oldIndex == -1:
        oldIndex = len(inputList)-1


    if newIndex == -1:
        newIndex = len(inputList)

    value = inputList[oldIndex]
    inputList.pop(oldIndex)
    inputList.insert(newIndex, value)

# IO
def _loadConfig(filePath):
    """
    Read the configuration file and strips out comments.

    :param filePath: File path.
    :type  filePath: Str.

    """
    with open(filePath, 'r') as myfile:
        fileString = myfile.read()

        # remove comments
        cleanString = re.sub('//.*?\n|/\*.*?\*/', '', fileString, re.S)

        data = json.loads(cleanString)

    return data

def _saveData(filePath, data):
    """
    save data as a .json file

    :param filePath: Path of the .json file.
    :type  filePath: Str.

    :param data: Data you want to save.
    :type  data: Dict or List.

    """
    f = open(filePath, "w")
    f.write(json.dumps(data,
                       sort_keys = True,
                       indent = 4,
                       ensure_ascii=False))
    f.close()

    print("Data successfully saved !")

def _loadData(filePath):
    """
    load data from a .json file.

    :param filePath: Path of the .json file.
    :type  filePath: Str.

    """
    with open(filePath) as json_file:
        j_data = json.load(json_file)

    json_file.close()

    print("Data successfully loaded !")
    return j_data

