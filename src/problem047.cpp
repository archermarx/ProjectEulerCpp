#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"
#include "common.h"

#include <chrono>
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

        
    auto start = high_resolution_clock::now();
    
    // CODE GOES HERE
    int answer = 0;

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << "Took " << duration.count() << " us" << std::endl;

    return 0;
}