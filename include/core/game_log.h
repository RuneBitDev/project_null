#ifndef PROJECT_NULL_GAME_LOG_H
#define PROJECT_NULL_GAME_LOG_H
#include <string>
#include <utility>
#include <vector>

#include "raylib.h"

struct log_entry {
    std::string message;
    Color color;
};

class game_log {
    public:
    static inline std::vector<log_entry> log;
    static void add(std::string msg, Color col = RAYWHITE) {
        log.push_back(log_entry{ std::move(msg), col });
        if (log.size() > 50) log.erase(log.begin());
    }
};

#endif //PROJECT_NULL_GAME_LOG_H