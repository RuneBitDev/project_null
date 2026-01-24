#ifndef PROJECT_NULL_CARD_H
#define PROJECT_NULL_CARD_H

#include <string>
#include <memory>

class card {
public:
    card(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked);
    virtual ~card() = default;
    virtual std::unique_ptr<card> clone() const;
    // virtual bool set_ability(std::shared_ptr<ability> ability);

    std::string get_id() const;
    std::string get_name() const;
    std::string get_faction_id() const;
    std::string get_card_type() const;
    std::string get_rarity() const;
    int get_slots() const;
    bool get_is_unlocked() const;
    virtual int get_strength() const { return 0; }
    virtual std::string get_range_type() const { return ""; }

    void set_slots(int new_slots);
    void set_is_unlocked(bool new_value);

private:
    std::string card_id;
    std::string name;
    std::string faction_id;
    std::string card_type;
    std::string rarity;
    int slots;
    bool is_unlocked;
    // std::vector<std::shared_ptr<ability>> card_abilities;

};

#endif //PROJECT_NULL_CARD_H