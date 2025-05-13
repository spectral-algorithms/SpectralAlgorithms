/**
 * This program computes the Single-Source Personalized PageRank (SSPPR) for a given graph.
 *
 * Usage:
 *   SSPPR <filename> <graph_type> <source> <alpha> <method> [--args]
 *
 * Arguments:
 *   <filename>    : Path to the input graph file.
 *   <graph_type>  : Type of the graph. Currently supported: "uwudgraph".
 *   <source>      : Source node for SSPPR computation.
 *   <alpha>       : Damping factor (teleport probability) for PageRank.
 *   <method>      : Method to compute SSPPR (e.g., "push", "rw", "fora", etc.).
 *
 * Optional arguments (specified with --args):
 *   --eps         : Convergence threshold for approximation methods.
 *   --delta       : Failure probability for probabilistic methods.
 *   --pf          : Probability of failure for certain methods.
 *   --rmax        : Residual threshold for push-based methods.
 *   --rw_num      : Number of random walks for sampling-based methods.
 *   --pi_num      : Number of iterations for certain iterative methods.
 *   --sample_size : Sample size for probabilistic methods.
 *   --batch_size  : Batch size for batched processing methods.
 *   --output      : [save | display | none] (default none).
 *   --save_path   : Path to save the output if --output is set to "save".
 */

#include "convenientPrint.hpp"
#include "serialize.hpp"

#include "uwudgraph/apps/ssppr/ssppr.hpp"

int main(int argc, char **argv) {
    if (argc < 5) {
        print("Usage: SSPPR <filename> <graph_type> <source> <alpha> <method> [--args]");
        print("Optional arguments (specified with --args):");
        print("\t--eps");
        print("\t--delta");
        print("\t--pf");
        print("\t--rmax");
        print("\t--rw_num");
        print("\t--pi_num");
        print("\t--sample_size");
        print("\t--batch_size");
        return -1;
    }

    std::string filename = argv[1];
    std::string graph_type = argv[2];
    std::string source_str = argv[3];
    double alpha = std::stod(argv[4]);
    std::string method = argv[5];

    double eps = 0.0;
    double delta = 0.0;
    double pf = 0.0;
    double rmax = 0.0;
    size_t rw_num = 0;
    size_t pi_num = 0;
    size_t sample_size = 0;
    size_t batch_size = 0;
    std::string output = "";
    std::string save_path = "";

    for (int i = 6; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--eps") {
            eps = std::stod(argv[++i]);
        } else if (arg == "--delta") {
            delta = std::stod(argv[++i]);
        } else if (arg == "--pf") {
            pf = std::stod(argv[++i]);
        } else if (arg == "--rmax") {
            rmax = std::stod(argv[++i]);
        } else if (arg == "--rw_num") {
            rw_num = std::stoull(argv[++i]);
        } else if (arg == "--pi_num") {
            pi_num = std::stoull(argv[++i]);
        } else if (arg == "--sample_size") {
            sample_size = std::stoull(argv[++i]);
        } else if (arg == "--batch_size") {
            batch_size = std::stoull(argv[++i]);
        } else if (arg == "--output") {
            output = argv[++i];
        } else if (arg == "--save_path") {
            save_path = argv[++i];
        } else {
            print("Unknown argument: " + arg);
            return -1;
        }
    }

    std::vector<double> ppr;
    if (graph_type == "uwudgraph") {
        ppr = uwudgraph::SSPPR(filename, source_str, alpha, method, eps, delta, pf, rmax, rw_num, pi_num, sample_size, batch_size);
    } else {
        throw std::invalid_argument("Unsupported graph type for SSPPR: " + graph_type);
    }

    if (output == "display") {
        print("source:", source_str, "alpha:", alpha);
        print("PPR:", ppr);
    } else if (output == "save") {
        save_file(save_path, ppr);
    }

    return 0;
}