#include "visual/widgets/widget_player_info.h"

#include "visual/render_config.h"

void widget_player_info::init_player_info(row_side side) {
    this->side = side;
    // this->faction_logo = logo;
    float y = render_config::board::BOARD_Y_CENTER;
    switch (side) {
        case row_side::PLAYER:      y += 150; break;
        case row_side::OPPONENT:    y -= 350; break;
    }
    bounds = {0, y, render_config::board::BOARD_EDGE_X, 200};
}


void widget_player_info::update_from_player(const player &player, const player_context& p_ctx) {
    cards_in_hand = player.get_hand().size();
    current_lives = player.get_lives();
    name = player.get_name();

    current_state = p_ctx.current_state;

}

void widget_player_info::update(float dt) {

}

void widget_player_info::draw() const {

    bool is_my_turn = (side == row_side::PLAYER && current_state == current_state::PLAYER_TURN) ||
                      (side == row_side::OPPONENT && current_state == current_state::AI_TURN);

    Color borderColor = is_my_turn ? ORANGE : Fade(GRAY, 0.3f);
    float borderThick = is_my_turn ? 4.0f : 2.0f;

    DrawRectangleRec(bounds, Fade(BLACK, 0.7f));
    DrawRectangleLinesEx(bounds, borderThick, borderColor);

    float margin = 15.0f;
    float iconSize = 70.0f;
    float iconX, textX;

    if (side == row_side::PLAYER) {
        iconX = bounds.x + margin;
        textX = iconX + iconSize + 15.0f;
    } else {
        iconX = bounds.x + bounds.width - iconSize - margin;
        textX = bounds.x + margin;
    }

    Rectangle dest = { iconX, bounds.y + (bounds.height / 2) - (iconSize / 2), iconSize, iconSize };
    DrawTexturePro(faction_logo, { 0, 0, (float)faction_logo.width, (float)faction_logo.height },
                   dest, { 0, 0 }, 0.0f, WHITE);


    DrawCircleLines(dest.x + iconSize/2, dest.y + iconSize/2, iconSize/2, borderColor);


    float nameY = bounds.y + 20;
    float statsY = nameY + 30;

    DrawText(name.c_str(), textX, nameY, 22, RAYWHITE);

    DrawRectangle(textX, statsY, 12, 18, LIGHTGRAY); // Simple Card Silhouette
    DrawText(std::to_string(cards_in_hand).c_str(), textX + 18, statsY, 18, GOLD);

    float gemX = textX + 50;
    for (int i = 0; i < 2; i++) {
        Color gemColor = (i < current_lives) ? RED : DARKGRAY;
        DrawCircle(gemX + (i * 22), statsY + 10, 6, gemColor);
        DrawCircleLines(gemX + (i * 22), statsY + 10, 6, GOLD);
    }
}