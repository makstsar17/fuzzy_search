
#ifndef DIPLOM_TOOLS_H
#define DIPLOM_TOOLS_H


#include <string>
#include "simtable.h"

float editDistance(std::wstring_view a, std::wstring_view b);
float editDistance(std::wstring_view a, std::wstring_view b, const SimTable& table);


#endif //DIPLOM_TOOLS_H
