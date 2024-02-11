import requests
from bs4 import BeautifulSoup
import json

COST_DICT = {
    "SIMILAR_CHAR_COST": 0.25,
    "CAPITAL_COST": 0.1,
    "KEYBOARD_MISTAKE_COST": 0.5,
    "VISUAL_SIMILAR_CHAR_COST": 0.6
}

page = requests.get("https://symbl.cc/en/collections/fancy-letters/")

soup = BeautifulSoup(page.content, 'html.parser')

sections = soup.find_all('section', {'id': lambda x: x and x.startswith('latin-')})

sym_table = sections[2:-2]

data = dict()
data["cost"] = COST_DICT
data["data"] = dict()

# SIMILAR CHARS WITHOUT CAPITAL
for sec in sym_table:
    h = sec.find('h3')
    letter = h.text[-2].lower()

    similar_group = set(letter)
    sim_letters = sec.find_all('a')
    for sl in sim_letters:
        tag = sl['data-template']
        data_dict = json.loads(tag)
        sym = data_dict["symbol"]

    # correction of input data
        if letter == 'b' and sym == 'Ђ':
            continue
        if letter == 'n' and sym == 'ή':
            continue
        if sym == 'İ':
            continue

        if sym.islower():
            similar_group.add(sym)
        else:
            similar_group.add(sym.lower())

    if letter == 'y':
        similar_group.add("\u00fd")

    if letter == 'z':
        similar_group.add("\u017e")

    for lt1 in similar_group:
        if lt1 not in data["data"].keys():
            data["data"][lt1] = dict()

        for lt2 in similar_group:
            if lt1 != lt2:
                data["data"][lt1][lt2] = COST_DICT["SIMILAR_CHAR_COST"]


# KEYBOARD MISTAKE
keyboard = [["q", "w", "e", "r", "t", "y", "u", "i", "o", "p"],
            ["a", "s", "d", "f", "g", "h", "j", "k", "l"],
            ["z", "x", "c", "v", "b", "n", "m"]]


def add_to_chars(char, *ch_set):
    for ch in ch_set:
        data["data"][char][ch] = COST_DICT["KEYBOARD_MISTAKE_COST"]


for ind, value in enumerate(keyboard[0]):
    data_set = []
    if ind == 0:
        data_set.extend((keyboard[0][ind + 1], keyboard[1][ind]))

    elif ind == len(keyboard[0]) - 1:
        data_set.extend((keyboard[0][ind - 1], keyboard[1][ind - 1]))

    else:
        data_set.extend((keyboard[0][ind - 1], keyboard[0][ind + 1],
                         keyboard[1][ind - 1], keyboard[1][ind]))
    add_to_chars(value, *data_set)

for ind, value in enumerate(keyboard[1]):
    data_set = []
    if ind == 0:
        data_set.extend((keyboard[0][ind], keyboard[0][ind + 1],
                         keyboard[1][ind + 1], keyboard[2][ind]))

    elif ind == len(keyboard[1]) - 1:
        data_set.extend((keyboard[0][ind], keyboard[0][ind + 1],
                         keyboard[1][ind - 1]))

    elif ind == len(keyboard[1]) - 2:
        data_set.extend((keyboard[0][ind], keyboard[0][ind + 1],
                         keyboard[1][ind - 1], keyboard[1][ind + 1],
                         keyboard[2][ind - 1]))

    else:
        data_set.extend((keyboard[0][ind], keyboard[0][ind + 1],
                         keyboard[1][ind - 1], keyboard[1][ind + 1],
                         keyboard[2][ind - 1], keyboard[2][ind]))
    add_to_chars(value, *data_set)

for ind, value in enumerate(keyboard[2]):
    data_set = []
    if ind == 0:
        data_set.extend((keyboard[1][ind], keyboard[1][ind + 1],
                         keyboard[2][ind + 1]))

    elif ind == len(keyboard[2]) - 1:
        data_set.extend((keyboard[1][ind], keyboard[1][ind + 1],
                         keyboard[2][ind - 1]))

    else:
        data_set.extend((keyboard[1][ind], keyboard[1][ind + 1],
                         keyboard[2][ind - 1], keyboard[2][ind + 1]))
    add_to_chars(value, *data_set)


# VISUAL SIMILAR CHAR
def add_similar_ch(chs):
    for ch1 in chs:
        for ch2 in chs:
            if ch1 == ch2:
                continue
            if ch2 not in data["data"][ch1].keys():
                data["data"][ch1][ch2] = COST_DICT["VISUAL_SIMILAR_CHAR_COST"]
            elif COST_DICT["VISUAL_SIMILAR_CHAR_COST"] < data["data"][ch1][ch2]:
                data["data"][ch1][ch2] = COST_DICT["VISUAL_SIMILAR_CHAR_COST"]


add_similar_ch(("b", "d", "p", "q", "o"))
add_similar_ch(("i", "j", "l", "t"))
add_similar_ch(("u", "v", "w"))
add_similar_ch(("m", "n"))
add_similar_ch(("c", "e", "o", "s"))
add_similar_ch(("h", "k"))
add_similar_ch(("f", "t"))
add_similar_ch(("g", "q"))
add_similar_ch(("x", "y"))
add_similar_ch(("a", "o"))
add_similar_ch(("k", "x"))
add_similar_ch(("r", "n", "v"))

json_data = json.dumps(data, indent=3)

with open('final_symtable.json', 'w') as f:
    f.write(json_data)
