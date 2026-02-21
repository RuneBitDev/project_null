#ifndef PROJECT_NULL_WIDGET_GRAVEYARD_H
#define PROJECT_NULL_WIDGET_GRAVEYARD_H
#include "widget.h"
#include "widget_card.h"
#include "core/components/player.h"
#include "visual/render_config.h"

class widget_graveyard : public widget {
    public:
    widget_graveyard() = default;

    void update_from_player(const player& player);
    void update() override;
    void draw() const override;

private:
    std::vector<widget_card> graveyard_views;

    Rectangle graveyard_bounds;
};

#endif //PROJECT_NULL_WIDGET_GRAVEYARD_H