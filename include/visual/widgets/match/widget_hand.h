#ifndef PROJECT_NULL_WIDGET_HAND_H
#define PROJECT_NULL_WIDGET_HAND_H
#include "../widget.h"
#include "../widget_card.h"
#include "core/components/player.h"


class widget_manager_match;

class widget_hand : public widget {
    public:
    void update_from_player(const player& p, widget_manager_match& manager);
    void update_from_player_opponent(const player& p, widget_manager_match& manager);

    void update(float dt) override;
    void draw() const override;

private:
    std::vector<widget_card*> card_view_ptrs;

    Rectangle hand_bounds{};
};

#endif //PROJECT_NULL_WIDGET_HAND_H