#include <algorithm>
#include <array>
#include <cctype>

template <class T, size_t N>
struct std::hash<std::array<T, N>> {
    auto operator()(const std::array<T, N> &key) const {
        std::hash<T> hasher;
        size_t       result = 0;
        for (size_t i = 0; i < N; ++i) {
            result = result * 31 + hasher(key[i]); // ??
        }
        return result;
    }
};
