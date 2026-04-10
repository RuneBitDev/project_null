#ifndef PROJECT_NULL_CARD_UNIT_H
#define PROJECT_NULL_CARD_UNIT_H
#include <string>
#include <memory>
#include <tuple>
#include "card.h"
#include "core/types.h"

class board;

enum class stances {PASSIVE, AGGRESSIVE, SUPPRESSIVE, DEFENSIVE, MAX_STANCES};

class card_unit : public card {
public:
    card_unit(std::string card_id, std::string name, std::string faction_id, card_type c_type,
        std::string rarity, bool is_unlocked, int strength, std::string range_type, int armor, int attack);

    std::unique_ptr<card> clone() const override;

    // score calculation
    int get_strength() const override;
    int get_virtual_strength(const board& b, row_side side, row_type type) const;
    void set_strength(int new_strength) {strength = new_strength;}

    // card modifiers
    void save_modifier(modifier_type m_type, int m_value);
    void delete_modifier(modifier_type m_type);

    // stance & attack system
    void change_stance();
    void change_stance_by_value(stances stance) { current_stance = stance; }
    stances get_stance() const {return current_stance;}
    int get_armor() const override;
    void change_armor(int by_amount);
    int get_attack() const override;
    void change_attack(int by_amount);

    // for placement
    std::string get_range_type() const override {return range_type;}
    void set_range_type(const std::string &new_range_type) {range_type = new_range_type;}


private:
    // main values
    int strength;
    int armor;
    int attack;

    // modifier stuff
    std::vector<std::tuple<modifier_type, int>> modifiers;
    int modifier_value = 0;
    int apply_mod_math(int base_value, const std::tuple<modifier_type, int> &modifier) const;

    // stance & attack stuff
    stances current_stance;

    // for placement
    std::string range_type;




};

#endif //PROJECT_NULL_CARD_UNIT_H