#ifndef PROJECT_NULL_BOARD_H
#define PROJECT_NULL_BOARD_H
#include <vector>
#include <array>
#include <map>
#include <functional>
#include "card.h"
#include "card_unit.h"
#include "player.h"




class board {
public:
    board() = default;
    // Card Movement methods
    void add_card(std::unique_ptr<card> c, row_side side, row_type type);
    void destroy_card(const std::vector<card*>& targets, player& p1, player& p2);
    void clear_board(player& p1, player& p2);

    // calculate score
    int calculate_row_score(row_side side, row_type type) const;
    int calculate_total_score(row_side side) const;
    // modifier stuff
    void save_modifiers(row_side side, row_type r_type, modifier_type m_type, int m_value);
    bool is_side_row_modified(const std::tuple<row_side, row_type> &key) const;
    std::vector<std::tuple<modifier_type, int>> get_modifiers(row_side side, row_type type) const;
    void clear_modifier(modifier_type);
    void clear_all_modifiers();
    //strike stuff
    void apply_destroy_effect(value_type v_type, player& p1, player& p2);

    // Getter & Setter
    const std::vector<std::unique_ptr<card>>& get_row_cards(int side, int type) const;
    void for_each_card(const std::function<void(card&)>& action) const; // Visitor Pattern still kinda Getter
    std::string get_row_name(row_type type) const;
    std::vector<card*> get_max_value_cards_on_board(value_type v_type) const;
    std::vector<card*> get_max_value_cards_on_row(value_type v_type, row_side side, row_type r_type) const;




private:
    std::array<std::array<std::vector<std::unique_ptr<card>>, 5>, 2> rows;
    std::vector<std::unique_ptr<card>> active_specials;
    std::map<row_type, bool> active_weather;
    std::map<std::tuple<row_side, row_type>, std::vector<std::tuple<modifier_type, int>>> active_modifiers;
};

#endif //PROJECT_NULL_BOARD_H