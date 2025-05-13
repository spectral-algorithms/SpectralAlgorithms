#include <iostream>
#include "convenientPrint.hpp"

#include "uwudgraph/graph.hpp"
#include "uwudgraph/graph_types.hpp"
#include "uwudgraph/graphio.hpp"

#include "uwudgraph/apps/ssppr/ssppr.hpp"

std::string test_file = "test/uwudgraph/data/demo.txt";


int main() {
    std::cout << "Loading graph from edgelist file: " << test_file << std::endl;
    uwudgraph::Graph* g = uwudgraph::load_edgelist(test_file);
    // std::cout << "Graph loaded successfully!" << std::endl;
    std::cout << "Number of nodes: " << g->n << std::endl;
    std::cout << "Number of edges: " << g->m << std::endl;
    
    std::vector<double> ppr;
    ppr = uwudgraph::ppr_rw(*g, 0, 0.2, 10000);
    print("PPR vector by rw: ", ppr);    
    ppr = uwudgraph::ppr_forarw_skelton(*g, 0, 0.2, 0.01, 100);
    print("PPR vector by forarw_skelton: ", ppr);
    ppr = uwudgraph::ppr_speedppr(*g, 0, 0.2, 0.01, 1.0/g->n, 1.0/g->n);
    print("PPR vector by speedppr: ", ppr);
    ppr = uwudgraph::ppr_fora(*g, 0, 0.2, 0.01, 1.0/g->n, 1.0/g->n);
    print("PPR vector by forarw: ", ppr);
    ppr = uwudgraph::ppr_forwardpush(*g, 0, 0.2, 0.01).first;
    print("PPR vector by forwardpush: ", ppr);
    ppr = uwudgraph::ppr_ppw(*g, 0, 0.2, 100, 100, 10);
    print("PPR vector by ppw: ", ppr);
    return 0;
}