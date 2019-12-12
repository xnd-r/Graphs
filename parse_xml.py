import json
from math import sin, cos, tan, pi, sqrt

def georg2decar(dLat, dLon):
    zone = int(dLon/6.0+1)
    a = 6378245.0          
    b = 6356863.019        
    e2 = (a**2-b**2)/a**2  
    n = (a-b)/(a+b)        
    F = 1.0                  
    Lat0 = 0.0                
    Lon0 = (zone*6-3)*pi/180  
    N0 = 0.0                  
    E0 = zone*1e6+500000.0    
    Lat = dLat*pi/180.0
    Lon = dLon*pi/180.0
    v = a*F*(1-e2*(sin(Lat)**2))**-0.5
    p = a*F*(1-e2)*(1-e2*(sin(Lat)**2))**-1.5
    n2 = v/p-1
    M1 = (1+n+5.0/4.0*n**2+5.0/4.0*n**3)*(Lat-Lat0)
    M2 = (3*n+3*n**2+21.0/8.0*n**3)*sin(Lat-Lat0)*cos(Lat+Lat0)
    M3 = (15.0/8.0*n**2+15.0/8.0*n**3)*sin(2*(Lat-Lat0))*cos(2*(Lat+Lat0))
    M4 = 35.0/24.0*n**3*sin(3*(Lat-Lat0))*cos(3*(Lat+Lat0))
    M = b*F*(M1-M2+M3-M4)
    I = M+N0
    II = v/2*sin(Lat)*cos(Lat)
    III = v/24*sin(Lat)*(cos(Lat))**3*(5-(tan(Lat)**2)+9*n2)
    IIIA = v/720*sin(Lat)*(cos(Lat)**5)*(61-58*(tan(Lat)**2)+(tan(Lat)**4))
    IV = v*cos(Lat)
    V = v/6*(cos(Lat)**3)*(v/p-(tan(Lat)**2))
    VI = v/120*(cos(Lat)**5)*(5-18*(tan(Lat)**2)+(tan(Lat)**4)+14*n2-58*(tan(Lat)**2)*n2)
    N = I+II*(Lon-Lon0)**2+III*(Lon-Lon0)**4+IIIA*(Lon-Lon0)**6
    E = E0+IV*(Lon-Lon0)+V*(Lon-Lon0)**3+VI*(Lon-Lon0)**5
    return int(N), int(E)

def get_dist(id1, id2, node_dict):
    N1, E1 = georg2decar(float(node_dict[id1][0]), float(node_dict[id1][1]))
    N2, E2 = georg2decar(float(node_dict[id2][0]), float(node_dict[id2][1]))
    return sqrt((N2-N1)**2+(E2-E1)**2)


def json_maker(node_dict, edge_list):
    file = open("test.json", "w")
    nodes = '\t\t\t"nodes":[\n'
    for i in node_dict.keys():
        nodes = nodes + '\t\t\t{\n\t\t\t\t' + \
            '"id": ' + '"' + i + '",\n\t\t\t\t"lat": "' + \
             str(node_dict[i][0]) + '",\n\t\t\t\t"lon": "' + str(node_dict[i][1]) + '"\n\t\t\t},\n'
    nodes = nodes[:-2] + "\n\t\t],\n"
    edges = '\t\t\t"edges":[\n'
    for i in edge_list:

        edges = edges + '\t\t\t{\n\t\t\t\t"source": ' + '"' + str(i[0]) + '",\n\t\t\t\t' + '"target": ' + '"' + str(
            i[1]) + '",\n\t\t\t\t' + '"weight": ' + '"' + str(get_dist(str(i[0]), str(i[1]), node_dict)) + '"\n\t\t\t},\n'
    edges = edges[:-2] + "\n\t\t]\n\t}\n}"        
    file.write('{\n\t"graph": {\n')
    file.write(nodes)
    file.write(edges)
    file.close()




import xml.etree.ElementTree as ET
tree = ET.parse('./unn.xml')
root = tree.getroot()

nodes = {}
for elem in root:
    if elem.tag == 'node':
        nodes[elem.attrib['id']] = [float(elem.attrib['lat']), float(elem.attrib['lon'])]

edges = []
counter = 0
for elem in root:
   # for subelem in elem:
    if elem.tag == 'way':
        counter += 1
        for i in range(len(elem)-1):
            if elem[i].tag == "nd" and elem[i+1].tag == "nd":
                edges.append([elem[i].attrib["ref"], elem[i+1].attrib["ref"]])
                # print(elem[i].attrib)
                # print(elem[i+1].attrib)
                # print("**"*20)
        # exit()  
# for ed in edges:
#     print(ed)
json_maker(nodes, edges)
# print(counter)
# print(len(nodes))