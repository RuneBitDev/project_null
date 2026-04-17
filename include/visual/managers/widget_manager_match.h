#ifndef PROJECT_NULL_WIDGET_MANAGER_MATCH_H
#define PROJECT_NULL_WIDGET_MANAGER_MATCH_H
#include "visual/texture_factory.h"
#include "visual/managers/widget_manager.h"
#include "visual/widgets/match/widget_board.h"
#include "visual/widgets/match/widget_deck.h"
#include "visual/widgets/match/widget_game_log.h"
#include "visual/widgets/match/widget_graveyard.h"
#include "visual/widgets/match/widget_hand.h"
#include "visual/widgets/match/widget_player_info.h"
#include "visual/widgets/match/widget_popup.h"


struct render_context {
    const board& b;
    const player& p1;
    const player& p2;
    current_state state;
};

class widget_manager_match : public widget_manager {
public:
    widget_manager_match(const player& p1, const player& p2, texture_factory& tex_factory);

    void update(float dt) override;
    void draw(renderer& ren) const override;

    void clear() override;
    void handle_input() override;

    void sync_with_game(const render_context& ctx);
    void add_popup(const std::string& text, Color color, float duration, popup_type p_type);

private:

    std::map<const card*, std::unique_ptr<widget_card>> card_cache;
    std::vector<std::unique_ptr<widget_popup>> active_popups;

    card_manager* c_manager;

    // view containers
    widget_board board_view;
    widget_hand hand_view;
    widget_graveyard graveyard_p1;
    widget_graveyard graveyard_p2;
    widget_deck deck_p1;
    widget_deck deck_p2;
    widget_player_info player_p1;
    widget_player_info player_p2;
    widget_game_log game_log;

};

#endif //PROJECT_NULL_WIDGET_MANAGER_MATCH_H