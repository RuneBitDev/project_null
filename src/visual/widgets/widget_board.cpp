#include "visual/widgets/widget_board.h"

widget_board::widget_board() {
    for (int side = 0; side < 2; ++side) {
        for (int type = 0; type < 5; ++type) {
            unit_rows.emplace_back(static_cast<row_side>(side), static_cast<row_type>(type));
        }
    }
}

void widget_board::update_from_game(const board &b) {
    for (auto &row : unit_rows) {
        row.update_row(b);
    }

    special_row.update_from_game(b);
}

void widget_board::update() {
    for (auto &row : unit_rows) {
        row.update();
    }

    special_row.update();
}

void widget_board::draw() const {
    for (const auto& row : unit_rows) {
        row.draw();
    }
    special_row.draw();
}
