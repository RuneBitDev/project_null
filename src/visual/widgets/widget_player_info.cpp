#include "visual/widgets/widget_player_info.h"
#include <cmath>
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

    Color sideColor = (side == row_side::PLAYER) ? GREEN : RED;

    float pulse = (sinf(GetTime() * 1.5f) * 0.5f + 0.5f);
    float pulseOpacity = 0.15f + (pulse * 0.45f);

    // borders
    Color borderColor = is_my_turn ? sideColor : Fade(GRAY, 0.4f);
    float borderThick = is_my_turn ? 3.0f : 1.5f;
    DrawRectangleRec(bounds, Fade(BLACK, 0.8f));
    DrawRectangleLinesEx(bounds, borderThick, Fade(borderColor, pulseOpacity));

    // corners
    float lineLen = 25.0f;
    float x = bounds.x;
    float y = bounds.y;
    float h = bounds.height;
    Color cornerColor = is_my_turn ? sideColor : GRAY;
    DrawLineEx({x, y}, {x + lineLen, y}, 2, cornerColor);
    DrawLineEx({x, y}, {x, y + lineLen}, 2, cornerColor);
    // bottom-left
    DrawLineEx({x, y + h}, {x + lineLen, y + h}, 2, cornerColor);
    DrawLineEx({x, y + h}, {x, y + h - lineLen}, 2, cornerColor);

    float margin = 15.0f;
    float iconSize = 70.0f;
    float iconX, textX;
    iconX = bounds.x + margin;
    textX = iconX + iconSize + 15.0f;

    Rectangle dest = { iconX, bounds.y + (bounds.height / 2) - (iconSize / 2), iconSize, iconSize };
    DrawTexturePro(faction_logo, { 0, 0, static_cast<float>(faction_logo.width), static_cast<float>(faction_logo.height) },
                   dest, { 0, 0 }, 0.0f, WHITE);


    DrawCircleLines(dest.x + iconSize/2, dest.y + iconSize/2, iconSize/2, borderColor);

    float nameY = bounds.y + 20;
    float statsY = nameY + 30;

    DrawText(name.c_str(), textX, nameY, 22, RAYWHITE);
    Color pulseColor = Fade(is_my_turn ? sideColor : GOLD, 0.4f + (pulse * 0.4f));
    DrawRectangleLinesEx({textX - 2, statsY - 2, 45, 22}, 1, Fade(sideColor, 0.3f));
    DrawRectangle(textX + 4, statsY + 4, 10, 14, Fade(sideColor, 0.5f));
    DrawRectangleLines(textX + 2, statsY + 2, 10, 14, pulseColor);
    DrawText(std::to_string(cards_in_hand).c_str(), textX + 18, statsY, 18, GOLD);

    // life gems
    float gemX = textX + 65;
    for (int i = 0; i < 2; i++) {
        bool isAlive = (i < current_lives);
        Color gemColor = isAlive ? RED : DARKGRAY;
        DrawCircle(gemX + (i * 24), statsY + 10, 6, gemColor);
    }

    // sub panel
    if (side == row_side::PLAYER) {
        float stateBarHeight = 30.0f;
        Rectangle stateBar = {
            bounds.x + 5,
            bounds.y + bounds.height - stateBarHeight - 5,
            bounds.width - 10,
            stateBarHeight
        };

        Color stateColor = get_state_color();

        DrawRectangleRec(stateBar, Fade(BLACK, 0.5f));
        DrawRectangleLinesEx(stateBar, 1.0f, Fade(stateColor, 0.6f));

        // cool scanning effect
        float scanX = stateBar.x + (stateBar.width * (sinf(GetTime() * 3.0f) * 0.5f + 0.5f));
        DrawLineEx({scanX, stateBar.y}, {scanX, stateBar.y + stateBar.height}, 2.0f, Fade(stateColor, 0.4f));

        std::string statusText = get_state_string();
        int fontSize = 16;
        int textWidth = MeasureText(statusText.c_str(), fontSize);
        DrawText(statusText.c_str(),
                 stateBar.x + (stateBar.width / 2) - (textWidth / 2),
                 stateBar.y + (stateBar.height / 2) - (fontSize / 2),
                 fontSize, stateColor);
    }
}



std::string widget_player_info::get_state_string() const {
    switch (current_state) {
        case current_state::PLAYER_TURN: return "ACTIVE: USER AUTHORIZED";
        case current_state::AI_TURN:     return "ACTIVE: SYSTEM UPLOAD";
        case current_state::FIREFIGHT:   return "FIREFIGHT";
        case current_state::ROUND_OVER:  return "ROUND TERMINATED";
        case current_state::GAME_OVER:   return "GAME TERMINATED";
        default:                         return "SYSTEM IDLE";
    }
}

Color widget_player_info::get_state_color() const {
    switch (current_state) {
        case current_state::PLAYER_TURN: return LIME;
        case current_state::AI_TURN:     return RED;
        case current_state::FIREFIGHT:   return WHITE;
        case current_state::ROUND_OVER:  return GOLD;
        case current_state::GAME_OVER:   return SKYBLUE;
        default:                         return GRAY;
    }
}