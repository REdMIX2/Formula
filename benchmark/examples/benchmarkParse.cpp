#include "benchmark/cppbenchmark.h"

#include <math.h>

// Benchmark sin() call for 5 seconds (by default).
// Make 5 attemtps (by default) and choose one with the best time result.
BENCHMARK("sin")
{
    sin(123.456);
}

BENCHMARK_MAIN()