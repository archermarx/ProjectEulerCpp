#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "common.h"

#include <chrono>
using namespace std::chrono;


tuple<vector<bool>, vector<int>, vector<int>> primes_and_sieve(int n) {

    std::vector<bool> sieve(n+1, true);
    sieve[1] = false;

    // fill evens with false
    for (int i = 0; i <= n; i+=2) {
        sieve[i] = false;
    }

    // Sieve remaining numbers
    for (int i = 3; i*i <= n; i+=2) {
        if (sieve[i]) {
            for (int j = i*i; j <= n; j +=i) {
                sieve[j] = false;
            }
        }
    }
    
    // make list of primes and cumulative sum
    std::vector<int> _primes{2};
    std::vector<int> sums{0, 2};
    int sum = 2;
    for (int i = 3; i < sieve.size(); i+=2) {
        if (sieve[i]) {
            _primes.push_back(i);
            sum += i;
            sums.push_back(sum);
        }
    }

    return make_tuple(sieve, _primes, sums);
}

void prime_sequences(int N_max) {
    int max_size = N_max;
    const auto [sieve, p, sums]= primes_and_sieve(max_size);
    
    int sum = 0;
    int best_sum = 0;
    int best_length = 1;
    int start_ind = 0;
    int best_start_ind = 0;

    // Naive O(n^2) solution
    for (int start_ind = 0; start_ind < p.size()-1; start_ind++) {
        // check if start_ind is even feasible
        if ((sums[start_ind + best_length + 1] - sums[start_ind]) > N_max) {
            break;
        }
        for (int last_ind = start_ind + best_length; last_ind < p.size() - 1; last_ind++){
            sum = sums[last_ind + 1] - sums[start_ind];
            if (sum >= N_max) {
                break;
            }
            if (sieve[sum]) {
                int len = last_ind - start_ind + 1;
                if (len > best_length) {
                    best_length = len;
                    best_start_ind = start_ind;
                    best_sum = sum;
                }
            }
        }
    }

    std::cout << "Best length: " << best_length << std::endl;
    std::cout << "Sum: " << best_sum << std::endl;
    // int checksum = -best_sum;
    // for (int i = best_start_ind; i < best_start_ind + best_length; i++) {
    //     std::cout << p[i];
    //     checksum += p[i];
    //     if (i < best_start_ind + best_length - 1) {
    //         std::cout << ", ";
    //     } else {
    //         std::cout << std::endl;
    //     }
    // }
    // std::cout<< "Checksum: " << checksum << std::endl; 
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
    prime_sequences(1'000'000);

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Took " << duration.count() << " us" << std::endl;

    return 0;
}