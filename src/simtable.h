#ifndef DIPLOM_SIMTABLE_H
#define DIPLOM_SIMTABLE_H


#include <optional>
#include <string>
#include <unordered_map>
#include <map>
#include "gtest/gtest.h"

class SimTable
{
private:
    std::unordered_map<wchar_t, std::unordered_map<wchar_t, float>> table;
    std::unordered_map<std::string, float> cost;
public:
    SimTable(const std::string& path);
    ~SimTable() = default;

    SimTable(const SimTable&) = default;
    SimTable &operator=(const SimTable&) = delete;
    SimTable(SimTable&&) = delete;
    SimTable &operator=(SimTable&&) = delete;

    std::optional<float> getReplaceCost(wchar_t c1, wchar_t c2) const;

    wchar_t getRandomSimChar(wchar_t wch) const;

    wchar_t getRandomSimSymbol(wchar_t wch) const;

    FRIEND_TEST(TableFixture, Table);
};


#endif //DIPLOM_SIMTABLE_H
