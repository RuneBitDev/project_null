#include "engine/menu_state.h"
#include "engine/game_state.h"
#include "engine/state_manager.h"
#include "visual/render_config.h"
#include "visual/ui_element.h"

menu_state::menu_state(player p1, player p2)
    : p1(std::move(p1)), p2(std::move(p2)) {
}



void menu_state::handle_input(state_manager &manager) {
    ui_element::update_button(render_config::ui::START_BUTTON);
    if (show_start_screen) {
        if (IsKeyPressed(KEY_ENTER)) show_start_screen = false;
    } else {

        if (render_config::ui::START_BUTTON.triggered) {
            manager.change_state(std::make_unique<game_state>(std::move(p1), std::move(p2)));
        }
    }

}

void menu_state::update(float dt, renderer& renderer) {
    renderer.update_widgets(dt);
}

void menu_state::render(renderer& renderer) {
    if (show_start_screen) {
        renderer.draw_start_screen();
    } else {
        renderer.draw_menu();

    }
}
