#include <iostream>
#include "document.h"
#include "filereadstream.h"
#include <vector>
#include <queue>
using namespace rapidjson;

using namespace std;
constexpr unsigned int INF = 100000000;

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

std::vector<std::vector<std::pair<int, int> > > parse_json(const char* filename){
    FILE* fp = fopen(filename, "r");
    char readBuffer[256];
    FileReadStream bis(fp, readBuffer, sizeof(readBuffer));
    AutoUTFInputStream<unsigned, FileReadStream> eis(bis);  // wraps bis into eis

    Document d; // Document is GenericDocument<UTF8<> >
    d.ParseStream<0, AutoUTF<unsigned> >(eis); // This parses any UTF file into UTF-8 in memory
    assert(d.HasMember("graph"));
    assert(d["graph"].IsObject());
    assert(d["graph"].HasMember("nodes"));
    assert(d["graph"]["nodes"].IsArray());

    std::vector<int> vertices;
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["nodes"].Begin(); itr != d["graph"]["nodes"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.IsObject()); // each attribute is an object
        for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
//            std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << std::endl;
            vertices.push_back(atoi(itr2->value.GetString()));
        }
    }

    std::vector<int> row;
    for(int i = 0; i < vertices.size(); ++i) {
        row.push_back(INF);
    }
    std::vector<std::vector<int> > weights;
    for(int i = 0; i < vertices.size(); ++i){
        weights.push_back(row);
    }

    assert(d["graph"].HasMember("edges"));
    assert(d["graph"]["edges"].IsArray());
    std::vector<std::pair<int, int> > vertex_list;
    std::vector<std::vector<std::pair<int, int> > > ad_list;
    for(int i = 0; i < vertices.size(); ++i){
        ad_list.push_back(vertex_list);
    }
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["edges"].Begin(); itr != d["graph"]["edges"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.HasMember("source")); // each attribute is an object
        assert(attribute["source"].IsString());
        std::cout << attribute["source"].GetString() << " : " << attribute["target"].GetString() << std::endl;
        weights[atoi(attribute["source"].GetString())-1][atoi(attribute["target"].GetString())-1] = atoi(attribute["weight"].GetString());
        weights[atoi(attribute["target"].GetString())-1][atoi(attribute["source"].GetString())-1] = atoi(attribute["weight"].GetString());
        ad_list[atoi(attribute["source"].GetString())-1].push_back(std::make_pair(atoi(attribute["target"].GetString())-1, atoi(attribute["weight"].GetString())));
        ad_list[atoi(attribute["target"].GetString())-1].push_back(std::make_pair(atoi(attribute["source"].GetString())-1, atoi(attribute["weight"].GetString())));
    }

    fclose(fp);
    return ad_list;
}
int main(){
    const char* filename = "./graph.json";
    std::vector<std::vector<std::pair<int, int> > > adj_list = parse_json(filename);
//    std::vector<std::vector<int> > mat = parse_json(filename);
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