from subprocess import Popen, PIPE
import sys
import datetime
import os
import ast
from glob import glob

dt = datetime.datetime.now().strftime("%d-%m-%Y-%H-%M-%S")
seq_times = []
exe_file = sys.argv[1] # "/home/xndr/devel/c++/Graphs/cmake-build-debug/main"
graph_folder = sys.argv[2] # "/home/xndr/devel/c++/Graphs/graphs"
parse_type = sys.argv[3]  # may be "mat" or "list"
algo_type = sys.argv[4]  # may be "bf", "ds", "pq", "d_ary", "binom"
graph_list = sorted(glob(graph_folder + "/*.json"))
node_list = [g.split("/")[-1][:-5].split("_")[1] for g in graph_list]
print(node_list)
def one_res(name):
    proc = Popen(name, shell=True, stdout=PIPE, stderr=PIPE)
    proc.wait()
    res = proc.communicate()
    if proc.returncode:
        return res[1]
    return res[0]


def min_res(name):
    min_time = 10000.0
    for i in range(3): # amounth of launches
        time = float(one_res(name))
        if time < min_time:
            min_time = time
    return min_time


def get_times():
    for graph_name in graph_list:
        run_string = exe_file + " " + graph_name + " " + parse_type + " " + algo_type
        print(run_string)
        t = datetime.datetime.now()
        seq_times.append((min_res(run_string)))
        print(datetime.datetime.now() - t)
    print(seq_times)


def write_log():
    with open("Summary_sparse" + "_" + parse_type + "_" + algo_type + '.log', 'w') as f:
        for ind, i in enumerate(seq_times):
            f.write(node_list[ind] + " " + str(i) + "\n")

if __name__ == '__main__':
    get_times()
    write_log()