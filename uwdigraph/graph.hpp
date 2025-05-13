#pragma once

#include <vector>
#include <stdexcept>

#include "graph_types.hpp"
#include "random.hpp"


namespace uwdigraph{
class Graph{
public:
    node_id n = 0;
    edge_id m = 0;

    std::vector<std::vector<node_id>> adj = {};
    Graph(){};

    node_id get_degree(node_id u) const{
        return adj[u].size();
    }

    std::vector<node_id> get_neighbors(node_id u) const{
        return adj[u];
    }

    node_id rand_neighbor(node_id u) const{
        if (adj[u].size() == 0){
            throw std::runtime_error("No neighbors for node " + std::to_string(u));
        }
        return adj[u][rand_uniform(adj[u].size())];
    }
};

};
