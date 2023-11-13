#include <cmath>

template <typename T>
bool is_pentagonal(const T n) {
    return std::fmod(std::sqrt(1 + 24 * n), 6) == 5;
}

template <typename T>
bool is_triangular(const T t) {
    //n^2 + n - 2t = 0
    // a = 1
    // b = 1
    // c = -2t
    // n = (-1 + sqrt(1 + 4(2t)))/2
    return std::fmod(std::sqrt(1 + 8 * t), 2) == 1;
}

template <typename T>
bool is_hexagonal(const T h) {
    // 2 n^2 - n - 2h = 0
    // a = 2, b = -1, c = -2h
    // n = (1 + sqrt(1 + 4(2h)(2))) / 2
    return std::fmod(std::sqrt(1 + 8 * h), 4) == 3;
}

template <typename T>
T triangular(int n) {
    return n * (n + 1) / 2;
}

template <typename T>
T pentagonal(int n) {
    return n * (3*n - 1) / 2;
}

template <typename T>
T hexagonal(int n) {
    return n * (2*n - 1);
}