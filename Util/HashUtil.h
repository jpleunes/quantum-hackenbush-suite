#ifndef HASH_UTIL_H
#define HASH_UTIL_H

#include <vector>
#include <cstdint>

namespace std {
    template<>
    struct hash<std::vector<uint32_t>> {
        size_t operator()(const std::vector<uint32_t>& vec) const {
            // Implementation taken from https://stackoverflow.com/a/72073933
            size_t seed = vec.size();
            for(uint32_t x : vec) {
                x = ((x >> 16) ^ x) * 0x45d9f3b;
                x = ((x >> 16) ^ x) * 0x45d9f3b;
                x = (x >> 16) ^ x;
                seed ^= x + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }
    };
}

#endif // HASH_UTIL_H
