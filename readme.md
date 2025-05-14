### Implementation and Tools: Spectral Algorithms

SpectralAlgorithms provides fundamental data types for spectral graph analysis and implements algorithms from various research papers. These functionalities are encapsulated in the `apps` directory, making it convenient for users to compare and utilize them. The project is written in C++ and is continuously being improved.

#### Features of SpectralAlgorithms:
- **Support for various graph types and algorithms.**
- **Ease of use with a clear structure.** You can invoke computations with a one-line command, such as calculating SSPPR (Single-Source Personalized PageRank).
- **Comprehensive and up-to-date.** We continuously add new algorithms.

For example, if you want to compute the SSPPR of an unweighted, undirected graph at a specific step in your code, simply include the following:

```cpp
#include "uwudgraph/apps/ssppr/ssppr.hpp"
#include "uwudgraph/graphio.hpp"

uwudgraph::Graph* g = load_edgelist("test/uwudgraph/data/demo.txt");

// Source node ID: 0, alpha: 0.2
std::vector<double> ppr = uwudgraph::SSPPR(g, 0, 0.2);

// Display or handle further
// ......

return 0;
```

You can also compute using different algorithms and parameters:  
`std::vector<double> ppr = uwudgraph::SSPPR(g, 0, 0.2, "push", 1e-4);`

Additionally, command-line invocation is supported:  
`apps/SSPPR test/uwudgraph/data/demo.txt 0 0.2 push --rmax 1e-4 --output display`

#### Quick Start
No external library dependencies.

Compile using `make`.
