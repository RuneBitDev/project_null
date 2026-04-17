#ifndef PROJECT_NULL_WIDGET_DECK_GRID_H
#define PROJECT_NULL_WIDGET_DECK_GRID_H
#include "../widget.h"
#include "raylib.h"
#include "widget_full_card.h"
#include <vector>



class widget_card_pool : public widget {
public:

    widget_card_pool(faction f_id, const std::vector<widget_full_card*>& pool, Rectangle bounds);

    void update(float dt) override;
    void draw() const override;


private:
    faction faction_id;
    std::vector<widget_full_card*> grid_entries;
    Rectangle grid_bounds {};

    float scroll_y = 0.0f;
    float max_scroll = 0.0f;

};

#endif //PROJECT_NULL_WIDGET_DECK_GRID_H