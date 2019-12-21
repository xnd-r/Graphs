#include <iostream>
#include "etc.h"
#include "methods.h"

int main(int argc, char** argv){
    const char* filename = argv[1];
    std::string parse_type = argv[2]; // may be "mat" or "list"
    std::string algo_type = argv[3]; // may be "bf", "ds", "pq", "d_ary", "binom"
    std::vector<std::vector<int> > weights;
    std::vector<std::vector<std::pair<int, int> > > adj_list;

    parse_json(filename, parse_type, adj_list, weights);

    if(parse_type == "mat"){
        std::vector<int> dist (weights.size(), INF);
        if(algo_type == "bf"){
            std::cout << bf_seq(weights, dist) << std::endl;
        }
        else if(algo_type == "ds") {
            std::cout << dijkstra_seq(weights, dist) << std::endl;
        }
    }
    else if (parse_type == "list"){
        std::vector<int> dist (adj_list.size(), INF);
        std::cout << "\n\n\n" << dijkstra_pq(adj_list, dist, algo_type) << std::endl;
    }
    return 0;
}