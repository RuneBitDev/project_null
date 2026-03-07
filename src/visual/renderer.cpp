#include "visual/renderer.h"
#include "visual/render_config.h"
#include "visual/ui_util.h"
#include <cmath>



renderer::renderer() {
    ui::init();
}

renderer::~renderer() {
    ui::unload();
}

void renderer::draw_start_screen() {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < render_config::VIRTUAL_HEIGHT; i += 4) {
        DrawLine(0, i, render_config::VIRTUAL_WIDTH, i, Fade(RAYWHITE, 0.03f));
    }

    ui::draw_text_header("PROJECT NULL", 300, 80, GREEN);
    ui::draw_text_header("SYSTEM INITIALIZED", 450, 30, Fade(GREEN, 0.6f));

    float alpha = (sinf(GetTime() * 4.0f) * 0.5f + 0.5f);
    ui::draw_text_header(">> PRESS ENTER <<", 900, 40, Fade(GREEN, alpha));
}

void renderer::draw_menu(int p1_idx, int p2_idx, const std::vector<std::string>& factions) {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < 1440; i += 5) {
        DrawLine(0, i, 2560, i, Fade(RAYWHITE, 0.05f));
    }

    float centerX = render_config::VIRTUAL_WIDTH / 2.0f;

    auto draw_selection_slot = [&](int idx, float y, Color theme, const char* title) {
        Texture2D tex = texture_factory::instance->get_texture("selection_" + factions[idx]);
        Rectangle dest = { centerX - 600, y, 1200, 400 };


        if (tex.id > 0) {
            DrawTexturePro(tex, {0, 0, static_cast<float>(tex.width), static_cast<float>(tex.height)}, dest, {0,0}, 0.0f, WHITE);
        }

        // cyberpunk frame
        DrawRectangleLinesEx(dest, 2.0f, Fade(theme, 0.5f));
        DrawRectangle(dest.x, dest.y - 40, 300, 40, theme);
        DrawText(title, dest.x + 10, dest.y - 35, 25, BLACK);

        std::string name = factions[idx];
        for(auto & c: name) c = toupper(c);
        DrawText(name.c_str(), dest.x + 10, dest.y + dest.height + 10, 40, theme);
    };

    draw_selection_slot(p2_idx, 180.0f, RED, "HOSTILE_ID");
    draw_selection_slot(p1_idx, 860.0f, GREEN, "OPERATIVE_USER");

    // central divider
    DrawLineEx({0, 720}, {centerX - 300, 720}, 2.0f, Fade(GREEN, 0.3f));
    DrawLineEx({centerX + 300, 720}, {2560, 720}, 2.0f, Fade(RED, 0.3f));

    manager.draw_buttons();
}

void renderer::draw_game(const render_context& ctx) {
    ClearBackground({ 10, 10, 15, 255 });
    for (int i = 0; i < 1440; i += 5) {
        DrawLine(0, i, 2560, i, Fade(RAYWHITE, 0.05f));
    }

    board_view.update_from_game(ctx.b, manager);
    hand_view.update_from_player(ctx.p1, manager);
    hand_view.update_from_player_opponent(ctx.p2, manager);
    graveyard_view_p1.update_from_player(ctx.p1, manager);
    graveyard_view_p2.update_from_player(ctx.p2, manager);
    deck_view_p1.update_from_deck(ctx.p1.get_deck(), manager);
    deck_view_p2.update_from_deck(ctx.p2.get_deck(), manager);

    player_context p_ctx{
        ctx.state,
        ctx.b.calculate_total_score(row_side::PLAYER),
        ctx.b.calculate_total_score(row_side::OPPONENT)
    };
    player_view_p1.update_from_player(ctx.p1, p_ctx);
    player_view_p2.update_from_player(ctx.p2, p_ctx);

    board_view.draw();
    hand_view.draw();
    graveyard_view_p1.draw();
    graveyard_view_p2.draw();
    deck_view_p1.draw();
    deck_view_p2.draw();
    player_view_p1.draw();
    player_view_p2.draw();
    game_log_view.draw();

    manager.draw_card_widgets();
    manager.draw_buttons();


    for (const auto& p : active_popups) {
        p->draw();
    }

}


// ------------------------ WIDGET MANAGEMENT ------------------------


void renderer::add_popup(const std::string& text, Color color, float duration, popup_type p_type) {
    active_popups.push_back(std::make_unique<widget_popup>(text, color, duration, p_type));
}

void renderer::init_match_widgets(const player& p1, const player& p2, texture_factory& tex_factory) {
    manager.clear_card_widgets();
    manager.clear_button_widgets();

    // initialize the deck view positions
    deck_view_p1.init_deck(row_side::PLAYER);
    deck_view_p2.init_deck(row_side::OPPONENT);

    player_view_p1.init_player_info(row_side::PLAYER);
    player_view_p2.init_player_info(row_side::OPPONENT);

    manager.manage_button_widget("PASS", "PASS",HOLDABLE, KEY_SPACE, render_config::buttons::PASS_BUTTON);


    auto spawn_cards = [&](const player& p, row_side side) {
        Rectangle deck_pos = (side == row_side::PLAYER) ?
                             render_config::deck::DECK_PLAYER :
                             render_config::deck::DECK_OPPONENT;

        for (card* c : p.get_deck().get_card_ptrs()) {
            card_context ctx;
            ctx.card_bounds = deck_pos;
            ctx.position = card_position::DECK;
            ctx.face_up = false;

            widget_card* w = manager.manage_card_widget(c, ctx);

            w->set_card_texture(tex_factory.get_texture_for_card(c->get_id()));
            w->set_bounds(deck_pos);
        }
    };

    spawn_cards(p1, row_side::PLAYER);
    spawn_cards(p2, row_side::OPPONENT);
}

void renderer::init_menu_widgets(const std::vector<std::string>& factions, const std::string& p1_select, const std::string& p2_select) {
    manager.clear_button_widgets();

    float centerX = render_config::VIRTUAL_WIDTH / 2.0f;

    float imgWidth = 1200.0f;
    float imgHeight = 400.0f;
    float arrowW = 80.0f;
    float arrowH = 120.0f;

    // p2 top section
    float p2_y = 180.0f;
    Rectangle p2_rect = { centerX - (imgWidth / 2.0f), p2_y, imgWidth, imgHeight };
    manager.manage_button_widget("P2_PREV", "<", CLICKABLE, 0, { p2_rect.x - arrowW - 40, p2_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });
    manager.manage_button_widget("P2_NEXT", ">", CLICKABLE, 0, { p2_rect.x + imgWidth + 40, p2_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });

    // p1 bottom section
    float p1_y = 860.0f;
    Rectangle p1_rect = { centerX - (imgWidth / 2.0f), p1_y, imgWidth, imgHeight };
    manager.manage_button_widget("P1_PREV", "<", CLICKABLE, 0, { p1_rect.x - arrowW - 40, p1_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });
    manager.manage_button_widget("P1_NEXT", ">", CLICKABLE, 0, { p1_rect.x + imgWidth + 40, p1_y + (imgHeight/2 - arrowH/2), arrowW, arrowH });

    Rectangle start_rect = { centerX - 250, 670, 500, 100 };
    manager.manage_button_widget("START", "INITIALIZE OPERATION", CLICKABLE, 0, start_rect);
}

bool renderer::is_button_triggered(const std::string& id) {
    auto* btn = manager.get_button(id);
    return btn->is_triggered();
}

void renderer::set_button_enabled(const std::string& id, bool enabled) {
    auto* btn = manager.get_button(id);
    if (btn) btn->set_enabled(enabled);
}

void renderer::update_widgets(float dt) {

    manager.update(dt);

    board_view.update(dt);
    hand_view.update(dt);
    graveyard_view_p1.update(dt);
    graveyard_view_p2.update(dt);
    deck_view_p1.update(dt);
    deck_view_p2.update(dt);
    player_view_p1.update(dt);
    player_view_p2.update(dt);
    game_log_view.update(dt);

    for (auto it = active_popups.begin(); it != active_popups.end();) {
        (*it)->update(dt);
        if ((*it)->is_finished()) it = active_popups.erase(it);
        else ++it;
    }
}