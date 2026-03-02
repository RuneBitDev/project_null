#ifndef PROJECT_NULL_WIDGET_PLAYER_INFO_H
#define PROJECT_NULL_WIDGET_PLAYER_INFO_H
#include "widget.h"
#include "core/components/player.h"
#include "visual/ui_types.h"
#include "visual/widget_manager.h"

class widget_player_info : public widget {
public:
    void init_player_info(row_side side);
    void update_from_player(const player& player, const player_context& p_ctx);

    void update(float dt) override;
    void draw() const override;

private:
    Rectangle bounds;
    Texture2D faction_logo;

    row_side side;
    current_state current_state;

    std::string name;
    int cards_in_hand;
    int current_lives = 0;

};
#endif //PROJECT_NULL_WIDGET_PLAYER_INFO_H