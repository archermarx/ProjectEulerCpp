#include <iostream>
#include <ranges>
#include <algorithm>
#include <chrono>

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

int pentagonal(const int n) {
    return n * (3*n - 1) / 2;
}

int pentagonal2(const int n) {
    return n * (3*n - 1);
}

bool is_pentagonal(const int n) {
    return fmod(sqrt(1 + 24 * n), 6) == 5;
}

bool is_pentagonal2(const int n) {
    return fmod(sqrt(1 + 12 * n), 6) == 5;
}

TEST_CASE("Pentagonals") {

    std::vector<int> first_ten_pentagonals{
        1, 5, 12, 22, 35, 51, 70, 92, 117, 145
    };

    for (int i = 0; i < 10; i++ ) {
        CHECK(pentagonal(i+1) == first_ten_pentagonals[i]);
        CHECK(pentagonal2(i+1) == 2 * first_ten_pentagonals[i]);
    }

    for (int i = 0; i < 100; i ++) {
        int p = pentagonal(i+1);
        CHECK(is_pentagonal2(2*p));
        CHECK(!is_pentagonal2(2*(p+1)));
    }
}

/*
*   Let k > j without loss of generality
*   Condition 1: S = Pk + Pj is pentagonal
*       => S = k * (3k - 1) / 2 + j * (3j - 1) / 2 = n * (3n - 1) / 2
*
*   Condition 2: S = Pk - Pj is pentagonal
*       => D = k * (3k - 1) / 2 - j * (3j - 1) / 2 = m * (3n - 1) / 2
*
*   Condition 3: D = Pk - Pj is minimized
*
*   Let k = j + l, where l > 0
*   
*   2 Pj = j(3j - 1)
*   2 Pk = k(3k - 1) = (j+l)(3(j+l) - 1)
         = j (3j + 3l - 1) + l (3j + 3l - 1)
         = j (3j - 1) + 3lj + 3jl + 3l^2 - l
         = Pj + 6 lj + 3l^2 - l
         = 2 Pj + 2 Pl + 6jl
        => Pk = Pj + Pl + 3jl
*   2 Pl = 3l(l - 1) = 3l^2 - 3l
*
*   S = Pj + Pk = 2 Pj + Pl + 3jl = 2 Pj + D
*   D = Pl + 3jl  
*
*   2D  = l (6j + 3l - 1)   
*       => Difference is proportional to l
*       => for a given j, start with low l and work from there
*       => Question: where to cut it off?
*
*   Checking if number is pentagonal:
*   2 P = n * (3n - 1)
*       Requires that 3n^2 - n - 2P = 0 has integer solution n
*       a = 3
*       b = -1
*       c = -2P
*       n = (1 +/- sqrt(1 + 4(3)(2P))) / 6
*           = (1 / 6 +/- sqrt(1 + 24 P) / 6)
*           (negative branch is < 0, so)
*       n = (1 + sqrt(1 + 24p)) / 6 has integer solutions
*       => 6n = 1 + sqrt(1 + 24p)
*       => 1 + sqrt(1 + 24p) = 0 (mod 6)
*       => sqrt(1 + 24p) = 5 (mod 6)
*   
*   Conditions
*   sqrt(1 + 24 S) = 5 (mod 6)
*   sqrt(1 + 24 D) = 5 (mod 6)
*   2 eqs, 2 unknowns. Should be only one unique solution (mod 6)
*   
*   if 1 + 24p is a perfect square, then p is a generalized pentagonal number
*   Then have to use the above conditions to check whether it is a regular pentagonal number
*   
*   24p is one less than a perfect square
*   24p = q^2 - 1
*   p = (q^2 - 1) / 24
*   for a perfect square q^2, q^2-1 must be a multiple of 24
*   Example: q = 5 => p = 1 is a pentagonal number
*   Not sure if this is useful
*
*   l = 1
*       => Pl = 1
*       => D = Pl + 3jl 
*       => D = 1 + 3j = 1 (mod 3)
*       => D = 4, 7, 10, 13, 16, 19, ...
*       (D_11 = P2 - 1)
*      
*       => S = 2 Pj + D
*   l = 2
*       => Pl = 5
*       => D = 5 + 6j = 5 (mod 6)
*       => D = 11, 17, 23, 29, 37, ... (sums of adjacent pairs of l = 1 numbers)
*       (D_12 = P3 - 1)
*   
*   l = 3
*
*       => Pl = 12
*       => D = 12 + 9j = 12 (mod 9) = 3 (mod 9)
*       => D = 21, 30, 39, ...
*       (D_13 = P4 - 1)
*
*   l = 4
*       => Pl = 22
*       => D = 22 + 12j = 22 (mod 12) = 10 (mod 12)
*       => D = 34, 46, 58, ...
*       (D_14 = P5 - 1)
*   
* Idea: iterate over distances D, find only those that are pentagonal
* Or, iterate over pentagonal numbers, find those that are valid distances
*
* Conjecture: D_{1l} = P_{l+1} - 1
* Proof:
*   D_{jl} = P_{j+l} - P{j}
*


*/

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

    const int MAX_N = 10'000;
    
    auto start = std::chrono::high_resolution_clock::now();

    int smallest_difference = pentagonal2(MAX_N);
    int best_k = 0;
    int best_j = 0;
    int min_diff = MAX_N;

    std::vector<int> pentagonals(MAX_N);
    pentagonals[0] = 1;

    // brute force option
    for (int k = 2; k <= MAX_N; k++) {
        int Pk = pentagonal2(k);
        pentagonals[k - 1] = Pk;
        for (int j = k - 1; j > 0; j--) {
            int Pj = pentagonals[j-1];
            int D = Pk - Pj;
            if (D > smallest_difference) {
                min_diff = std::min(min_diff, k - j);
                break;
            }
            if (is_pentagonal2(D)) {
                int S = Pk + Pj;
                if (is_pentagonal2(S)) {
                    smallest_difference = std::min(smallest_difference, D);
                    best_k = k;
                    best_j = j;
                }
            }
        }
        if (min_diff == 1) {
            break;
        }
    }

    std::cout << smallest_difference / 2 << ", " << best_k << ", " << best_j << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Took " << duration.count() << " ms" << std::endl;
}
