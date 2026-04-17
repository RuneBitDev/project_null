#ifndef PROJECT_NULL_WIDGET_PLAYER_INFO_H
#define PROJECT_NULL_WIDGET_PLAYER_INFO_H
#include "../widget.h"
#include "core/components/player.h"
#include "visual/ui_types.h"
#include "../../managers/card_manager.h"

class widget_player_info : public widget {
public:
    void init_player_info(row_side side);
    void update_from_player(const player& player, const player_context& p_ctx);

    void update(float dt) override;
    void draw() const override;

private:
    Rectangle bounds{};
    Texture2D faction_logo = {};

    row_side side {};
    current_state c_state {};

    std::string name;
    int cards_in_hand = 0;
    int current_lives = 0;
    int total_score_p1 = 0;
    int total_score_p2 = 0;

    std::string get_state_string() const;
    Color get_state_color() const;

};
#endif //PROJECT_NULL_WIDGET_PLAYER_INFO_H