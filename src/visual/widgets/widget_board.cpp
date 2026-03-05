#include "visual/widgets/widget_board.h"

#include <cmath>

#include "visual/render_config.h"

widget_board::widget_board() {
    for (int side = 0; side < 2; ++side) {
        for (int type = 0; type < 5; ++type) {
            unit_rows.emplace_back(static_cast<row_side>(side), static_cast<row_type>(type));
        }
    }
}

void widget_board::update_from_game(const board &b, widget_manager& manager) {
    for (auto &row : unit_rows) {
        row.update_row(b, manager);
    }
    total_score_p1 = b.calculate_total_score(row_side::PLAYER);
    total_score_p2 = b.calculate_total_score(row_side::PLAYER);
    special_row.update_from_game(b, manager);
}

void widget_board::update(float dt) {
    for (auto &row : unit_rows) {
        row.update(dt);
    }

    special_row.update(dt);
}

void widget_board::draw() const {
    for (const auto& row : unit_rows) {
        row.draw();
    }
    special_row.draw();

    float layoutHorizonY = render_config::board::START_Y;
    float start_x = render_config::board::BOARD_EDGE_X;
    float end_x = render_config::board::BOARD_EDGE_X2;

    // centerline
    DrawLineEx({start_x, layoutHorizonY}, {end_x, layoutHorizonY}, 3.0f, Fade(WHITE, 0.4f));
    for (int i = 0; i < 20; i++) {
        float dotX = start_x + (i * (end_x - start_x) / 20.0f);
        float pulse = sinf(GetTime() * 2.0f + i) * 0.5f + 0.5f;
        DrawCircleV({dotX, layoutHorizonY}, 3.0f, Fade(WHITE, pulse));
    }


}
