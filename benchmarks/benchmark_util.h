#ifndef DIPLOM_BENCHMARK_UTIL_H
#define DIPLOM_BENCHMARK_UTIL_H
#include <string>
#include <vector>
#include <map>
#include "../src/simtable.h"

struct InputData{
    std::wstring requestWord;
    std::shared_ptr<std::vector<std::wstring>> words;
    std::string name;
};

std::vector<std::wstring> getWords(const std::string& path);

std::vector<wchar_t> getShuffledAlphabet();
std::vector<wchar_t> getShuffledAlphabet(std::wstring word);

void changeSymbols(std::vector<std::wstring>& words, wchar_t oldWch, wchar_t newWch);

std::wstring getRandomWord(std::vector<std::wstring> words, int len);
std::wstring getRandomWord(std::vector<std::wstring> words);

void addMistake(std::vector<std::wstring>& words, int n, const SimTable& table);

#endif //DIPLOM_BENCHMARK_UTIL_H
