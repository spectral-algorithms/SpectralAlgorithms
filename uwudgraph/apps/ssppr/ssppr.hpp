




# pragma once

#include "uwudgraph/graph.hpp"
#include "uwudgraph/graph_types.hpp"
#include "uwudgraph/graphio.hpp"

#include "ssppr_custom.hpp"


namespace uwudgraph{


std::vector<double> SSPPR(std::string filename, std::string source_str, double alpha, std::string method, double eps, double delta, double pf, double rmax, size_t rw_num, size_t pi_num, size_t sample_size, size_t batch_size){
    Graph* g = load_edgelist(filename);
    node_id source = static_cast<node_id>(std::stoul(source_str));

    if(eps == 0) eps = 0.1;
    if(delta == 0) delta = 1.0/g->n;
    if(pf == 0) pf = 1.0/g->n;
    if(rmax == 0) rmax = 1e-4;
    if(rw_num == 0) rw_num = 1000;
    if(pi_num == 0) pi_num = 10;
    if(sample_size == 0) sample_size = 100;
    if(batch_size == 0) batch_size = 10; 


    if(method == "push" or method == "forwardpush"){
        return ppr_forwardpush(*g,source,alpha,rmax).first;
    } else if(method == "rw"){
        return ppr_rw(*g, source, alpha, rw_num);
    } else if(method == "fora_skeleton"){
        return ppr_forarw_skelton(*g, source, alpha, rmax, rw_num);
    } else if(method == "fora"){
        return ppr_fora(*g, source, alpha, eps, delta, pf);
    } else if(method == "speedppr"){
        return ppr_speedppr(*g, source, alpha, eps, delta, pf);
    } else if(method == "ppw"){
        return ppr_ppw(*g, source, alpha, pi_num, sample_size, batch_size);
    } else{
        throw std::invalid_argument("Invalid method specified for SSPPR.");
    }
}

};