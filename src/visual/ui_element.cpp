#include "visual/ui_element.h"
#include "visual/render_config.h"

bool ui_element::button_rec(Rectangle rec) {

    if (CheckCollisionPointRec(render_config::get_virtual_mouse(), rec)) {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }
    return false;
}

bool ui_element::card_rec(Rectangle rec) {

    if (CheckCollisionPointRec(render_config::get_virtual_mouse(), rec)) {
        return IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
    }

    return false;
}

bool ui_element::button_pass(Rectangle rec) {

    if (IsKeyDown(KEY_SPACE)) {
        hold_timer += GetFrameTime();

        if (hold_timer >= 3.0f) {
            hold_timer = 0.0f;
            return true;
        }
    } else {
        hold_timer = 0.0f;
    }
    return false;
}

void ui_element::update_button(button &btn) {
    Vector2 mouse_pos = render_config::get_virtual_mouse();
    btn.is_hovered = CheckCollisionPointRec(mouse_pos, btn.bounds);
    btn.triggered = false;

    bool mouse_interact = (btn.is_hovered && IsMouseButtonDown(MOUSE_LEFT_BUTTON));
    bool key_interact = (btn.shortcut_key != 0 && IsKeyDown(btn.shortcut_key));

    bool being_pressed = mouse_interact || key_interact;

    if (being_pressed) {
        btn.is_down = true;
        if (btn.type == HOLDABLE) {
            btn.hold_progress += GetFrameTime();
            if (btn.hold_progress >= 1.0f) {
                btn.hold_progress = 1.0f;
                btn.triggered = true;
            }
        }
    } else {
        if (btn.is_down && btn.type == CLICKABLE && (btn.is_hovered || key_interact)) {
            btn.triggered = true;
        }

        btn.is_down = false;
        btn.hold_progress = 0.0f;
    }
}

void ui_element::update_card(ui_card &card) {
    Vector2 mouse_pos = render_config::get_virtual_mouse();
    card.is_hovered = CheckCollisionPointRec(mouse_pos, card.bounds);

    if (card.is_hovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        card.triggered = true;
    }
}
