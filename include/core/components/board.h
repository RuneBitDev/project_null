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
    void remove_card_at(card_location loc, player& p);
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


    // Getter & Setter
    const std::vector<std::unique_ptr<card>>& get_row_cards(row_side side, row_type type) const;
    std::vector<std::unique_ptr<card>>& get_row_cards(row_side side, row_type type); // for combat (write access)
    void for_each_card(const std::function<void(card&)>& action) const; // Visitor Pattern still kinda Getter
    std::string get_row_name(row_type type) const;
    std::vector<card_location> get_max_value_locations_on_board(value_type v_type) const;
    std::vector<card_location> get_max_value_locations_on_side(value_type v_type, row_side side) const;
    std::vector<card_location> get_max_value_locations_on_row(value_type v_type, row_side side, row_type r_type) const;
    std::vector<int> get_dead_unit_indices(row_side side, row_type type) const;




private:
    std::array<std::array<std::vector<std::unique_ptr<card>>, 5>, 2> rows;
    std::map<std::tuple<row_side, row_type>, std::vector<std::tuple<modifier_type, int>>> active_modifiers;
};

#endif //PROJECT_NULL_BOARD_H