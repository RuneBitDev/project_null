cmake_minimum_required(VERSION 4.1)
project(project_null)

set(CMAKE_CXX_STANDARD 20)

# ------------------------------------------------------------------
# External dependency: nlohmann/json
# ------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT-TAG v3.11.3
)
FetchContent_MakeAvailable(json)

# ------------------------------------------------------------------
# Add the executable target
# ------------------------------------------------------------------

add_executable(project_null src/main.cpp
        include/game/Card.h
        include/game/Deck.h
        src/core/Card.cpp
        src/core/Deck.cpp
        include/game/card_unit.h
        include/game/card_effect.h
        src/core/card_unit.cpp
        src/core/card_effect.cpp
        include/game/factory.h
        src/core/factory.cpp)

# ------------------------------------------------------------------
# Link dependencies and set include directories
# ------------------------------------------------------------------

target_link_libraries(project_null PRIVATE nlohmann_json::nlohmann_json)