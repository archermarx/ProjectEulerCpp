#include <cmath>
#include <vector>
#include "doctest.h"

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

bool is_prime(int n) {
    if (n == 1) {
        return false;
    } else if (n < 4) {
        return true;
    } else if (n % 2 == 0) {
        return false;
    } else if (n < 9) {
        return true;
    } else if (n % 3 == 0) {
        return false;
    } else {
        int f = 5;
        while (f*f <= n) {
            if (n % f == 0) {
                return false;
            }
            if (n % (f+2) == 0) {
                return false;
            }
            f += 6;
        }
        return true;
    }
}

std::vector<bool> prime_sieve(int n) {

    std::vector<bool> sieve(n+1, true);

    // Sieve remaining numbers
    for (int i = 2; i*i <= n; i+=1) {
        if (sieve[i]) {
            for (int j = i*i; j <= n; j +=i) {
                sieve[j] = false;
            }
        }
    }

    return sieve;
}

std::vector<int> primes(int n) {
    auto sieve = prime_sieve(n);
    std::vector<int> _primes{2};
    for (int i = 3; i <= n; ++ i) {
        if (sieve[i]) {
            _primes.push_back(i);
        }
    }
    return _primes;
}

std::vector<int> prime_factors(const int n) {
    std::vector<int> factors{};
    int m = n;
    if (m % 2 == 0) {
        factors.push_back(2);
        while (m % 2 == 0) {
            m /= 2;
        }
    }
    if (m % 3 == 0) {
        factors.push_back(3);
        while (m % 3 == 0) {
            m /= 3;
        }
    }
    int f = 5;
    while (f * f <= m) {
        if (m % f == 0) {
            factors.push_back(f);
            while (m % f == 0) {
                m /= f;
            }
        } else {
            f += 2;
        }
    } 

    if (n != 1) {
        factors.push_back(f);
    }

    return factors;
}

template <int M>
inline bool trial_divide(int &m) {
    bool ans = false;
    if (m % M == 0) {
        ans = true;
        while (m % M == 0) {
            m /= M;
        }
    }
    return ans;
}

int num_prime_factors(const int n) {
    int N = 0;
    int m = n;
    N += trial_divide<2>(m);
    N += trial_divide<3>(m);
    int f = 5;
    while (f * f <= m) {
        if (m % f == 0) {
            N += 1;
            while (m % f == 0) {
                m /= f;
            }
        } else {
            f += 2;
        }
    } 

    if (n != 1) {
        N += 1;
    }

    return N;
}

TEST_CASE("Primes") {

    int N = 100;
    auto sieve = prime_sieve(N);
    auto p = primes(N);
    CHECK(sieve[2]);
    CHECK(sieve[3]);
    CHECK(!sieve[4]);
    CHECK(sieve[5]);
    CHECK(!sieve[6]);
    CHECK(sieve[7]);
    CHECK(!sieve[8]);
    CHECK(!sieve[9]);
    CHECK(!sieve[10]);
    CHECK(sieve[11]);
    CHECK(!sieve[12]);
    CHECK(sieve[13]);

    for (int i = 0; i <= p.size(); i ++) {
        CHECK(sieve[p[i]]);
    }

    for (int i = 2; i < N; i++) {
        auto a = sieve[i];
        CHECK(a == is_prime(i));
        if (! (a == is_prime(i))) {
            std::cout << i << std::endl;
        }
    }
}

TEST_CASE("Prime factors") {
    CHECK(prime_factors(14).size() == 2);
    CHECK(prime_factors(15).size() == 2);
    CHECK(prime_factors(644).size() == 3);
    CHECK(prime_factors(645).size() == 3);
    CHECK(prime_factors(646).size() == 3);
    for (int i = 2; i < 1000; i++) {
        CHECK(prime_factors(i).size() == num_prime_factors(i));
    }
}

