#include "visual/texture_factory.h"
#include "sqlite3.h"
#include <iostream>

texture_factory::texture_factory() {
}

texture_factory::~texture_factory() {
}

bool texture_factory::initialize_manifest(const std::string& filepath) {
    sqlite3* db;

    std::cout << "\n=======================[TEXTURE FACTORY START]=========================" << std::endl;

    if (sqlite3_open(filepath.c_str(), &db) != SQLITE_OK) {
        std::cerr << "Failed to open DB: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }

    const char* sql =
        "SELECT a.asset_id, a.filepath, a.asset_type, ca.card_id "
        "FROM assets a "
        "LEFT JOIN card_assets ca ON a.asset_id = ca.asset_id";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) == SQLITE_OK) {
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            const char* id   = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            const char* path = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            const char* type = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            const char* c_id = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            if (id && path) {
                asset_info info;
                info.path =     path;
                info.type =     type ? type : "UNKNOWN";
                info.card_id =  c_id ? c_id : "";

                manifest[id] = info;

            }
        }
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);

    TraceLog(LOG_INFO, "TEXTURE: Manifest loaded with %d assets", static_cast<int>(manifest.size()));
    return true;
}

void texture_factory::load_texture(const std::vector<std::string> &asset_ids) {
    for (const auto& id : asset_ids) {
        if (texture_map.find(id) != texture_map.end()) continue;

        auto it = manifest.find(id);
        if (it != manifest.end()) {
            Texture2D tex = LoadTexture(it->second.path.c_str());

            if (tex.id > 0) {
                texture_map[id] = tex;
            }
        }
    }
}

void texture_factory::load_texture_for_cards(const std::vector<std::string> &card_ids) {
    std::vector<std::string> assets_to_load;

    for (const auto& c_id : card_ids) {
        for (auto const& [asset_id, info] : manifest) {
            if (info.card_id == c_id) {
                assets_to_load.push_back(asset_id);
            }
        }
    }

    load_texture(assets_to_load);
}

void texture_factory::unload_all() {
    for (auto& [id, texture] : texture_map) {
        UnloadTexture(texture);
    }
    texture_map.clear();
    TraceLog(LOG_INFO, "TEXTURE: All textures unloaded from VRAM");
}
