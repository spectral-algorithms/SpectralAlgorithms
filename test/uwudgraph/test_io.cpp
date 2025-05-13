#include <iostream>
#include "convenientPrint.hpp"

#include "uwudgraph/graph.hpp"
#include "uwudgraph/graph_types.hpp"
#include "uwudgraph/graphio.hpp"

std::string test_file = "test/uwudgraph/data/demo.txt";


int main() {
    std::cout << "Loading graph from edgelist file: " << test_file << std::endl;
    uwudgraph::Graph* g = uwudgraph::load_edgelist(test_file);
    // std::cout << "Graph loaded successfully!" << std::endl;
    std::cout << "Number of nodes: " << g->n << std::endl;
    std::cout << "Number of edges: " << g->m << std::endl;
    std::cout << "Adjacency list: " << std::endl;
    for (int i = 0; i < g->n; ++i) {
        std::cout << i << ": ";
        for (auto neighbor : g->get_neighbors(i)) {
            std::cout << neighbor << " ";
        }
        std::cout << std::endl;
    }
    
    return 0;
}