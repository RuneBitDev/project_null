#ifndef PROJECT_NULL_CARD_H
#define PROJECT_NULL_CARD_H
#include <string>
#include <iostream>

class card {
    private:
    std::string card_id;
    std::string name;
    std::string faction_id;
    std::string card_type;
    std::string rarity;
    int slots;
    bool is_unlocked;


    public:
    card(std::string card_id, std::string name, std::string faction_id, std::string card_type,
        std::string rarity, int slots, bool is_unlocked);
    virtual ~card() = default;
    virtual void display() const;

    std::string get_id() const;
    std::string get_name() const;
    std::string get_faction_id() const;
    std::string get_card_type() const;
    std::string get_rarity() const;
    int get_slots() const;
    bool get_is_unlocked() const;

    void set_slots(int new_slots);
    void set_is_unlocked(bool new_value);

};

#endif //PROJECT_NULL_CARD_H