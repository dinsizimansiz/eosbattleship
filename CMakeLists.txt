cmake_minimum_required(VERSION 3.12)
project(battleship)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES main.cpp IPlayerTable.h playertable.cpp playertable.h coordinate.cpp coordinate.h ship.cpp ship.h tests/test_coordinate.cpp)


include_directories(BEFORE ${CMAKE_CURRENT_BINARY_DIR}/../../eos/contracts)
include_directories(AFTER ../../boost_1_66_0/include)

add_executable(${SOURCE_FILES} IEnemyTable.h tests/test_playertable.cpp enemytable.cpp enemytable.h tests/test_enemytable.cpp battleshipplayer.cpp battleshipplayer.h battleship.cpp user.h Finish.cpp TableMapping.h)
add_executable(scratch /home/oguz/.CLion2018.2/config/scratches/scratch.cpp)
add_executable(test_coordinate tests/test_coordinate.cpp)
add_executable(test_ship tests/test_ship.cpp)
add_executable(test_playertable tests/test_playertable.cpp)
add_executable(test_enemytable tests/test_enemytable.cpp)
add_executable(test_battleshipplayer tests/test_battleshipplayer.cpp)
add_executable(test_game tests/test_game.cpp)