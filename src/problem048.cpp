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

uint64_t binary_log(const uint64_t n) {
    uint64_t m = n;
    uint64_t i = 0;
    while (m > 1) {
        m /= 2;
        i ++;
    }
    return i;
}

// find a * b % n
uint64_t mul_mod_n(uint64_t a, uint64_t b, uint64_t n) {
    uint64_t res = 0;
    a %= n;
    while (b) {
        if (b & 1)
            res = (res + a) % n;
        b >>= 1;
        a = (a << 1) % n;
    }

    return res;
}

// Find a^b mod n by repeated squaring
uint64_t pow_mod_n(uint64_t a, uint64_t b, uint64_t n) {

    // a^b = a^(c0 + c1*2 + c2*2^2 + c3*2^3 + ...), ci = 0 or 1
    // a^b = prod_{c_i != 0} a^2^i
    uint64_t acc = 1;
    uint64_t exponent = a;

    while (b > 0) {
    
        // check if the i-th digit of n is 1
        if (b & 1) {
            // if so, then multiply the present exponent with our accumulator (mod 10^m)
            acc = mul_mod_n(acc, exponent, n);
        }

        // square the exponent
        exponent = mul_mod_n(exponent, exponent, n);

        // Shift n0 one bit to the right
        b >>= 1;
    }

    return acc;
}

TEST_CASE("powers of two") {
    const uint64_t ten_ten = 10'000'000'000;
    CHECK(binary_log(1) == 0);
    CHECK(binary_log(2) == 1);
    CHECK(binary_log(10) == 3);
    CHECK(pow_mod_n(2, 2, 100) == 4);
    CHECK(pow_mod_n(2, 3, 100) == 8);
    CHECK(pow_mod_n(3, 3, 100) == 27);
    CHECK(pow_mod_n(11, 11, 100) == 11);
    CHECK(pow_mod_n(1000, 1000, ten_ten) == 0);
    CHECK(pow_mod_n(1000, 100, ten_ten) == 0);
    CHECK(pow_mod_n(7, 358, 10) == 9);
    CHECK(pow_mod_n(17, 17, 10) == 7);
    CHECK(pow_mod_n(74, 540, 100) == 76);
    CHECK(pow_mod_n(33, 42, 100) == 89);
    CHECK(pow_mod_n(31, 25, 100) == 51);
    CHECK(pow_mod_n(117, 1023, 229) == 141);
    CHECK(pow_mod_n(9, 9, ten_ten) == 387420489);
    CHECK(pow_mod_n(10, 10, ten_ten) == 0);
    CHECK(pow_mod_n(11, 11, ten_ten) == 5311670611);
    CHECK(pow_mod_n(12, 12, ten_ten) == 6100448256);
    CHECK(pow_mod_n(13, 13, ten_ten) == 5106592253);
    CHECK(pow_mod_n(14, 14, ten_ten) == 6825558016);
    CHECK(pow_mod_n(15, 15, ten_ten) == 380859375);
    CHECK(pow_mod_n(16, 16, ten_ten) == 3709551616);
    CHECK(pow_mod_n(19, 19, ten_ten) == 3589123979);
    CHECK(pow_mod_n(21, 21, ten_ten) == 1381124421);
    CHECK(pow_mod_n(31, 31, ten_ten) == 6044734431);
    CHECK(pow_mod_n(51, 51, ten_ten) == 8231315051);
    CHECK(pow_mod_n(99, 99, ten_ten) == 9200499899);
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
    uint64_t m = 10;
    uint64_t n = 1000;
    uint64_t ten_to_the_m = 1;
    for (uint64_t i = 0; i < m; i ++) {
        ten_to_the_m *= 10;
    }

    // compute the answer
    uint64_t answer = 0;
    for (uint64_t i = 1; i <= n; i++) {
        answer += pow_mod_n(i, i, ten_to_the_m);
    }

    std::cout<< "Answer = " << answer % ten_to_the_m << std::endl;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Took " << duration.count() << " us" << std::endl;

    return 0;
}