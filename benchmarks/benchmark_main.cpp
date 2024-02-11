
#include "table_benchmark.cpp"
#include "benchmark_util.h"
#include "fuzzy_search_benchmark.h"

#include <fstream>
#include <codecvt>
#include <iostream>
#include <string>
#include <cctype>
#include <cwctype>

std::wstring strip(const std::wstring& input) {
    if(input.length() == 0)
        return input;

    std::size_t start = 0;
    std::size_t end = input.length() - 1;

    while (start <= end && std::iswspace(input[start])) {
        start++;
    }

    while (end >= start && std::iswspace(input[end])) {
        end--;
    }

    return input.substr(start, end - start + 1);
}

std::wstring removePunctuation(const std::wstring& input) {
    std::wstring result;
    for (auto c : input) {
        if (!std::iswpunct(c)) {
            result += c;
        }
    }
    return result;
}

std::vector<std::wstring> readFile(const std::string& path){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    std::ifstream words(path);
    if(!words.is_open())
        std::cerr << "Failed to open the file: " << "words.txt" << std::endl;

    std::vector<std::wstring> result;

    std::string line;
    while (getline(words, line)) {
        std::wstringstream ss(strip(converter.from_bytes(line)));
        std::wstring word;
        while (getline(ss, word, L' ')){
            result.emplace_back(removePunctuation(word));
        }
    }
    words.close();
    return result;
}

int getErrorThreshold(int len){
    return len/3;
}

int main(int argc, char** argv) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    benchmark::Initialize(&argc, argv);

    BENCHMARK(BM_TABLE)->Unit(benchmark::kMillisecond);

    const std::string pathToSimTable = "..\\..\\simtable.json";

    SimTable table(pathToSimTable);

    const std::string pathToTestWords = (R"(..\..\benchmarks\words.txt)");

    InputData dataAlp;
    dataAlp.words = std::make_shared<std::vector<std::wstring>>(getWords(pathToTestWords));
    dataAlp.requestWord = getRandomWord(*dataAlp.words, 4);


    auto shuffledAlphabet = getShuffledAlphabet(dataAlp.requestWord);

    for(auto amountOfSymbolsToReplace = 1; amountOfSymbolsToReplace < 10; amountOfSymbolsToReplace++){
        auto oldSym = shuffledAlphabet[amountOfSymbolsToReplace-1];
        auto newSym = table.getRandomSimChar(oldSym);
        changeSymbols(*dataAlp.words, oldSym, newSym);
        for(auto errorThreshold = 0; errorThreshold <= 350; errorThreshold += 50){
            benchmark::RegisterBenchmark("ALBenchmarkWithoutTable", [dataAlp](benchmark::State& state){
                BenchmarkWithoutTable(state, dataAlp);
            })
            ->Args({amountOfSymbolsToReplace, errorThreshold})
            ->Unit(benchmark::kMillisecond)
            ->Iterations(1);

            benchmark::RegisterBenchmark("ALBenchmarkWithTable", [dataAlp, table](benchmark::State& state){
                BenchmarkWithTable(state, dataAlp, table);
            })
            ->Args({amountOfSymbolsToReplace, errorThreshold})
            ->Unit(benchmark::kMillisecond)
            ->Iterations(1);
        }
    }


    const std::string pathToEnglishText = (R"(..\..\test_text\HP-English.txt)");
    const std::string pathToGermanText = (R"(..\..\test_text\HP-German.txt)");
    const std::string pathToCzechText = (R"(..\..\test_text\HP-Czech.txt)");

    const auto& engText = std::make_shared<std::vector<std::wstring>>(readFile(pathToEnglishText));
    const auto& gerText = std::make_shared<std::vector<std::wstring>>(readFile(pathToGermanText));
    const auto& czText = std::make_shared<std::vector<std::wstring>>(readFile(pathToCzechText));

    std::vector<InputData> data;
    data.emplace_back(converter.from_bytes("cloud"), engText, "English_Text");
    data.emplace_back(converter.from_bytes("Cloud"), engText, "English_Text");
    data.emplace_back(converter.from_bytes("ciouf"), engText, "English_Text");
    data.emplace_back(converter.from_bytes("clóud"), engText, "English_Text");
    data.emplace_back(converter.from_bytes("clóůd"), engText, "English_Text");
    data.emplace_back(converter.from_bytes("clóůd"), engText, "English_Text");

//    data.emplace_back(converter.from_bytes("thousand"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("Thousand"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("thovsamd"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("thöusand"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("thöüsand"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("thöüsänd"), engText, "English_Text");
//
//    data.emplace_back(converter.from_bytes("strange"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("Strange"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("sttanqe"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("štrange"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("štřange"), engText, "English_Text");
//    data.emplace_back(converter.from_bytes("štřánge"), engText, "English_Text");
//
    data.emplace_back(converter.from_bytes("geändert"), gerText, "German_Text");
    data.emplace_back(converter.from_bytes("Geändert"), gerText, "German_Text");
    data.emplace_back(converter.from_bytes("Qeänberl"), gerText, "German_Text");
    data.emplace_back(converter.from_bytes("qeandert"), gerText, "German_Text");
    data.emplace_back(converter.from_bytes("geándert"), gerText, "German_Text");
    data.emplace_back(converter.from_bytes("geándeřť"), gerText, "German_Text");
//
//    data.emplace_back(converter.from_bytes("völlig"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("Völlig"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("Vöttiq"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("vokkig"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("vóllig"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("vóllíg"), gerText, "German_Text");
//
//    data.emplace_back(converter.from_bytes("erzählte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("Erzählte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("erxähite"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("Erzahlte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("eřzáhlte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("ěřzáhltě"), gerText, "German_Text");
//
//    data.emplace_back(converter.from_bytes("erzählte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("Erzählte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("erzäbkte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("erzahlte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("érzáhlte"), gerText, "German_Text");
//    data.emplace_back(converter.from_bytes("éřzáhlte"), gerText, "German_Text");
//


    data.emplace_back(converter.from_bytes("každý"), czText, "Czech_Text");
    data.emplace_back(converter.from_bytes("Každý"), czText, "Czech_Text");
    data.emplace_back(converter.from_bytes("Ksždý"), czText, "Czech_Text");
    data.emplace_back(converter.from_bytes("kazdý"), czText, "Czech_Text");
    data.emplace_back(converter.from_bytes("kazdy"), czText, "Czech_Text");
    data.emplace_back(converter.from_bytes("käzdy"), czText, "Czech_Text");
//
//    data.emplace_back(converter.from_bytes("měsíčních"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("Měsíčních"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("Měsíčmícj"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("měsičnich"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("mesičnich"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("mecicnich"), czText, "Czech_Text");

//    data.emplace_back(converter.from_bytes("stříbrné"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("Stříbrné"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("dtřídrné"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("stríbrné"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("stribrné"), czText, "Czech_Text");
//    data.emplace_back(converter.from_bytes("stridrne"), czText, "Czech_Text");



    for (const auto& inputData : data) {
        benchmark::RegisterBenchmark("HPBenchmarkWithoutTable", [inputData](benchmark::State &state) {
            BenchmarkWithoutTable(state, inputData);
        })
                ->Args({getErrorThreshold(inputData.requestWord.length())})
                ->Unit(benchmark::kMillisecond)
                ->Iterations(1);

        benchmark::RegisterBenchmark("HPBenchmarkWithTable", [inputData, table](benchmark::State &state) {
            BenchmarkWithTable(state, inputData, table);
        })
                ->Args({getErrorThreshold(inputData.requestWord.length())})
                ->Unit(benchmark::kMillisecond)
                ->Iterations(1);

        benchmark::RegisterBenchmark("HPBenchmarkExactSearch", [inputData, table](benchmark::State &state) {
            BenchmarkExactSearch(state, inputData);
        })
                ->Unit(benchmark::kMillisecond)
                ->Iterations(1);
    }
        benchmark::RunSpecifiedBenchmarks();
        benchmark::Shutdown();

    std::wofstream file(R"(..\..\test_text\res_additional.txt)", std::ios::binary | std::ios::trunc);
    const unsigned long MaxCode = 0x10ffff;
    const std::codecvt_mode Mode = std::generate_header;
    std::locale utf16_locale_data(file.getloc(), new std::codecvt_utf16<wchar_t, MaxCode, Mode>);
    file.imbue(utf16_locale_data);
    if (!file.is_open())
        std::cerr << "Failed to create the file: " << R"(..\..\test_text\res_additional.txt)" << std::endl;

    for(const auto& inputData : data){
        file << inputData.requestWord << converter.from_bytes(" ") << converter.from_bytes(inputData.name) << converter.from_bytes('\n');
    }
    file.close();
}
