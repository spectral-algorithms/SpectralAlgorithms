/*
  This header file provides utilities for generating random numbers and sampling
  from various distributions. It includes functions for generating uniform,
  geometric, and binomial random numbers, as well as an implementation of
  alias sampling for non-uniform distributions.

  Example usage:
  - Generate a uniform random double in [0, 1):
    double u = rand_uniformf();

  - Generate a uniform random uint32 in [0, n):
    uint32_t r = rand_uniform(n);

  - Generate a geometric random uint32 with probability p:
    uint32_t g = rand_geometric(p);

  - Generate a binomial random uint32 with n trials and probability p:
    uint32_t b = rand_binomial(n, p);

  - Use AliasSampler for non-uniform sampling:
    std::vector<double> weights = {0.1, 0.3, 0.6};
    AliasSampler sampler(weights);
    uint32_t sample = sampler.sample();
*/


#pragma once

#include <cmath>
#include <cstdint>
#include <random>
#include <stack>

std::mt19937 rand_uint{(std::random_device())()};

// uniform random double in [0, 1)
double rand_uniformf() {
  return 0x1.0p-32 * rand_uint();
}

// uniform random uint32 in [0, n)
uint32_t rand_uniform(uint32_t n) {
  uint64_t m = (uint64_t)rand_uint() * n;
  if ((uint32_t)m < n) {
    uint32_t t = -n;
    if (t >= n) t -= n;
    if (t >= n) t %= n;
    while ((uint32_t)m < t)
      m = (uint64_t)rand_uint() * n;
  }
  return m >> 32;
}

// geometric random uint32 with p(x=k) = (1-p)^k * p
uint32_t rand_geometric(double p) {
  if (p == 1) return 1;
  uint32_t x = rand_uint();
  while (x == 0)
    x = rand_uint();
  double u = 0x1.0p-32 * x;
  return (uint32_t)ceil(std::log(u) / std::log(1 - p));
}

// binomial random uint32 with p(x=k) = C(n, k) * p^k * (1-p)^(n-k)
uint32_t rand_binomial(uint32_t n, double p) {
  if (p == 0) return 0;
  if (p == 1) return n;
  uint32_t k = 0;
  for (uint64_t i = 0; i <= n; ++k)
    i += rand_geometric(p);
  return k - 1;
}

// Alias sampling for non-uniform distributions
class AliasSampler {
private:
  std::vector<double> probabilities;
  std::vector<uint32_t> alias;
  std::vector<double> prob;
  uint32_t n;

public:
  AliasSampler(const std::vector<double>& weights) {
    n = weights.size();
    probabilities.resize(n);
    alias.resize(n);
    prob.resize(n);

    // Normalize weights
    double sum = 0;
    for (double w : weights) sum += w;
    for (uint32_t i = 0; i < n; ++i) probabilities[i] = weights[i] * n / sum;

    // Initialize small and large stacks
    std::stack<uint32_t> small, large;
    for (uint32_t i = 0; i < n; ++i) {
      if (probabilities[i] < 1.0)
        small.push(i);
      else
        large.push(i);
    }

    // Construct alias table
    while (!small.empty() && !large.empty()) {
      uint32_t s = small.top();
      small.pop();
      uint32_t l = large.top();
      large.pop();

      prob[s] = probabilities[s];
      alias[s] = l;

      probabilities[l] = probabilities[l] + probabilities[s] - 1.0;
      if (probabilities[l] < 1.0)
        small.push(l);
      else
        large.push(l);
    }

    // Assign remaining probabilities
    while (!large.empty()) {
      uint32_t l = large.top();
      large.pop();
      prob[l] = 1.0;
    }
    while (!small.empty()) {
      uint32_t s = small.top();
      small.pop();
      prob[s] = 1.0;
    }
  }

  uint32_t sample() const {
    uint32_t i = rand_uniform(n);
    double u = rand_uniformf();
    return u < prob[i] ? i : alias[i];
  }
};
