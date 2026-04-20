#include <utility>
#include "visual/widgets/construction/widget_card_pool.h"
#include <cmath>

widget_card_pool::widget_card_pool(const faction f_id, const std::vector<widget_card*>& pool, Rectangle bounds)
    : faction_id(f_id), grid_entries(pool), grid_bounds(bounds) {

    float padding = 30.0f;
    float cardW = 200.0f;
    float cardH = 300.0f;
    int cols = 5;

    for (size_t i = 0; i < pool.size(); ++i) {
        int column = i % cols;
        int row = static_cast<int>(i / cols);

        auto w = grid_entries[i];

        bounds = {
            grid_bounds.x + 20 + (column * (cardW + padding)),
            grid_bounds.y + 20 + (row * (cardH + padding)),
            cardW,
            cardH
        };

        w->set_bounds(bounds);

        float total_content_height = (row + 1) * (cardH + padding) + 40;
        max_scroll = fmaxf(0, total_content_height - grid_bounds.height);
    }

}


void widget_card_pool::update(float dt) {
    Vector2 mousePos = GetMousePosition();

    if (CheckCollisionPointRec(mousePos, grid_bounds)) {
        float wheel = GetMouseWheelMove();
        if (wheel != 0) {
            scroll_y += wheel * 80.0f;
            if (scroll_y > 0) scroll_y = 0;
            if (scroll_y < -max_scroll) scroll_y = -max_scroll;
        }
    }

    // update every card's internal state with the scroll offset
    for (auto& entry : grid_entries) {
        Rectangle b = entry->get_base_bounds();
        entry->sync_card_context({
            { b.x, b.y + scroll_y, b.width, b.height },
            card_position::POOL,
            card_detail::MAX
        });
        entry->update(dt);
    }
}

void widget_card_pool::draw() const {

    DrawRectangleRec(grid_bounds, { 10, 10, 20, 200 });
    DrawRectangleLinesEx(grid_bounds, 2.0f, Fade(GREEN, 0.3f));

    // corners
    DrawRectangle(grid_bounds.x - 5, grid_bounds.y - 5, 20, 2, GREEN);
    DrawRectangle(grid_bounds.x - 5, grid_bounds.y - 5, 2, 20, GREEN);

    for (int i = 0; i < grid_bounds.height; i += 4) {
        DrawLine(grid_bounds.x, grid_bounds.y + i,
                 grid_bounds.x + grid_bounds.width, grid_bounds.y + i,
                 Fade(GREEN, 0.02f));
    }

    BeginScissorMode(grid_bounds.x, grid_bounds.y, grid_bounds.width, grid_bounds.height);

    for (const auto& entry : grid_entries) {
        // current_bounds ALREADY contains scroll offset from sync_card_context in update()
        Rectangle b = entry->get_current_bounds();

        if (b.y + b.height > grid_bounds.y && b.y < grid_bounds.y + grid_bounds.height) {
            entry->draw_at(b);
        }
    }

    EndScissorMode();

    // scrollbar indicator
    if (max_scroll > 0) {
        float scrollbar_h = (grid_bounds.height / (max_scroll + grid_bounds.height)) * grid_bounds.height;
        float scrollbar_y = grid_bounds.y + (-scroll_y / max_scroll) * (grid_bounds.height - scrollbar_h);
        DrawRectangle(grid_bounds.x + grid_bounds.width - 8, scrollbar_y, 4, scrollbar_h, Fade(GREEN, 0.6f));
    }

}
