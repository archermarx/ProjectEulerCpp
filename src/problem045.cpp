#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <chrono>
#include "common.h"

using namespace std::chrono;

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

    int N_MIN = 144;
    int N_MAX = 100'000;

    // observation 1: hexagons > pentagons > triangles
    // observation 2: all hexagonal numbers are pentagonal
    auto start = high_resolution_clock::now();

    int64_t answer = -1;
    int index = -1;

    for (int i = N_MIN; i <= N_MAX; i++) {
        auto H = hexagonal<int64_t>(i);
        if (is_pentagonal(H)) {
            answer = H;
            index = i;
            break;
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Took " << duration.count() << " ms" << std::endl;

    std::cout << "Answer: " << "H(" << index << ") = " << answer << std::endl;
    std::cout << "Is triangular: " << is_triangular(answer) << std::endl;
    std::cout << "Is pentagonal: " << is_pentagonal(answer) << std::endl;
    std::cout << "Is hexagonal: " << is_hexagonal(answer) << std::endl;
}