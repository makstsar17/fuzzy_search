#ifndef DIPLOM_FUZZY_SEARCH_BENCHMARK_H
#define DIPLOM_FUZZY_SEARCH_BENCHMARK_H

#include "benchmark/benchmark.h"
#include "benchmark_util.h"
#include "../src/simtable.h"

void BenchmarkWithoutTable(benchmark::State& state, const InputData& data);

void BenchmarkWithTable(benchmark::State& state, const InputData& data, const SimTable& table);

void BenchmarkExactSearch(benchmark::State& state, const InputData& data);

#endif //DIPLOM_FUZZY_SEARCH_BENCHMARK_H
