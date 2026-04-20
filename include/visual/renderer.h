#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "texture_factory.h"






class renderer {
public:
    renderer();
    ~renderer();

    void draw_background();

    void draw_start_screen();
    void draw_construction();
    void draw_menu(int p1_idx, int p2_idx, const std::vector<std::string>& factions);


};

#endif //PROJECT_NULL_RENDERER_H