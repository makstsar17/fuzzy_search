#include "fuzzy_search_benchmark.h"
#include "../src/tools.h"

#include <fstream>
#include <sstream>
#include <random>
#include <codecvt>
#include <algorithm>

void BenchmarkWithoutTable(benchmark::State& state, const InputData& data) {
    for (auto _ : state) {
        size_t countMatch = 0;
        auto errorThreshold = state.range(0) + 0.1;
        for(const auto& word : *data.words){
            auto result = editDistance(data.requestWord, word);
            benchmark::DoNotOptimize(result);

            if (result < errorThreshold)
                countMatch++;

        }
        state.counters["MATCHES"] = countMatch;
    }
}

void BenchmarkWithTable(benchmark::State& state, const InputData& data, const SimTable& table) {
    for (auto _ : state) {
        size_t countMatch = 0;
        auto errorThreshold = state.range(0) + 0.1;
        for(const auto& word : *data.words){
            auto result = editDistance(data.requestWord, word, table);
            benchmark::DoNotOptimize(result);

            if (result < errorThreshold)
                countMatch++;
        }
        state.counters["MATCHES"] = countMatch;
    }
}

void BenchmarkExactSearch(benchmark::State& state, const InputData& data) {
    for (auto _ : state) {
        size_t countMatch = 0;
        for(const auto& word : *data.words){
            if (word == data.requestWord)
                countMatch++;
        }
        state.counters["MATCHES"] = countMatch;
    }
}
