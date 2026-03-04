#ifndef PROJECT_NULL_TEXTURE_FACTORY_H
#define PROJECT_NULL_TEXTURE_FACTORY_H
#include <map>
#include <string>
#include <vector>

#include "raylib.h"


struct asset_info {
    std::string path;
    std::string type;
    std::string card_id;
};

class texture_factory {
public:
    texture_factory();
    ~texture_factory();

    static texture_factory* instance;

    bool initialize_manifest(const std::string& filepath);

    void load_texture(const std::vector<std::string>& asset_ids);
    void load_texture_for_cards(const std::vector<std::string>& card_ids);
    void load_texture_all_ui();
    Texture2D get_texture(const std::string& texture_id);
    Texture2D get_texture_for_card(const std::string& card_id);

    void unload_all();

    void debug_print_manifest();

private:
    std::map<std::string, asset_info> manifest;
    std::map<std::string, Texture2D> texture_map;
};

#endif //PROJECT_NULL_TEXTURE_FACTORY_H