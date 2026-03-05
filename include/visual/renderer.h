#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "layout_manager.h"
#include "texture_factory.h"
#include "../core/components/board.h"
#include "../core/components/player.h"
#include "widgets/widget_board.h"
#include "widgets/widget_deck.h"
#include "widgets/widget_graveyard.h"
#include "widgets/widget_hand.h"
#include "widgets/widget_player_info.h"
#include "widgets/widget_popup.h"

struct render_context {
    const board& b;
    const player& p1;
    const player& p2;
    current_state state;
};

class renderer {
public:
    renderer();
    ~renderer();

    void draw_start_screen();
    void draw_menu(int p1_idx, int p2_idx, const std::vector<std::string>& factions);
    void draw_game(const render_context& ctx);

    // widget management
    void add_popup(const std::string& text, Color color, float duration, popup_type p_type);
    void init_match_widgets(const player& p1, const player& p2, texture_factory& tex_factory);
    void init_menu_widgets(const std::vector<std::string>& factions, const std::string& p1_select, const std::string& p2_select);
    bool is_button_triggered(const std::string& id);
    void set_button_enabled(const std::string& id, bool enabled);
    void update_widgets(float dt);

private:
    std::vector<std::unique_ptr<widget_popup>> active_popups;

    widget_manager manager;

    widget_board board_view;
    widget_hand hand_view;
    widget_graveyard graveyard_view_p1;
    widget_graveyard graveyard_view_p2;
    widget_deck deck_view_p1;
    widget_deck deck_view_p2;
    widget_player_info player_view_p1;
    widget_player_info player_view_p2;

    Font main_font;

    void draw_text_cyber(const Font &font, const char* text, Vector2 pos, float size, Color mainColor);
    void draw_text_centered(const char* text, int y, int size, Color color);
    void draw_text_in_rect(const char* text, Rectangle rect, int y_offset, int size, Color color);
};

#endif //PROJECT_NULL_RENDERER_H