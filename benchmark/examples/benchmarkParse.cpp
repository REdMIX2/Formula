#include "benchmark/cppbenchmark.h"
#include <formula.hpp>
using namespace formula;

// Benchmark sin() call for 5 seconds (by default).
// Make 5 attemtps (by default) and choose one with the best time result.
BENCHMARK("formula-parse")
{
    Formula f = "sin(pi/12)^2 + 0.65*(-8.32 + 9) + 3 / tan(pi/4)";
}

BENCHMARK_MAIN()
