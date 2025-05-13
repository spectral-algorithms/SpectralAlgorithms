/*
// This header file implements several algorithms for computing personalized PageRank (PPR) on graphs.
// The algorithms include random walk-based methods, forward push, power push, and hybrid approaches.
// Each method has its own trade-offs in terms of accuracy, efficiency, and memory usage.
// The namespace `uwudgraph` encapsulates all the functions to avoid naming conflicts.

// Sources:
//     forwardpush, fora        : "FORA: Simple and Effective Approximate Single-Source Personalized PageRank",
//     powerpush, speedppr      : "Unifying the Global and Local Approaches: An Efficient Power Iteration with Forward Push",
//     ppw                      : "Efficient Personalized PageRank Computation: The Power of Variance-Reduced Monte Carlo Approaches"，
*/


# pragma once

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <cmath>

#include "uniqueue.hpp"
#include "random.hpp"


#include "uwudgraph/graph.hpp"
#include "uwudgraph/graph_types.hpp"


namespace uwudgraph{


node_id random_walk(const Graph& g, node_id v, double alpha) {
    while (true) {
        if (rand_uniformf() < alpha) return v;
        v = g.rand_neighbor(v);
    };
}

std::vector<double> ppr_rw(const Graph& g, node_id source, double alpha, size_t rw_num){
    std::vector<double> ppr(g.n, 0.0);
    for(size_t _=0; _<rw_num; ++_){
        ppr[random_walk(g,source,alpha)] += 1.0 / rw_num;
    }
    return ppr;
}


std::pair<std::vector<double>, std::vector<double>> ppr_forwardpush(const Graph& g, node_id source, double alpha, double rmax){
    std::vector<double> ppr(g.n, 0.0);
    std::vector<double> r(g.n, 0.0);
    r[source] = 1.0;
    uniqueue<node_id> queue(g.n);
    queue.push(source);

    while(!queue.empty()){
        node_id u = queue.pop();
        ppr[u] += alpha * r[u];
        double ruv = r[u] * (1.0 - alpha) / g.get_degree(u);
        for(node_id v : g.get_neighbors(u)){
            r[v] += ruv;
            if(r[v] > g.get_degree(v) * rmax){
                queue.push(v);
            }
        }
        r[u] = 0.0;
    }
    return std::make_pair(ppr, r);
}

std::pair<std::vector<double>, std::vector<double>> ppr_powerpush(const Graph& g, node_id source, double alpha, double lambda){
    int epoch_num = 8;
    node_id scanThreshold = g.n / 4;
    
    std::vector<double> ppr(g.n, 0.0);
    std::vector<double> r(g.n, 0.0);
    r[source] = 1.0;
    uniqueue<node_id> queue(g.n);
    queue.push(source);

    double rmax = lambda / g.m;
    double rsum = 1;
    double ruv = 0;
    while(!queue.empty() && queue.size() <= scanThreshold && rsum > lambda){
        node_id u = queue.pop();
        ppr[u] += alpha * r[u];
        rsum -= alpha * r[u];
        ruv = r[u] * (1.0 - alpha) / g.get_degree(u);
        for(node_id v : g.get_neighbors(u)){
            r[v] += ruv;
            if(r[v] > g.get_degree(v) * rmax){
                queue.push(v);
            }
        }
        r[u] = 0.0;
    }
    if(rsum > lambda){
        // Switch to using sequential scan;
        for(int i=1;i<=epoch_num;++i){
            double rmaxp = std::pow(lambda, static_cast<double>(i/epoch_num)) / g.m;
            while(rsum > g.m * rmaxp){
                for(node_id u=0; u<g.n; ++u){
                    if(r[u] > g.get_degree(u) * rmaxp){
                        ppr[u] += alpha * r[u];
                        rsum -= alpha * r[u];
                        ruv = r[u] * (1.0 - alpha) / g.get_degree(u);
                        for(node_id v : g.get_neighbors(u)){
                            r[v] += ruv;
                        }
                        r[u] = 0;
                    }
                }
            }
        }
    }
    return std::make_pair(ppr, r);
}

std::vector<double> ppr_forarw_skelton(const Graph& g, node_id source, double alpha, double rmax, size_t rw_num){
    auto [ppr, r] = ppr_forwardpush(g, source, alpha, rmax);
    for(node_id u = 0; u < g.n; ++u){
        if(r[u] > 0){
            for(size_t _ = 0; _ < rw_num; ++_){
                ppr[random_walk(g,u,alpha)] += r[u] / rw_num;
            }
        }
    }
    return ppr;
}

std::vector<double> ppr_fora(const Graph& g, node_id source, double alpha, double eps, double delta, double pf){
    size_t w = ((2*eps/3+2)*std::log(2.0/pf)) / (eps*eps*delta);
    double rmax = std::sqrt(1.0/(g.m*w));
    auto [ppr, r] = ppr_forwardpush(g, source, alpha, rmax);
    for(node_id u = 0; u < g.n; ++u){
        if(r[u] > 0){
            size_t rw_num = std::ceil(r[u] * w);
            for(size_t _ = 0; _ < rw_num; ++_){
                ppr[random_walk(g,u,alpha)] += r[u] / rw_num;
            }
        }
    }
    return ppr;
}

std::vector<double> ppr_speedppr(const Graph& g, node_id source, double alpha, double eps, double delta, double pf){
    size_t w = 2 * ((2*eps/3+2)*std::log(2.0/pf)) / (eps*eps*delta);
    double lambda = static_cast<double>(g.m / w);
    auto [ppr, r] = ppr_powerpush(g, source, alpha, lambda);
    double rmax = 1.0 / w;
    for(node_id u = 0; u < g.n; ++u){
        if(r[u] > 0){
            size_t rw_num = std::ceil(r[u] * w);
            for(size_t _ = 0; _ < rw_num; ++_){
                ppr[random_walk(g,u,alpha)] += r[u] / rw_num;
            }
        }
    }
    return ppr;
}

std::vector<double> ppr_ppw(const Graph& g, node_id source, double alpha, size_t pi_num, size_t sample_size, size_t batch_size){
    std::vector<double> ppr(g.n,0);
    std::vector<double> sigma(g.n,0);
    sigma[source] = 1.0;
    std::vector<double> r = sigma;
    std::vector<double> rabs = r;
    double puv = 0;

    size_t samples_per_batch = static_cast<size_t>(std::ceil(static_cast<double>(sample_size) / batch_size));
    for(size_t _=0; _<batch_size; ++_){
        // r = sigma + (1-alpha)/alpha * P * ppr - 1/alpha * ppr
        // rabs = |r|
        for(size_t u=0; u<g.n; ++u){
            r[u] = sigma[u] - 1.0/alpha * ppr[u];
            for(node_id v : g.get_neighbors(u)){
                r[u] += ((1.0 - alpha)/alpha)  * ppr[v] / g.get_degree(u);
            }
            rabs[u] = std::abs(r[u]);
        }

        for(size_t i=0; i<samples_per_batch; ++i){
            AliasSampler sampler(rabs);
            node_id s = sampler.sample();
            ppr[random_walk(g,s,alpha)] += static_cast<double>((1-2*std::signbit(r[s])) * std::accumulate(rabs.begin(), rabs.end(), 0)) / samples_per_batch;
        }
        // ppr = \sum_{k=0}^{K-1} alpha(1-alpha)^k * sigma + (1-alpha)^K * P^K * ppr
        std::vector<double> facpsigma = sigma;
        std::vector<double> facpppr = ppr;
        std::vector<double> facpsigma_temp, facpppr_temp;
        std::fill(ppr.begin(), ppr.end(), 0);
        for(size_t k=0; k<pi_num-1; ++k){
            facpsigma_temp = facpsigma;
            facpppr_temp = facpppr;
            for(node_id u=0; u<g.n; ++u){
                ppr[u] += alpha * facpsigma[u];
                facpsigma[u] = 0;
                facpppr[u] = 0;
                for(node_id v : g.get_neighbors(u)){
                     facpsigma[u] += (1.0 - alpha) * facpsigma_temp[v] / g.get_degree(u);
                     facpppr[u] += (1.0 - alpha) * facpppr_temp[v] / g.get_degree(u);
                }    
            }
        }
        for(node_id u=0; u<g.n; ++u){
            ppr[u] += facpppr[u];
        }
    }
    return ppr;
}


}