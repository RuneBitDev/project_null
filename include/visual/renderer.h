#ifndef PROJECT_NULL_RENDERER_H
#define PROJECT_NULL_RENDERER_H
#include "raylib.h"
#include "ui_types.h"
#include "texture_factory.h"
#include "widgets/match/widget_board.h"
#include "widgets/match/widget_deck.h"
#include "widgets/construction/widget_card_pool.h"
#include "widgets/match/widget_game_log.h"
#include "widgets/match/widget_graveyard.h"
#include "widgets/match/widget_hand.h"
#include "widgets/match/widget_player_info.h"
#include "widgets/match/widget_popup.h"
#include "visual/managers/card_manager.h"






class renderer {
public:
    renderer();
    ~renderer();

    void draw_background();

    void draw_start_screen();
    void draw_construction();
    void draw_menu(int p1_idx, int p2_idx, const std::vector<std::string>& factions);


};

#endif //PROJECT_NULL_RENDERER_H