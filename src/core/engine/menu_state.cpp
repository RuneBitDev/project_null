#include "game/engine/menu_state.h"
#include "game/engine/state_manager.h"
#include "visual/renderer.h"


void menu_state::handle_input(state_manager &manager) {
    if (IsKeyPressed(KEY_ENTER)) {
        show_start_screen = false;
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
