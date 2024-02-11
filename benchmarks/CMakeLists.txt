project(benchmarks)

find_package(benchmark REQUIRED)

add_executable(benchmarks benchmark_main.cpp table_benchmark.cpp benchmark_util.h benchmark_util.cpp fuzzy_search_benchmark.h fuzzy_search_benchmark.cpp)

target_link_libraries(benchmarks similarityTable benchmark::benchmark)
