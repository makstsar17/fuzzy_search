#include <iostream>
#include "src/tools.h"
#include "src/simtable.h"

bool test(const SimTable& table);
bool check(std::wstring a, std::wstring b, const SimTable& table, float res);

int main()
{
    const std::string pathToFile = "..\\simtable.json";
    SimTable table(pathToFile);

    if (!test(table)) std::cout << "Error" << std::endl;
    else std::cout << "Complete" << std::endl;

    return 0;
}

bool test(const SimTable& table)
{
    if (!check(L"aba", L"aca", table, 1.0f)) return false;
    if (!check(L"aba", L"baca", table, 2.0f)) return false;
    if (!check(L"bac", L"bca", table, 1.0f)) return false;

    return true;
}

bool check(std::wstring a, std::wstring b, const SimTable& table, float res)
{
    return editDistance(a, b) == res;
    // return editDistance(a, b, table) == res;
}