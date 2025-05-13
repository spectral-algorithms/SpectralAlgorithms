/*
   load_edgelist from file and convert to uwdigraph.
*/

#pragma once

#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

#include "graph.hpp"
#include "graph_types.hpp"


namespace uwdigraph{

edge parse_edgelist_content_line(std::string line) {
  // List the possible delimiters for the elements on the line
  std::vector<std::string> delimiters{",", " ", "\t"};

  uint32_t num_tokens_found = 0;
  int u = -1;
  int v = -1;

  // Try splitting by each delimiter in turn
  size_t split_pos = 0;
  std::string token;
  for (const std::string &delimiter: delimiters) {
    // If we have not found any delimiters of the previous types yet,
    // then try this one.
    if (num_tokens_found == 0) {
      while ((split_pos = line.find(delimiter)) != std::string::npos) {
        // Extract the portion of the line up to the delimiter
        token = line.substr(0, split_pos);
        line.erase(0, split_pos + delimiter.length());

        // If the token has length 0, then skip
        if (token.length() == 0) continue;

        // Parse the token 
        if (num_tokens_found == 0) u = std::stoi(token);
        if (num_tokens_found == 1){
          throw std::invalid_argument("Weight is not supported in uwdigraph");
        }
        // Increase the counter of the number of tokens found
        num_tokens_found++;
      }
    }
  }

  // Extract the final token in the line
  if (num_tokens_found == 1) {
    v = std::stoi(line);
    num_tokens_found++;
  } else if (num_tokens_found == 2) {
    throw std::invalid_argument("Weight is not supported in uwdigraph");
  }

  if (num_tokens_found != 2) {
    throw std::invalid_argument("Wrong number of tokens on edgelist line.");
  }

  // Make sure that the vertices u and v have been updated successfully
  if (u < 0 || v < 0) {
    throw std::invalid_argument("Parse error on edgelist line.");
  }

  return std::make_pair(u, v);
}

Graph* load_edgelist(std::string filename) {
  // Attempt to open the provided file
  std::ifstream is(filename);
  if (!is.is_open()) {
    throw std::runtime_error("Could not open file: " + filename);
  }

  Graph* graph = new Graph();
  std::string line;

  // If meta file exists, read n and m from it, otherwise we surf the data to find n and m and save to meta.
  std::string metaname = filename + ".meta";
  std::ifstream meta(metaname);
  if (meta.is_open()) {
    meta >> graph->n >> graph->m;
    meta.close();
  } else{
    while (std::getline(is, line)) {
      if (line[0] != '#' && line[0] != '/' && line.length() > 0) {
        try {
          // This line of the input file isn't a comment, parse it.
          edge this_edge = parse_edgelist_content_line(line);
  
          graph->n = std::max(graph->n, this_edge.first + 1);
          graph->n = std::max(graph->n, this_edge.second + 1);
          graph->m++;
        } catch (std::invalid_argument &e) {
          throw(std::runtime_error(e.what()));
        }
      }
    }
    // Reset the file stream to the beginning
    is.clear();
    is.seekg(0, std::ios::beg);
    // Write n and m to the meta file
    std::ofstream meta(metaname);
    if (!meta.is_open()) {
      throw std::runtime_error("Could not write to file: " + metaname);
    } else{
      meta << graph->n << " " << graph->m << std::endl;
      meta.close();
      std::cout << "Wriiten meta to " << metaname << std::endl;
    }
  }

  // Resize the adjacency list to the number of vertices
  graph->adj.resize(graph->n);
  while (std::getline(is, line)) {
    if (line[0] != '#' && line[0] != '/' && line.length() > 0) {
      try {
        // This line of the input file isn't a comment, parse it.
        edge this_edge = parse_edgelist_content_line(line);
        graph->adj[this_edge.first].emplace_back(this_edge.second);
      } catch (std::invalid_argument &e) {
        throw(std::runtime_error(e.what()));
      }
    }
  }
  is.close();
  return graph;
}

};

