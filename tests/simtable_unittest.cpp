#include <random>
#include <codecvt>
#include "gtest/gtest.h"
#include "table_fixture.cpp"


TEST_F(TableFixture, Table){
    std::random_device rd;
    std::mt19937  generator(rd());
    std::uniform_int_distribution<int> distribution(0,  st_->table.size()-1);

    std::set<size_t> ids;
    while(ids.size() < 30)
        ids.insert(distribution(generator));

    size_t iter = 0;
    for (const auto& pair : st_->table){
        if (ids.find(iter) != ids.end()){
            for(const auto& simWch : pair.second){
                auto simWchIter = st_->table.find(simWch.first);
                EXPECT_NE(simWchIter, st_->table.end());
                EXPECT_NE(simWchIter->second.find(pair.first), simWchIter->second.end());
            }
        }
        iter++;
    }
}

TEST_F(TableFixture, getReplaceCost){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

// TEST SYMBOL A

    EXPECT_EQ(st_->getReplaceCost(L'a', L'q').value(), 0.5);

    auto testAA = st_->getReplaceCost(L'A', L'a').value();
    EXPECT_TRUE(0.09 < testAA and testAA < 0.11);

    EXPECT_EQ(st_->getReplaceCost(L'a', L'l'), std::nullopt);

    EXPECT_EQ(st_->getReplaceCost(L'a', converter.from_bytes("ǻ")[0]).value(), 0.25);

    auto testAO = st_->getReplaceCost(L'a', L'o').value();
    EXPECT_TRUE(0.59 < testAO and testAO < 0.61);


// TEST SYMBOL G

    EXPECT_EQ(st_->getReplaceCost(L'g', L't').value(), 0.5);

    auto testGG = st_->getReplaceCost(L'g', L'G').value();
    EXPECT_TRUE(0.09 < testGG and testGG < 0.11);

    EXPECT_EQ(st_->getReplaceCost(L'g', L'z'), std::nullopt);

    EXPECT_EQ(st_->getReplaceCost(L'g', converter.from_bytes("ℊ")[0]).value(), 0.25);

    auto testGQ = st_->getReplaceCost(L'g', L'q').value();
    EXPECT_TRUE(0.59 < testGQ and testGQ < 0.61);


// TEST SYMBOL N

    EXPECT_EQ(st_->getReplaceCost(L'n', L'h').value(), 0.5);

    auto testNN = st_->getReplaceCost(L'N', L'n').value();
    EXPECT_TRUE(0.09 < testNN and testNN < 0.11);

    EXPECT_EQ(st_->getReplaceCost(L'n', L'w'), std::nullopt);

    EXPECT_EQ(st_->getReplaceCost(L'n', converter.from_bytes("η")[0]).value(), 0.25);

    auto testNM = st_->getReplaceCost(L'n', L'v').value();;
    EXPECT_TRUE(0.59 < testNM and testNM < 0.61);


// TEST SYMBOL R

    EXPECT_EQ(st_->getReplaceCost(L'r', L'e').value(), 0.5);

    auto testRR = st_->getReplaceCost(L'r', L'R').value();
    EXPECT_TRUE(0.09 < testRR and testRR < 0.11);

    EXPECT_EQ(st_->getReplaceCost(L'r', L'x'), std::nullopt);

    EXPECT_EQ(st_->getReplaceCost(L'r', converter.from_bytes("ŕ")[0]).value(), 0.25);

    auto testRV = st_->getReplaceCost(L'r', L'v').value();
    EXPECT_TRUE(0.59 < testRV and testRV < 0.61);

// ANOTHER SYMBOLS

    EXPECT_EQ(st_->getReplaceCost(L'b', converter.from_bytes("β")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'c', converter.from_bytes("ς")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'd', converter.from_bytes("đ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'e', converter.from_bytes("ė")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'f', converter.from_bytes("ḟ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'h', converter.from_bytes("♄")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'i', converter.from_bytes("ì")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'j', converter.from_bytes("ɉ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'k', converter.from_bytes("ќ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'l', converter.from_bytes("ȴ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'm', converter.from_bytes("ḿ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'o', converter.from_bytes("ô")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'p', converter.from_bytes("ҏ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'q', converter.from_bytes("ᶐ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L's', converter.from_bytes("ȿ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L't', converter.from_bytes("ţ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'u', converter.from_bytes("ü")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'v', converter.from_bytes("ṽ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'w', converter.from_bytes("ẅ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'x', converter.from_bytes("ᶍ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'y', converter.from_bytes("ɤ")[0]).value(), 0.25);
    EXPECT_EQ(st_->getReplaceCost(L'z', converter.from_bytes("ɀ")[0]).value(), 0.25);
}