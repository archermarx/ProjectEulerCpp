#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "common.h"

#include <algorithm>
#include <chrono>
#include <stdio.h>
using namespace std::chrono;

bool ispermutation(int a, int b) {
    int digits[10] = {0};
    while (a != 0) {
        digits[a%10]++;
        a /= 10;
    }

    while (b != 0) {
        digits[b%10]--;
        b /= 10;
    }

    for (int i = 0; i < 10; i++) {
        if (digits[i] != 0)
            return false;
    }
    return true;
}

TEST_CASE("permutations") {
    CHECK(ispermutation(1234, 4321));
    CHECK(!ispermutation(1234, 1235));
}


void problem49() {

    auto const sieve = prime_sieve(10'000);
    auto const prime_list = primes(10'000);

    // find location of first prime number greater than 1000
    int ind = -1;
    for (int i = 0; i < prime_list.size(); i++) {
        if (prime_list[i] > 1000) {
            ind = i;
            break;
        }
    }

    int found_answers = 0;
    const int max_answers = 2;

    std::cout<< "starting prime: "<< prime_list[ind] << "\n";

    const int last_prime = prime_list[prime_list.size()-1];

    std::cout << "last prime: " << last_prime << "\n";
 
    // iterate over starting primes
    for (int i = ind; i < prime_list.size(); i++) {
        // todo: would be way faster to just generate all possible permutations, then check for primality of permutations, then
        // check for arithmetic sequences.
        int n1 = prime_list[i];
        // intervals have to be even numbers
        // maximum possible interval is (last_prime - n1) / 2
        for (int interval = 2; interval < (last_prime - n1) / 2; interval+=2) {
            int n2 = n1 + interval;
            if (!sieve[n2]) {
                continue;
            }
            int n3 = n2 + interval;
            if (!sieve[n3]) {
                continue;
            }
            if (!ispermutation(n1, n2)) {
                continue;
            }
            if (!ispermutation(n1, n3)) {
                continue;
            }
            std::cout << "found answer (interval=" << interval << "): " << n1 << " " << n2 << " " <<  n3 << "\n";
            found_answers += 1;
            if (found_answers == max_answers) {
                return;
            }
        }
    }
    return;
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
    
    // CODE GOES HERE
    problem49();

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Took " << duration.count() << " us" << std::endl;

    return 0;
}