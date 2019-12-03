#include <iostream>
#include "document.h"
#include "filereadstream.h"
#include <vector>
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



std::vector<std::vector<int> > parse_json(const char* filename){
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

    std::vector<int> vertics;
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["nodes"].Begin(); itr != d["graph"]["nodes"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.IsObject()); // each attribute is an object
        for (rapidjson::Value::ConstMemberIterator itr2 = attribute.MemberBegin(); itr2 != attribute.MemberEnd(); ++itr2) {
//            std::cout << itr2->name.GetString() << " : " << itr2->value.GetString() << std::endl;
            vertics.push_back(atoi(itr2->value.GetString()));
        }
    }

    std::vector<int> row;
    for(int i = 0; i < vertics.size(); ++i) {
        row.push_back(INF);
    }
    std::vector<std::vector<int> > weights;
    for(int i = 0; i < vertics.size(); ++i){
        weights.push_back(row);
    }

    assert(d["graph"].HasMember("edges"));
    assert(d["graph"]["edges"].IsArray());
    for (rapidjson::Value::ConstValueIterator itr = d["graph"]["edges"].Begin(); itr != d["graph"]["edges"].End(); ++itr) {
        const rapidjson::Value& attribute = *itr;
        assert(attribute.HasMember("source")); // each attribute is an object
        assert(attribute["source"].IsString());
        std::cout << attribute["source"].GetString() << " : " << attribute["target"].GetString() << std::endl;
        weights[atoi(attribute["source"].GetString())-1][atoi(attribute["target"].GetString())-1] = atoi(attribute["weight"].GetString());
        weights[atoi(attribute["target"].GetString())-1][atoi(attribute["source"].GetString())-1] = atoi(attribute["weight"].GetString());
    }
    fclose(fp);
    return weights;
}
int main(){
    const char* filename = "./graph.json";
    std::vector<std::vector<int> > mat = parse_json(filename);
    std::vector<int> dist;
    for(int i = 0; i < mat.size(); ++i) {
        dist.push_back(0);
    }
//    bf_seq(mat, dist);
    std::vector<int> path;
    dijkstra_seq(mat, dist, path);
    for (auto it = dist.begin(); it != dist.end(); ++it) {
        std::cout << *it << "\t";
    }


    return 0;
}