project(tests)

find_package(GTest CONFIG REQUIRED)

add_executable(tests table_fixture.cpp simtable_unittest.cpp editdistance_unittest.cpp)

target_link_libraries(tests PUBLIC similarityTable GTest::gtest GTest::gtest_main)

