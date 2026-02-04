#ifndef PROJECT_NULL_CARD_UNIT_H
#define PROJECT_NULL_CARD_UNIT_H
#include <string>
#include <memory>
#include <tuple>
#include "card.h"
#include "game/game_config.h"
class board;
enum class row_side;
enum class row_type;

class card_unit : public card {
public:
    card_unit(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, bool is_unlocked, int strength, std::string range_type, int armor, int attack);

    std::unique_ptr<card> clone() const override;
    void set_modifier(bool state, int value); // should save modifiers into a vector probably
    void save_modifier(modifier_type m_type, int m_value);
    void delete_modifier(modifier_type m_type);

    // GETTER & SETTER
    int get_strength() const override;
    int get_virtual_strength(const board& b, row_side side, row_type type) const;
    void set_strength(int new_strength) {strength = new_strength;}
    int get_armor() const override {return armor;}
    void change_armor(int by_amount) {armor += by_amount;}
    int get_attack() const override {return attack;}
    void change_attack(int by_amount) {attack += by_amount;}
    std::string get_range_type() const override {return range_type;}
    void set_range_type(const std::string &new_range_type) {range_type = new_range_type;};

private:
    int strength;
    int armor;
    int attack;
    std::string range_type;
    bool modified = false;
    std::vector<std::tuple<modifier_type, int>> modifiers;
    int modifier_value = 0;

    int apply_mod_math(int base_value, const std::tuple<modifier_type, int> &modifier) const;

};

#endif //PROJECT_NULL_CARD_UNIT_H