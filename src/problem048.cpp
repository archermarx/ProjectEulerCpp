#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "common.h"

#include <chrono>
using namespace std::chrono;

#include <vector>


// Euler's theorem: if gcd(a, n) = 1 (a and n are coprime) and phi(n) is euler's totient function, then a^(phi(n)) = 1 (mod n)
// Euler's totient function: number of positive integers less than n that are coprime to n

// 1^1 + 2^2 + 3^3 + ... + 100^100, last ten digits = 1^1 + 2^2 + 3^3 + ... + 100^100 (mod 10^10)
// Compute phi(10^10): 
// 10^10 = (2*5)^(10) = 2^10 * 5^10
// phi(10^10) = 10^10 * (1 - 1/2) * (1 - 1/5) = 1/2 * 4/5 * 10^10 = 2/5 * 10^10 = 0.4e10 = 4e9
//

size_t binary_log(const size_t n) {
    size_t m = n;
    size_t i = 0;
    while (m > 1) {
        m /= 2;
        i ++;
    }
    return i;
}

// return nth binary digit of n
inline size_t binary_digit(const size_t n, const size_t m) {
    return (n >> (m - 1)) & 1; 
}

std::vector<size_t> powers_of_two(const size_t n) {
    std::vector<size_t> powers{};

    size_t i = 1;
    while (i <= n) {
        if (i & n) {
            powers.push_back(i);
        }
        i <<= 1;
    }
    return powers;
}

// Find n^n mod m by repeated squaring
size_t last_digits(const size_t n, const size_t m) {

    // n^n = n^(a0 + a1*2 + a2*2^2 + a3*2^3 + ...)

    // largest power of 2 less than n is 2^p
    size_t p = binary_log(n);

    // working value of n (so as not to modify input value)
    size_t n0 = n;
    
    size_t i = 0;
    size_t acc = 1;
    size_t exponent = n;

    while (i <= p) {
    
        // check if the i-th digit of n is 1
        if (n0 & 1) {
            // if so, then multiply the present exponent with our accumulator (mod 10^m)
            acc *= exponent;
            acc %= m;
        }

        // square the exponent
        exponent *= exponent;
        acc %= m; 

        // Shift n0 one bit to the right
        n0 >>= 1;
        i++;
    }

    return acc;
}

TEST_CASE("powers of two") {
    CHECK(powers_of_two(2) == std::vector<size_t>{2});
    CHECK(powers_of_two(3) == std::vector<size_t>{1, 2});
    CHECK(binary_log(1) == 0);
    CHECK(binary_log(2) == 1);
    CHECK(binary_log(10) == 3);
    CHECK(((5 >> 2) & 1) == 1);
    CHECK(binary_digit(5, 1) == 1);
    CHECK(binary_digit(5, 2) == 0);
    CHECK(binary_digit(5, 3) == 1);
    CHECK(binary_digit(4, 3) == 1);
    CHECK(binary_digit(8, 4) == 1);
    CHECK(last_digits(2, 100) == 4);
    CHECK(last_digits(3, 100) == 27);
    CHECK(last_digits(11, 100) == 11);
    CHECK(last_digits(11, 10'000'000'000) == 5311670611);
    CHECK(last_digits(1000, 10'000'000'000) == 0);
    CHECK(last_digits(100, 10'000'000'000) == 0);
    CHECK(last_digits(10, 10'000'000'000) == 0);
}

int main(int argc, char** argv) {

    doctest::Context ctx;
    // default - stop after 5 failed asserts
    ctx.setOption("abort-after", 5);
    // apply command line - argc / argv
    ctx.applyCommandLine(argc, argv);
    // override - don't break in the debugger
    ctx.setOption("no-breaks", true); 
    // run test cases unless with --no-run
    int res = ctx.run();
    // query flags (and --exit) rely on this
    if(ctx.shouldExit())
    // propagate the result of the tests
        return res;

        
    auto start = high_resolution_clock::now();
    

    // get our large power of ten
    size_t m = 10;
    size_t n = 1000;
    size_t ten_to_the_m = 1;
    for (size_t i = 0; i < m; i ++) {
        ten_to_the_m *= 10;
    }

    // initialize our answer to 1^1 + 2^2
    size_t answer = 5;
    for (size_t i = 3; i <= n; i++) {
        answer += last_digits(i, ten_to_the_m);
    }

    std::cout<< "Answer = " << answer % ten_to_the_m << std::endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Took " << duration.count() << " us" << std::endl;

    return 0;
}