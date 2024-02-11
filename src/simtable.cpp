#include "simtable.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/istreamwrapper.h"
#include <fstream>
#include <iostream>
#include <cwctype>
#include <random>

typedef rapidjson::GenericDocument<rapidjson::UTF16LE<> > WDocument;

SimTable::SimTable(const std::string& path)
{
    std::ifstream ifs(path, std::ios::binary);
    if (ifs.fail()) {
        throw std::ifstream::failure("Could not open file: " + std::string(path));
    }

    rapidjson::IStreamWrapper isw(ifs);
    rapidjson::AutoUTFInputStream<unsigned , rapidjson::IStreamWrapper> eis(isw);

    WDocument document;
    document.ParseStream<0, rapidjson::AutoUTF<unsigned> >(eis);
    if (!document.IsObject()){
        throw std::runtime_error("Opened file is not JSON");
    }

    //PARSE COST
    for (auto key = document.MemberBegin(); key != document.MemberEnd(); key++) {
        if (key->name == L"cost") {
            for(auto costName = key->value.MemberBegin(); costName != key->value.MemberEnd(); costName++){
                std::wstring wstr = costName->name.GetString();
                std::string str(wstr.begin(), wstr.end());
                cost[str] = costName->value.GetFloat();
            }
            break;
        }
    }

    //PARSE DATA
    for (auto key = document.MemberBegin(); key != document.MemberEnd(); key++){
        if(key->name == L"data"){
            for (auto wch = key->value.MemberBegin(); wch != key->value.MemberEnd(); wch++){
                for (auto simWch = wch->value.MemberBegin(); simWch != wch->value.MemberEnd(); simWch++){
                    auto valueWch = wch->name.GetString()[0];
                    auto valueSimWch = simWch->name.GetString()[0];
                    auto price = simWch->value.GetFloat();

                    table[valueWch][valueSimWch] = price;
                }
            }
            break;
        }
    }
}

std::optional<float> SimTable::getReplaceCost(wchar_t c1, wchar_t c2) const
{
    if (c1 == c2){
        return 0.;
    }

    std::optional<float> result;

    bool isUpperC1 = std::iswupper(c1);
    bool isUpperC2 = std::iswupper(c2);


    wchar_t lowerC1 = isUpperC1 ? std::towlower(c1) : c1;
    wchar_t lowerC2 = isUpperC2 ? std::towlower(c2) : c2;
    if (lowerC1 == lowerC2){
        return cost.at("CAPITAL_COST");
    }

    auto simChars = table.find(lowerC1);
    if(simChars != table.end()){
        auto resPrice = simChars->second.find(lowerC2);
        if (resPrice != simChars->second.end()){
            result = resPrice->second;
        }
    }

    if ((isUpperC1 xor isUpperC2) and result.has_value())
        result.emplace(result.value() + cost.at("CAPITAL_COST"));

    return result;
}

wchar_t SimTable::getRandomSimChar(wchar_t wch) const {
    std::random_device rd;
    std::mt19937  generator(rd());

    auto simObjs = table.find(wch);
    if (simObjs != table.end()) {
        std::vector<wchar_t> temp;
        for(auto simPair : simObjs->second){
            if(simPair.second == 0.25){
                temp.emplace_back(simPair.first);
            }
        }
        std::uniform_int_distribution<int> dist(0, temp.size() - 1);
        return temp[dist(generator)];
    }
    return L'\0';
}

wchar_t SimTable::getRandomSimSymbol(wchar_t wch) const {
    std::random_device rd;
    std::mt19937  generator(rd());

    auto simObjs = table.find(wch);
    if (simObjs != table.end()) {
        std::uniform_int_distribution<int> dist(0, simObjs->second.size() - 1);
        auto resId = dist(generator);

        int currId = 0;
        for(auto simWch : simObjs->second){
            if(currId == resId)
                return simWch.first;

            currId++;
        }
    }
    return L'\0';
}
