/**
 * EDOProSE Syntax Checker
 * Copyright (C) 2019  Kevin Lu
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>

#include <dirent.h>

#include "ocgapi.h"
#include "common.h"


int GetCard([[maybe_unused]] void *payload, int code, OCG_CardData *card) {
    memset(card, 0, sizeof(OCG_CardData));
    card->code = code;
    return true;
}

int LoadScript([[maybe_unused]] void *payload, OCG_Duel duel, const char *path) {
    std::ifstream f(path);
    std::stringstream buf;
    buf << f.rdbuf();
    return buf.str().length() && OCG_LoadScript(duel, buf.str().c_str(), buf.str().length(), path);
}

int exitCode = EXIT_SUCCESS;
void Log([[maybe_unused]] void *payload, const char *string, int type) {
    std::cerr << type << ": " << string << std::endl;
    exitCode = EXIT_FAILURE;
}

void LoadIfExists(OCG_Duel duel, const char* script) {
    if (LoadScript(nullptr, duel, script)) {
        std::cout << "Loaded " << script << std::endl;
    } else {
        std::cerr << "Failed to load " << script << std::endl;
    }
}

int main() {
    OCG_DuelOptions config;
    config.cardReader = &GetCard;
    config.scriptReader = &LoadScript;
    config.logHandler = &Log;
    OCG_Duel duel;
    if (OCG_CreateDuel(&duel, config) != OCG_DUEL_CREATION_SUCCESS) {
        std::cerr << "Failed to create duel instance!" << std::endl;
        return EXIT_FAILURE;
    }
    LoadIfExists(duel, "constant.lua");
    LoadIfExists(duel, "utility.lua");
    auto listing = opendir(".");
    if (!listing) {
        std::cerr << "Failed to open current directory!" << std::endl;
        return EXIT_FAILURE;
    }
    for (dirent *entry; (entry = readdir(listing)) != nullptr; ) {
        if (entry->d_type == DT_REG) {
            std::string name(entry->d_name);
            auto length = name.length();
            if (length > 0 && name != "constant.lua" && name != "utility.lua" &&
                name[0] == 'c' && name.rfind(".lua") == length - 4) {
                    OCG_NewCardInfo card;
                    card.team = card.duelist = card.con = 0;
                    card.seq = 1;
                    card.loc = LOCATION_DECK;
                    card.pos = POS_FACEDOWN_ATTACK;
                    card.code = std::stoi(name.substr(1, length - 4));
                    OCG_DuelNewCard(duel, card);
            }
        }
    }
    OCG_DestroyDuel(duel);
    return exitCode;
}
