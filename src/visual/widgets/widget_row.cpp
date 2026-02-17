#include "visual/widgets/widget_row.h"
#include "visual/layout_manager.h"
#include "visual/render_config.h"

widget_row::widget_row(row_side side, row_type type)
    : side(side), type(type) {
    switch (type) {
        case row_type::MELEE:   label = "MELEE";    break;
        case row_type::RANGED:  label = "RANGED";   break;
        case row_type::HEAVY:   label = "HEAVY";    break;
        case row_type::NET:     label = "NET";      break;
        case row_type::SPECIAL: label = "SPECIAL";  break;
        default:                label = "UNKNOWN";  break;
    }
}

void widget_row::update_row(const board &game_board) {
    const auto& row_cards = game_board.get_row_cards(static_cast<int>(side), static_cast<int>(type));
    int total_cards = static_cast<int>(row_cards.size());

    current_score = game_board.calculate_row_score(side, type);

    card_views.clear();
    for (int i = 0; i < total_cards; i++) {
        card_location loc { side, type, i };
        Rectangle bounds = layout_manager::get_card_bounds(loc, total_cards);

        ui_card state;
        state.face_up = true;

        auto* unit = dynamic_cast<card_unit*>(row_cards[i].get());
        if (unit) {
            state.strength = unit->get_virtual_strength(game_board, side, type);
            state.armor = unit->get_armor();
            state.attack = unit->get_attack();

            switch (unit->get_stance()) {
                case stances::AGGRESSIVE:   state.border_color = RED; break;
                case stances::SUPPRESSIVE:  state.border_color = GREEN; break;
                case stances::DEFENSIVE:    state.border_color = BLUE; break;

            }
        }

        card_views.emplace_back(row_cards[i].get(), bounds, state);
    }
}

void widget_row::update() {
    for (auto& card : card_views) {
        card.update();
    }
}


void widget_row::draw() const {

    DrawRectangleLinesEx(row_bounds, 2, DARKGREEN);

    DrawText(std::to_string(current_score).c_str(), row_bounds.x - 40, row_bounds.y, 20, DARKGREEN);

    for (const auto& card_view : card_views) {
        card_view.draw();
    }

}
