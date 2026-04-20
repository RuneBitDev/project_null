#include "visual/managers/widget_manager_match.h"

widget_manager_match::widget_manager_match(const player& p1, const player& p2, texture_factory& tex_factory) {
    clear_buttons();
    manage_button("PASS", "PASS", HOLDABLE, KEY_SPACE, render_config::buttons::PASS_BUTTON);

    // init specialized views (positions only)
    deck_p1.init_deck(row_side::PLAYER);
    deck_p2.init_deck(row_side::OPPONENT);
    player_p1.init_player_info(row_side::PLAYER);
    player_p2.init_player_info(row_side::OPPONENT);

    //  populate card cache from both players
    auto spawn_cards = [&](const player& p, row_side side) {
        Rectangle deck_pos = (side == row_side::PLAYER) ?
                             render_config::deck::DECK_PLAYER :
                             render_config::deck::DECK_OPPONENT;

        for (card* c : p.get_deck().get_card_ptrs()) {
            card_context ctx;
            ctx.card_bounds = deck_pos;
            ctx.position = card_position::DECK;
            ctx.face_up = false;

            widget_card* w = manage_card_widget(c, ctx);

            w->set_card_texture(tex_factory.get_texture_for_card(c->get_id()));
            w->set_bounds(deck_pos);
        }
    };

    spawn_cards(p1, row_side::PLAYER);
    spawn_cards(p2, row_side::OPPONENT);
}

void widget_manager_match::update(float dt) {
    update_buttons(dt);

    for (auto& [key, value] : card_cache) {
        value.update(dt);
    }

    board_view.update(dt);
    hand_view.update(dt);
    graveyard_p1.update(dt);
    graveyard_p2.update(dt);
    deck_p1.update(dt);
    deck_p2.update(dt);
    player_p1.update(dt);
    player_p2.update(dt);
    game_log.update(dt);
}

void widget_manager_match::sync_with_game(const render_context& ctx) {

    board_view.update_from_game(ctx.b, *this);
    hand_view.update_from_player(ctx.p1, *this);
    hand_view.update_from_player_opponent(ctx.p2, *this);
    graveyard_p1.update_from_player(ctx.p1, *this);
    graveyard_p2.update_from_player(ctx.p2, *this);
    deck_p1.update_from_deck(ctx.p1.get_deck(), *this);
    deck_p2.update_from_deck(ctx.p2.get_deck(), *this);

    player_context p_ctx{
        ctx.state,
        ctx.b.calculate_total_score(row_side::PLAYER),
        ctx.b.calculate_total_score(row_side::OPPONENT)
    };
    player_p1.update_from_player(ctx.p1, p_ctx);
    player_p2.update_from_player(ctx.p2, p_ctx);
}


void widget_manager_match::draw(renderer& ren) const {

    board_view.draw();
    hand_view.draw();
    graveyard_p1.draw();
    graveyard_p2.draw();
    deck_p1.draw();
    deck_p2.draw();
    player_p1.draw();
    player_p2.draw();
    game_log.draw();

    draw_buttons();

    // draw cards
    for (const auto& [key, value] : card_cache) {
        value.draw();
    }
}

void widget_manager_match::clear() {
    clear_buttons();
    clear_card_widgets();
}

void widget_manager_match::handle_input() {
}


void widget_manager_match::add_popup(const std::string &text, Color color, float duration, popup_type p_type) {
    active_popups.push_back(std::make_unique<widget_popup>(text, color, duration, p_type));
}

widget_card* widget_manager_match::manage_card_widget(const card* card_ptr, card_context card_ctx) {
    auto it = card_cache.find(card_ptr);
    if (it == card_cache.end()) {
        auto result = card_cache.emplace(card_ptr, widget_card(card_ptr, card_ctx));
        return &result.first->second;
    }

    it->second.sync_card_context(card_ctx);
    return &it->second;
}

void widget_manager_match::clear_card_widgets() {
    card_cache.clear();
}

