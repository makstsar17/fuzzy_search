#include "benchmark/benchmark.h"
#include "../src/simtable.h"

static void BM_TABLE(benchmark::State& state) {
    for (auto _ : state) {
        benchmark::DoNotOptimize(SimTable("..\\..\\simtable.json"));
    }
}
