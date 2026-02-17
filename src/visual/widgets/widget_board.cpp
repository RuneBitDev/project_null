#include "visual/widgets/widget_board.h"

widget_board::widget_board() {
    for (int side = 0; side < 2; ++side) {
        for (int type = 0; type < 5; ++type) {
            rows.emplace_back(static_cast<row_side>(side), static_cast<row_type>(type));
        }
    }
}

void widget_board::update_from_game(const board &b) {
    for (auto &row : rows) {
        row.update_row(b);
    }
}

void widget_board::update() {
    // some global animations and shit
}

void widget_board::draw() const {
    for (const auto& row : rows) {
        row.draw();
    }
}
