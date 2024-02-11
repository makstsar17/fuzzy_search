#include "benchmark_util.h"

#include <iostream>
#include <random>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <codecvt>

std::vector<std::wstring> getWords(const std::string& path){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    std::ifstream words(path);
    if(!words.is_open())
        std::cerr << "Failed to open the file: " << "words.txt" << std::endl;

    std::vector<std::wstring> result;

    std::string line;
    while (getline(words, line))
        result.emplace_back(converter.from_bytes(line));

    return result;
}

std::vector<wchar_t> getShuffledAlphabet(){
    std::random_device rd;
    std::mt19937  generator(rd());

    std::vector<wchar_t> englishSymbols;
    for (auto i = 97; i < 123; ++i)
        englishSymbols.emplace_back(static_cast<wchar_t>(i));

    std::shuffle(englishSymbols.begin(), englishSymbols.end(), generator);

    return englishSymbols;
}

std::vector<wchar_t> getShuffledAlphabet(std::wstring word){
    std::random_device rd;
    std::mt19937  generator(rd());

    std::set<wchar_t> alBegin;
    for(auto wch : word)
        alBegin.insert(wch);

    std::vector<wchar_t> englishSymbols;
    for (auto i = 97; i < 123; ++i) {
        auto wch = static_cast<wchar_t>(i);
        if(alBegin.find(wch) == alBegin.end())
            englishSymbols.emplace_back(static_cast<wchar_t>(i));
    }

    std::shuffle(englishSymbols.begin(), englishSymbols.end(), generator);

    englishSymbols.insert(englishSymbols.begin(), alBegin.begin(), alBegin.end());

    return englishSymbols;
}

void changeSymbols(std::vector<std::wstring>& words, wchar_t oldWch, wchar_t newWch){
    for(auto& word : words){
        std::replace(word.begin(), word.end(), oldWch, newWch);
    }
}

std::wstring getRandomWord(std::vector<std::wstring> words){
    std::random_device rd;
    std::mt19937  generator(rd());
    std::uniform_int_distribution<int> distribution(0,  words.size()-1);
    return words[distribution(generator)];
}

std::wstring getRandomWord(std::vector<std::wstring> words, int len){
    std::vector<int> temp;

    for (size_t i = 0; i < words.size(); i++)
        if (words[i].size() == len)
            temp.emplace_back(i);

    std::random_device rd;
    std::mt19937  generator(rd());
    std::uniform_int_distribution<int> distribution(0,  temp.size()-1);
    return words[temp[distribution(generator)]];
}

void addMistake(std::vector<std::wstring>& words, int n, const SimTable& table){
    std::random_device rd;
    std::mt19937  generator(rd());
    std::uniform_int_distribution<int> distribution(0,  words.size()-1);

    for(int i = 0; i < n; i++){
        auto wordId = distribution(generator);
        std::uniform_int_distribution<int> wdist(0,  words[wordId].size()-1);
        auto wchId = wdist(generator);
        words[wordId][wchId] = table.getRandomSimChar(words[wordId][wchId]);
    }
}
