#pragma once
#include <vector>
#include <queue>
#include "boost/heap/d_ary_heap.hpp"
#include "boost/heap/binomial_heap.hpp"
#include <ctime>
#include <fstream>
#define DEBUG_INFO(f, args...) fprintf(stderr, "%s_%s\t%s:%d:%s():\t\t\t" f, __DATE__, __TIME__, __FILE__, __LINE__, __func__, ##args);


double bf_seq(std::vector<std::vector<int> >& weights, std::vector<int>& dist) {
    dist[0] = 0;
    bool is_change;
    clock_t t1 = clock();
    for (int i = 0; i < dist.size() - 1; ++i) {// n - 1 iteration
        is_change = false;
        for (int u = 0; u < dist.size(); u++) {
            for (int v = 0; v < dist.size();v ++) {
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
            break;
        }
    }
    double t2 = (double)(clock() - t1) / CLOCKS_PER_SEC;
    return t2;
}


double dijkstra_seq(std::vector<std::vector<int> >& weights, std::vector<int>& dist){
    int n_vertics = weights.size();
    std::vector<bool> visited (n_vertics, false);
    dist[0] = 0;
    clock_t t1 = clock();
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
            }
        }

    }
    double t2 = (double)(clock() - t1) / CLOCKS_PER_SEC;
    return t2;
}


double dijkstra_pq(std::vector<std::vector<std::pair<int, int> > >& adj_list, std::vector<int>& dist, std::string algo_type) {
    int s = 0; //adj_list[0][0].first; // стартовая вершина
    int n = adj_list.size();
//    for(int i = 0; i < n; ++i){
//        if(adj_list[i].size() == 0){
//            adj_list[i].push_back(std::make_pair(s, 100));
//        }
//    }
    std::vector<int> path(n);
    std::vector< std::pair<int, int> > ancestors (n);
    dist[s] = 0;
    clock_t t1 = clock();
    if(algo_type == "pq") {
        std::priority_queue<std::pair<int, int> > prior_q;
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
                    ancestors[to] = std::make_pair(v, len);
                    prior_q.push(std::make_pair(-dist[to], to));
                }
            }
        }

        std::vector< std::pair<int, int> > path_restoration;
        for (int v=5; v!=s; v=ancestors[v].first)
            path_restoration.push_back(std::make_pair(v+1, ancestors[v].second));
        path_restoration.push_back(std::make_pair(s+1, 0));
        reverse (path_restoration.begin(), path_restoration.end());

        std::ofstream out;
        out.open("./restoration.txt");
        if (out.is_open())
        {
            for(auto it = path_restoration.begin(); it != path_restoration.end(); ++it){
                out << (*it).first << " " << (*it).second << "\n";
            }
        }
//        std::cout << "\n";
    }
    else if (algo_type == "d_ary"){
        boost::heap::d_ary_heap<std::pair<int, int>, boost::heap::arity<5> > prior_q;
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
                    prior_q.push(std::make_pair(-dist[to], to));
                }
            }
        }
    }
    else if (algo_type == "binom"){
        boost::heap::binomial_heap<std::pair<int, int> > prior_q;
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
                    prior_q.push(std::make_pair(-dist[to], to));
                }
            }
        }
    }
    else{
        std::cout << "Unnamed algo_type" << std::endl;
    }
    double t2 = (double)(clock() - t1) / CLOCKS_PER_SEC;
    return t2;
}
