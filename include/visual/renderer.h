#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "texture_factory.h"
#include "core/components/player.h"
#include "widgets/match/widget_board.h"
#include "widgets/match/widget_deck.h"
#include "widgets/deck_builder/widget_deck_grid.h"
#include "widgets/match/widget_game_log.h"
#include "widgets/match/widget_graveyard.h"
#include "widgets/match/widget_hand.h"
#include "widgets/match/widget_player_info.h"
#include "widgets/match/widget_popup.h"


struct render_context {
    const board& b;
    const player& p1;
    const player& p2;
    current_state state;
};

struct all_cards {
    std::vector<card> special_cards;
    std::vector<card_unit> unit_cards;
};

class renderer {
public:
    renderer();
    ~renderer();

    void draw_start_screen();
    void draw_deck_builder(all_cards& cards, int current_deck_idx);
    void draw_menu(int p1_idx, int p2_idx, const std::vector<std::string>& factions);
    void draw_game(const render_context& ctx);

    // widget management
    void add_popup(const std::string& text, Color color, float duration, popup_type p_type);
    void init_deck_builder_widgets();
    void init_match_widgets(const player& p1, const player& p2, texture_factory& tex_factory);
    void init_menu_widgets(const std::vector<std::string>& factions, const std::string& p1_select, const std::string& p2_select);
    bool is_button_triggered(const std::string& id);
    void set_button_enabled(const std::string& id, bool enabled);
    void update_widgets(float dt);
    void update_deck_builder_widgets(float dt);

private:
    std::vector<std::unique_ptr<widget_popup>> active_popups;

    widget_manager manager;

    widget_deck_grid deck_grid_view;

    widget_board board_view;
    widget_hand hand_view;
    widget_graveyard graveyard_view_p1;
    widget_graveyard graveyard_view_p2;
    widget_deck deck_view_p1;
    widget_deck deck_view_p2;
    widget_player_info player_view_p1;
    widget_player_info player_view_p2;
    widget_game_log game_log_view;

};

#endif //PROJECT_NULL_RENDERER_H