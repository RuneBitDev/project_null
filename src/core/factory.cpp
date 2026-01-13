#include "../../include/game/factory.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

#include "../../include/game/card_effect.h"
#include "../../include/game/card_unit.h"

using json = nlohmann::json;


bool factory::load_master_data(const std::string &filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) return false;

    try {
        json j;
        file >> j;

        for (const auto &item : j) {
            std::cout << "Processing ID: " << item.value("id", -1) << std::endl;
            std::string type = item.at("type");

            if (type == "Hero" || type == "Unit") {
                std::string faction = item["faction"].is_null() ? "Neutral" : item.value("faction", "Neutral");

                card_unit ca(
                    item.value("id", 0),
                    item.value("name", "Unknown"),
                    faction,
                    item.value("strength", 0),
                    (type == "Hero")
                );
                unit_library.push_back(ca);
            } else if (item.at("type") == "Effect") {
                card_effect ce(
                    item.value("id", 0),
                    item.value("name", "Unnamed"),
                    item.value("effect", "None")
                    );
                effect_library.push_back(ce);
            }

        }
    } catch (json::parse_error &e) {
        return false;
    }

    return true;
}
