#include <iostream>
#include "etc.h"
#include "methods.h"

int main(){
    const char* filename = "./graph.json";
    std::vector<std::vector<int> > weights;
    std::vector<std::vector<std::pair<int, int> > > adj_list;
    std::string parse_type = "mat";

    parse_json(filename, parse_type, adj_list, weights);

    if(parse_type == "mat"){
        std::vector<int> dist (weights.size(), INF);
        bf_seq(weights, dist);
        for (auto it = dist.begin(); it != dist.end(); ++it) {
            std::cout << *it << "\t";
        }
        std::cout<<std::endl;
        dijkstra_seq(weights, dist);
        for (auto it = dist.begin(); it != dist.end(); ++it) {
            std::cout << *it << "\t";
        }
    }
    else if (parse_type == "list"){
        std::vector<int> dist (adj_list.size(), INF);
        std::cout << "\n\n\n" << dijkstra_pq(adj_list, dist, "pq") << std::endl;
//        for (auto it = dist.begin(); it != dist.end(); ++it) {
//            std::cout << *it << "\t";
//        }
    }


    return 0;
}