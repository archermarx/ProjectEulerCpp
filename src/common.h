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

std::vector<bool> prime_sieve(int n) {
    std::vector<bool> sieve(n, true);
    int max_i = ceil(std::sqrt(n));
    
    for (int i = 2; i < max_i; i++) {
        if (sieve[i]) {
            for (int j = i*i; j < n; j +=i) {
                sieve[j] = false;
            }
        }
    }
    return sieve;
}

TEST_CASE("Primes") {
    auto primes = prime_sieve(100);
    CHECK(primes[2]);
    CHECK(primes[3]);
    CHECK(!primes[4]);
    CHECK(primes[5]);
    CHECK(!primes[6]);
    CHECK(primes[7]);
    CHECK(!primes[8]);
    CHECK(!primes[9]);
    CHECK(!primes[10]);
    CHECK(primes[11]);
    CHECK(!primes[12]);
    CHECK(primes[13]);
}