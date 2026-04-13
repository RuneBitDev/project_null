#ifndef PROJECT_NULL_WIDGET_DECK_GRID_H
#define PROJECT_NULL_WIDGET_DECK_GRID_H
#include "../widget.h"
#include "raylib.h"
#include "widget_full_card.h"
#include <vector>



class widget_card_pool : public widget {
public:

    widget_card_pool(std::string faction_id, const std::vector<card*>& pool, Rectangle bounds);

    void update(float dt) override;
    void draw() const override;


private:
    std::string faction_id;
    std::vector<std::unique_ptr<widget_full_card>> grid_entries;
    Rectangle grid_bounds {};

    float scroll_y = 0.0f;
    float max_scroll = 0.0f;

};

#endif //PROJECT_NULL_WIDGET_DECK_GRID_H