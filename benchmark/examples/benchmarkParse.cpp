#include <benchmark/benchmark.h>
#include <formula.hpp>
using namespace formula;

///////////////// ParseFormula /////////////////
bool ParseFormula()
{
    Formula f = "sin(pi/12)^2 + 0.65*(-8.32 + 9) + 3 / tan(pi/4)";
    return true;
}
void BM_ParseFormula(benchmark::State &state)
{
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(ParseFormula());
    }
}

///////////////// EvalFormula /////////////////
bool EvalFormula(Formula &f)
{
    f(-3,5);
    return true;
}
void BM_EvalFormula(benchmark::State &state)
{
    Formula f = "sin(pi/12)^2 + 0.65*(-8.32 + 9) + 3 / tan(pi/4)";
    for (auto _ : state)
    {
        benchmark::DoNotOptimize(EvalFormula(f));
    }
}

BENCHMARK(BM_ParseFormula)->Iterations(1'000'000);
BENCHMARK(BM_EvalFormula)->Iterations(1'000'000);

BENCHMARK_MAIN();