#ifndef hashfunction_h
#define hashfunction_h

#include <utility>

// Define a custom hash function for std::pair to use it as a key in unordered_map
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ hash2;
    }
};

#endif /* hashfunction_h */
