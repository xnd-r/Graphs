from subprocess import Popen, PIPE
import sys
import datetime
import os
import ast
from glob import glob

dt = datetime.datetime.now().strftime("%d-%m-%Y-%H-%M-%S")
seq_times = []
exe_file = sys.argv[1]
graph_folder = sys.argv[2]
algo_mode = sys.argv[3]
algo_type = sys.argv[4]
graph_list = sorted(glob(graph_folder + "/*.json"))
# graph_list = [g.split("/")[2] for g in graph_list]


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
        run_string = os.path.join(os.getcwd(), exe_file) + " " + graph_name + " " + algo_mode + " " + algo_type
        print(run_string)
        t = datetime.datetime.now()
        seq_times.append((min_res(run_string)))
        print(datetime.datetime.now() - t)
    print(seq_times)


def write_log():
    with open("trial" + "_" + algo_mode + '.log', 'w') as f:
        for i in seq_times:
            info = "Algo: {}, graph: {}".format(algo_mode, "graph_name")
            f.write(str(datetime.datetime.now()) + " " + info + "; min time " + str(i) + " seconds" + "\n")

if __name__ == '__main__':
    get_times()
    write_log()