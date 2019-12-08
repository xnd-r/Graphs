#include <iostream>
#include "document.h"
#include "filereadstream.h"
#include <vector>
#include <queue>
#include <etc.h>
using namespace std;

void bf_seq(std::vector<std::vector<int> >& weights, std::vector<int>& dist) {

    for (auto it = dist.begin(); it != dist.end(); ++it) {
        *it = INF;
    }
    dist[0] = 0;
    bool is_change;
    for (int i = 0; i < dist.size() - 1; ++i) {// n - 1 iteration
        is_change = false;
        for (int u = 0; u < dist.size(); u++) {
            for (int v = 0; v < dist.size(); v++) {
                if (weights[u][v] < INF) {//test if u--v has an edge
                    if (dist[u] + weights[u][v] < dist[v]) {
                        is_change = true;
                        dist[v] = dist[u] + weights[u][v];
                    }
                }
            }
        }
        //if there is no change in this iteration, then we have finished
        if (!is_change) {
        }
    }
    return;
}

// TODO:
// vector<int> d (n, INF)

void get_ajiancy_list(){}


void dijkstra_seq(std::vector<std::vector<int> >& weights, std::vector<int>& dist, std::vector<int>& p){
    int n_vertics = weights.size();
    std::vector<bool> visited (n_vertics, false);
    for (auto it = dist.begin(); it != dist.end(); ++it) {
        *it = INF;
    }
    dist[0] = 0;
    for (int i=0; i<n_vertics; ++i) {
        int v = -1;
        for (int j=0; j<n_vertics; ++j)
            if (!visited[j] && (v == -1 || dist[j] < dist[v]))
                v = j;
        if (dist[v] == INF)
            break;
        visited[v] = true;

        for (size_t j=0; j<n_vertics; ++j) {
            if (dist[v] + weights[v][j] < dist[j]) {
                dist[j] = dist[v] + weights[v][j];
                p.push_back(v);
            }
        }

    }
}

void dijkstra_heap(std::vector<std::vector<std::pair<int, int> > >& adj_list, std::vector<int>& dist) {
    int s = 0; // стартовая вершина
    int n = adj_list.size();
    std::vector<int> path(n);
    dist[s] = 0;
    std::priority_queue<pair<int, int> > prior_q;
    prior_q.push(std::make_pair(0, s));
    while (!prior_q.empty()) {
        // v -- vertex what has minimum distance among all unvisited vertices
        int v = prior_q.top().second, cur_d = -prior_q.top().first;
        prior_q.pop();
        if (cur_d > dist[v]) continue;

        for (size_t j = 0; j < adj_list[v].size(); ++j) {
            int to = adj_list[v][j].first,
                    len = adj_list[v][j].second;
            if (dist[v] + len < dist[to]) {
                dist[to] = dist[v] + len;
                path[to] = v;
                prior_q.push(make_pair(-dist[to], to));
            }

        }
    }
    return;
}

int main(){
    const char* filename = "./graph.json";
    std::vector<std::vector<int> > weights;
    std::vector<std::vector<std::pair<int, int> > > adj_list = parse_json(filename, weights);
//     = parse_json(filename);
    std::vector<int> dist (adj_list.size(), INF);
    dijkstra_heap(adj_list, dist);
//    for(int i = 0; i < mat.size(); ++i) {
//        dist.push_back(0);
//    }
////    bf_seq(mat, dist);
//    std::vector<int> path;
//    dijkstra_seq(mat, dist, path);
    for (auto it = dist.begin(); it != dist.end(); ++it) {
        std::cout << *it << "\t";
    }


    return 0;
}