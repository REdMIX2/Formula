#include <benchmark/benchmark.h>
#include <formula.hpp>
using namespace formula;

void formulaParse(benchmark::State &state)
{
    Formula f = "sin(pi/12)^2 + 0.65*(-8.32 + 9) + 3 / tan(pi/4)";
}

// void ParseFormulaBench(benchmark::State &state)
// {

//         benchmark::DoNotOptimize(formulaParse);   
//     }
// }


BENCHMARK(formulaParse);

BENCHMARK_MAIN();