#include <codecvt>
#include "gtest/gtest.h"
#include "table_fixture.cpp"
#include "../src/tools.h"


TEST(editDistance, editDistanceWithoutTable) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;

    EXPECT_EQ(editDistance(L"", L""), 0.);
    EXPECT_EQ(editDistance(L"open", L""), 4.);
    EXPECT_EQ(editDistance(L"", L"river"), 5.);
    EXPECT_EQ(editDistance(L"elephant", L"eephlant"), 2);
    EXPECT_EQ(editDistance(L"sunday", L"monday"), 2.);
    EXPECT_EQ(editDistance(L"stack", L"steak"), 2.);
    EXPECT_EQ(editDistance(L"Cat", L"car"), 2.);
    EXPECT_EQ(editDistance(L"lake", L"lame"), 1.);
    EXPECT_EQ(editDistance(L"write", L"right"), 4.);
    EXPECT_EQ(editDistance(L"house", L"horses"), 2.);
    EXPECT_EQ(editDistance(L"chair", L"choir"), 1.);
    EXPECT_EQ(editDistance(L"rain", L"train"), 1.);
    EXPECT_EQ(editDistance(L"open", L"opne"), 1.);
    EXPECT_EQ(editDistance(L"chair", L"chiar"), 1.);
    EXPECT_EQ(editDistance(L"examples", L"examlpe"), 2.);
    EXPECT_EQ(editDistance(L"algorithm", L"algorihm"), 1.);
    EXPECT_EQ(editDistance(L"elephant", L"eelphant"), 1.);
    EXPECT_EQ(editDistance(L"mississippi", L"missippi"), 3.);
    EXPECT_EQ(editDistance(L"elephant", L"relevant"), 3.);
    EXPECT_EQ(editDistance(L"hypothesis", L"hypocrisy"), 5.);
    EXPECT_EQ(editDistance(L"phenomenon", L"penitentiary"), 8.);
    EXPECT_EQ(editDistance(L"exaggerate", L"extravagant"), 8.);
    EXPECT_EQ(editDistance(L"necessity", L"negotiable"), 8.);
    EXPECT_EQ(editDistance(L"sufficient", converter.from_bytes("insufficienṫ")), 3.);
    EXPECT_EQ(editDistance(L"ambiguous", converter.from_bytes("aḿbivalence")), 8.);
    EXPECT_EQ(editDistance(converter.from_bytes("ꭿccommodate"), L"accomplish"), 7.);
    EXPECT_EQ(editDistance(L"disastrous", converter.from_bytes("spontaneouś")), 9.);
    EXPECT_EQ(editDistance(converter.from_bytes("ℰxponential"), L"experimental"), 6.);
}


TEST_F(TableFixture, editDistanceWithTable){
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    float test;

    EXPECT_EQ(editDistance(L"aah", L"aah", *st_), 0.);
    EXPECT_EQ(editDistance(L"", L"", *st_), 0.);
    EXPECT_EQ(editDistance(L"Open", L"", *st_), 4.);
    EXPECT_EQ(editDistance(L"", L"River", *st_), 5.);

    // KEYBOARD MISTAKE

    EXPECT_EQ(editDistance(L"algorithm", L"algoritjm", *st_), .5);
    EXPECT_EQ(editDistance(L"synchronixe", L"synchronize", *st_), .5);
    EXPECT_EQ(editDistance(L"difficult", L"diffidult", *st_), .5);
    EXPECT_EQ(editDistance(L"resiluent", L"resilient", *st_), .5);
    EXPECT_EQ(editDistance(L"alliance", L"allisnce", *st_), .5);

    // VISUAL SIMILAR CHAR

    test = editDistance(L"necessary", L"recessary", *st_);
    EXPECT_TRUE(0.59 < test and test < 0.61);
    test = editDistance(L"opporlunity", L"opportunity", *st_);
    EXPECT_TRUE(0.59 < test and test < 0.61);
    test = editDistance(L"catastrophe", L"catactrophe", *st_);
    EXPECT_TRUE(0.59 < test and test < 0.61);
    test = editDistance(L"relevont", L"relevant", *st_);
    EXPECT_TRUE(0.59 < test and test < 0.61);
    test = editDistance(L"renovate", L"renonate", *st_);
    EXPECT_TRUE(0.59 < test and test < 0.61);

    // SIMILAR CHARS

    EXPECT_EQ(editDistance(L"white", converter.from_bytes("whΐte"), *st_), .25);
    EXPECT_EQ(editDistance( converter.from_bytes("brilliânt"), L"brilliant", *st_), .25);
    EXPECT_EQ(editDistance(L"agile",  converter.from_bytes("agiļe"), *st_), .25);
    EXPECT_EQ(editDistance( converter.from_bytes("anaŕchy"), L"anarchy", *st_), .25);
    EXPECT_EQ(editDistance(L"elephant",  converter.from_bytes("elephanꮏ"), *st_), .25);

    // CAPITAL

    test = editDistance(L"Frog", L"frog", *st_);
    EXPECT_TRUE(0.09 < test and test < 0.11);
    test = editDistance(L"water", L"Water", *st_);
    EXPECT_TRUE(0.09 < test and test < 0.11);
    test = editDistance(L"Horse", L"horse", *st_);
    EXPECT_TRUE(0.09 < test and test < 0.11);

    // DIFFERENT COMBINATION

    test = editDistance(L"Illustrious", L"illustriuos", *st_);
    EXPECT_TRUE(1.09 < test and test < 1.11);

    EXPECT_EQ(editDistance(L"anthropology", L"amthopology", *st_), 1.5);

    test = editDistance(L"Galaxy", converter.from_bytes("gaŀaxy"), *st_);
    EXPECT_TRUE(0.34 < test and test < 0.36);

    test = editDistance(L"Whispre", L"Vhisper", *st_);
    EXPECT_TRUE(1.59 < test and test < 1.61);

    test = editDistance(L"Thunder", L"yhunlder", *st_);
    EXPECT_TRUE(1.59 < test and test < 1.61);

    test = editDistance(L"Raimcoat", converter.from_bytes("raincaoτ"), *st_);
    EXPECT_TRUE(1.84 < test and test < 1.86);

    test = editDistance(L"Flrbfty", L"Firefly", *st_);
    EXPECT_TRUE(2.19 < test and test < 2.21);

    test = editDistance(L"Velvet", converter.from_bytes("welvět"), *st_);
    EXPECT_TRUE(0.94 < test and test < 0.96);

    test = editDistance(L"Castle", converter.from_bytes("ςasltc"), *st_);
    EXPECT_TRUE(1.94 < test and test < 1.96);

    test = editDistance(L"Carousel", L"caraucel", *st_);
    EXPECT_TRUE(1.29 < test and test < 1.31);

    EXPECT_EQ(editDistance(converter.from_bytes("Sailíng"), L"Ssiling", *st_), 0.75);

    EXPECT_EQ(editDistance(L"Luaghtr", L"Laughter", *st_), 2.);

    test = editDistance(L"Puzzle", converter.from_bytes("ᶈussle"), *st_);
    EXPECT_TRUE(1.35 < test and test < 1.36);

    test = editDistance(L"Mavshnalow", L"marshmallow", *st_);
    EXPECT_TRUE(2.19 < test and test < 2.21);

    test = editDistance(L"Serenade", converter.from_bytes("sěrěnadě"), *st_);
    EXPECT_TRUE(0.84 < test and test < 0.86);

    test = editDistance(L"wamdevlust", L"Wanderlust", *st_);
    EXPECT_TRUE(1.19 < test and test < 1.21);

    test = editDistance(L"Slardust", L"Star", *st_);
    EXPECT_TRUE(4.59 < test and test < 4.61);

    test = editDistance(converter.from_bytes("Prělěvanr"), L"Relevant", *st_);
    EXPECT_TRUE(2.09 < test and test < 2.11);

    test = editDistance(L"Accomplish", L"accamptish", *st_);
    EXPECT_TRUE(1.29 < test and test < 1.31);

    test = editDistance(L"Eccentric", converter.from_bytes("essentrīs"), *st_);
    EXPECT_TRUE(2.14 < test and test < 2.16);
}
