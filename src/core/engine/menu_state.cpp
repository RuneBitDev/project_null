#include "game/engine/menu_state.h"

#include "game/engine/game_state.h"
#include "game/engine/state_manager.h"

menu_state::menu_state(player p1, player p2)
    : p1(std::move(p1)), p2(std::move(p2)) {}


void menu_state::handle_input(state_manager &manager) {
    if (show_start_screen) {
        if (IsKeyPressed(KEY_ENTER)) show_start_screen = false;
    } else {
        Rectangle startBtn = { (float)GetScreenWidth()/2 - 100, 300, 200, 50};

        if (CheckCollisionPointRec(GetMousePosition(), startBtn)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                manager.change_state(std::make_unique<game_state>(std::move(p1), std::move(p2)));
            }
        }
    }

}

void menu_state::update(float dt) {

}

void menu_state::render(renderer& renderer) {
    if (show_start_screen) {
        renderer.draw_start_screen();
    } else {
        renderer.draw_menu();


    }

}
