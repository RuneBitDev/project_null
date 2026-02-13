#ifndef PROJECT_NULL_COMBAT_MANAGER_H
#define PROJECT_NULL_COMBAT_MANAGER_H
#include "components/card_unit.h"
#include "components/board.h"
#include "components/player.h"


enum class combat_phase { IDLE, PLAYER_ATTACKING, OPPONENT_ATTACKING, CLEANUP };

class combat_manager {
public:
    combat_manager(board& b, player& p1, player& p2);

    void update(float dt);
    void cleanup_dead_units() const;

    void firefight();
    bool is_busy() const { return current_phase != combat_phase::IDLE; }
    void apply_damage_by_value(int dmg, const card_location& target_loc) const;

    board& get_board() { return game_board; }
    player& get_p1() { return p1; }
    player& get_p2() { return p2; }
private:
    void advance_phase();
    void resolve_turn(const player& p) const;
    void apply_damage(const card_unit* attacker, const card_location& target_loc) const;
    card_location find_best_target(row_side enemy_side, row_type attacker_row, stances stance) const;

    board& game_board;
    player& p1;
    player& p2;

    combat_phase current_phase = combat_phase::IDLE;
    bool is_resolving_combat = false;
    float timer = 0.0f;
    const float STEP_DELAY = 1.0f;
};
#endif //PROJECT_NULL_COMBAT_MANAGER_H