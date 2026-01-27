#ifndef PROJECT_NULL_CARD_H
#define PROJECT_NULL_CARD_H
#include <string>
#include <memory>
#include <vector>

class ability;

class card {
public:
    card(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked);
    virtual ~card() = default;
    virtual std::unique_ptr<card> clone() const;
    void add_ability(std::shared_ptr<ability> ability_ptr);

    std::string get_id() const { return card_id; };
    std::string get_name() const { return name; };
    std::string get_faction_id() const { return faction_id; };
    std::string get_card_type() const { return card_type; };
    std::string get_rarity() const { return rarity; };
    int get_slots() const { return slots; };
    bool get_is_unlocked() const { return is_unlocked; };
    virtual int get_strength() const { return 0; }
    virtual std::string get_range_type() const { return ""; }
    const std::vector<std::shared_ptr<ability>>& get_abilities() const { return card_abilities; };

    void set_slots(int new_slots) { slots = new_slots; };
    void set_is_unlocked(bool new_value) { is_unlocked = new_value; };

private:
    std::string card_id;
    std::string name;
    std::string faction_id;
    std::string card_type;
    std::string rarity;
    int slots;
    bool is_unlocked;
    std::vector<std::shared_ptr<ability>> card_abilities;

};

#endif //PROJECT_NULL_CARD_H