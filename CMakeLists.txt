cmake_minimum_required(VERSION 4.1)
project(project_null)

set(CMAKE_CXX_STANDARD 20)

# ------------------------------------------------------------------
# Compile SQLite as a separate static library
# ------------------------------------------------------------------

add_library(sqlite3_lib STATIC src/sqlite3.c)

# ------------------------------------------------------------------
# Fetch Raylib
# ------------------------------------------------------------------

include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/master.tar.gz
)
FetchContent_MakeAvailable(raylib)

# ------------------------------------------------------------------
# Add the executable target
# ------------------------------------------------------------------

add_executable(project_null
        src/main.cpp
        src/core/Card.cpp
        src/core/Deck.cpp
        src/core/card_unit.cpp
        src/core/factory.cpp
        src/core/board.cpp
        src/core/player.cpp
        src/core/engine/game_state.cpp
        src/core/engine/deck_state.cpp
        src/core/engine/menu_state.cpp
        src/core/engine/state_manager.cpp
)

# ------------------------------------------------------------------
# Link dependencies and set include directories
# ------------------------------------------------------------------

target_link_libraries(project_null PRIVATE sqlite3_lib raylib)
target_include_directories(project_null PRIVATE
        include
        ${raylib_SOURCE_DIR}/src
)