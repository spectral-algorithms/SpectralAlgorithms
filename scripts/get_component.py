'''
This script reads a graph from a file, finds the largest connected component, converts the node labels to continuous integers starting from 0,
and saves it back to a file. The graph is represented as an edge list, where each line contains two nodes connected by an edge.

Usage: python get_component.py <graph_file_path> <output_file_path>
'''

import networkx as nx
import sys

def read_graph(file_path):
    """Reads an edgelist from a file and returns a graph."""
    try:
        with open(file_path, 'r',encoding='utf-8') as f:
            graph = nx.read_edgelist(f)
        return graph
    except Exception as e:
        print(f"Error reading graph: {e}")
        return None

def save_largest_connected_component(graph, file_path):
    """Finds the largest connected component and saves it back to the file."""
    try:
        largest_cc = max(nx.connected_components(graph), key=len)
        subgraph = graph.subgraph(largest_cc)
        subgraph = nx.convert_node_labels_to_integers(subgraph, first_label=0)
        with open(file_path, 'w') as f:
            for u, v in subgraph.edges():
                f.write(f"{u}\t{v}\n")
        print(f"Largest connected component: {len(subgraph.nodes())} nodes, {len(subgraph.edges())} edges")
    except Exception as e:
        print(f"Error saving largest connected component: {e}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python get_component.py <graph_file_path> <output_file_path>")
        sys.exit(1)

    file_path = sys.argv[1]
    save_path = sys.argv[2]
    print(f"Reading graph from {file_path}...")

    graph = read_graph(file_path)
    print(f"Graph loaded with {graph.number_of_nodes()} nodes and {graph.number_of_edges()} edges.")
    if graph:
        save_largest_connected_component(graph, save_path)