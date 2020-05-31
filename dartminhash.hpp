#ifndef SKETCH_DARTMINHASH
#define SKETCH_DARTMINHASH

#include <cstdint>
#include <random>
#include <vector>
#include <unordered_set>
#include <algorithm>
#include <cmath>
#include "similarity.hpp"
#include "hashing.hpp"
#include "darthash.hpp"

class DartMinHash {

	private:
		uint64_t k;
        TabulationHashFunction T;
        DartHash D;
        
	public:
        // Set t = k ln(k) + 2k so the probability of failing on the first run is at most exp(-2)
		DartMinHash(std::mt19937_64& rng, uint64_t k) : k(k), T(rng), D(rng, ceil(k*log(k) + 2*k)) {};

        std::vector<std::pair<uint64_t, double>> operator()(const std::vector<std::pair<uint64_t, double>>& x) {
            bool all_minhashed = false;
            double theta = 1.0;
            std::vector<std::pair<uint64_t, double>> minhashes(k, {0, std::numeric_limits<double>::max()});
            while(!all_minhashed) {
                std::vector<bool> minhashed(k, false);
                auto darts = D(x, theta);
                // Place darts into buckets
                for(auto& dart : darts) {
                    uint64_t j = T(dart.first) % k;
                    minhashed[j] = true;
                    if(dart.second < minhashes[j].second) {
                        minhashes[j] = dart;
                    }
                }
                // Verify whether all minhashes were computed
                all_minhashed = true;
                for(bool mh : minhashed) {
                    if(!mh) {
                        all_minhashed = false;
                    }
                }

                theta = theta + 0.5;
            }
            return minhashes;
        }

        std::vector<bool> onebit_minhash(const std::vector<std::pair<uint64_t, double>>& x) { 
            std::vector<bool> sketch(k, false);
            auto minhashes = (*this)(x);
            for(uint64_t i = 0; i < k; i++) {
                sketch[i] = ((minhashes[i].first & 1ull) == 1ull); // use first bit of MinHash id  
            }
            return sketch;
        }
};

#endif