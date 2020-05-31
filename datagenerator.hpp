#ifndef SKETCH_DATAGENERATOR
#define SKETCH_DATAGENERATOR

#include <cstdint>
#include <random>
#include <vector>
#include <unordered_set>
#include <algorithm>

// Generate random histograms by first picking m entries randomly to have nonzero weights, and then sorting m - 1 uniformly distributed variables between zero and one. 
// Assign weights as the gaps in sorted order.

std::vector<std::pair<uint64_t, double>> generate_weighted_set(uint64_t L0, double L1, std::mt19937_64& rng) {
    std::unordered_set<uint64_t> elements;
    std::uniform_int_distribution<uint64_t> random_index;
    while(elements.size() < L0) {
        elements.insert(random_index(rng));
    }

    std::uniform_real_distribution<double> uniform_splitter(0, 1);
    std::vector<double> z;
    for(uint64_t i = 0; i < L0 - 1; i++) {
        z.push_back(uniform_splitter(rng));
    }
    z.push_back(1.0);
    sort(z.begin(), z.end());

    double prev = 0.0;
    uint32_t j = 0;
    std::vector<std::pair<uint64_t, double>> x;
    for(uint64_t index : elements) {
        double weight = L1*(z[j] - prev);
        x.push_back(std::pair<uint64_t, double>(index, weight));
        prev = z[j];
        j++;
    }

    // Sort the vector of pairs by indices
    std::sort(x.begin(), x.end());
    return x;
}

// Given x we can generate y s.t. the intersection between x and y is equal to some pre-specificed value
// We will do this by setting y to an appropriately scaled down copy of x and adding the remaining mass to an element that does not exist in x.
 std::vector<std::pair<uint64_t, double>> generate_similar_weighted_set(const std::vector<std::pair<uint64_t, double>>& x, double relative_overlap, std::mt19937_64& rng) {
    // Pick a random free element j
    uint64_t j;
    bool free = false;
    while(!free) {
        j = rng();
        free = true;
        for(auto element : x) {
            if(j == element.first) {
                free = false;
            }
        }
    }

    double excess_weight = 0.0;
    std::vector<std::pair<uint64_t, double>> y;
    for(auto element : x) {
        double w = element.second;
        double w_scaled = w*relative_overlap;
        double excess = w - w_scaled;
        y.push_back({element.first, w_scaled});
        excess_weight += excess;
    }
    y.push_back({j, excess_weight});
    return y;
}

#endif