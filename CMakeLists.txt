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
        src/core/components/card.cpp
        src/core/components/deck.cpp
        src/core/components/card_unit.cpp
        src/core/factory.cpp
        src/core/components/board.cpp
        src/core/components/player.cpp
        src/engine/game_state.cpp
        src/engine/deck_state.cpp
        src/engine/menu_state.cpp
        src/engine/state_manager.cpp
        src/visual/renderer.cpp
        src/core/components/abilities/ability_summon.cpp
        src/core/components/abilities/ability_spy.cpp
        src/core/components/abilities/ability_modifier.cpp
        src/visual/layout_manager.cpp
        src/core/match_manager.cpp
        src/core/components/abilities/ability_strike.cpp
        src/core/combat_manager.cpp
        src/core/components/abilities/ability.cpp
        src/core/types.cpp
        src/visual/widgets/match/widget_card.cpp
        src/visual/widgets/match/widget_row.cpp
        src/visual/widgets/match/widget_board.cpp
        src/visual/widgets/match/widget_hand.cpp
        src/visual/widgets/match/widget_special_row.cpp
        src/visual/widgets/match/widget_graveyard.cpp
        src/visual/widget_manager.cpp
        src/visual/widgets/match/widget_deck.cpp
        src/visual/widgets/match/widget_popup.cpp
        src/visual/widgets/match/widget_player_info.cpp
        src/visual/widgets/widget_button.cpp
        src/visual/texture_factory.cpp
        src/visual/widgets/match/widget_game_log.cpp
        src/visual/ui_util.cpp
        src/engine/start_state.cpp
        src/visual/widgets/deck_builder/widget_deck_grid.cpp
)

# ------------------------------------------------------------------
# Link dependencies and set include directories
# ------------------------------------------------------------------

target_link_libraries(project_null PRIVATE sqlite3_lib raylib)
target_include_directories(project_null PRIVATE
        include
        ${raylib_SOURCE_DIR}/src
)