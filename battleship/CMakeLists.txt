cmake_minimum_required(VERSION 3.12)
project(.)

set(CMAKE_CXX_STANDARD 14)


set(SOURCE_FILES main.cpp IPlayerTable.h playertable.cpp playertable.h coordinate.cpp coordinate.h ship.cpp ship.h ../tests/test_coordinate.cpp)
set(Boost_INCLUDE_DIR //home/oguz/opt/boost/include)
set(Boost_LIBRARY_DIR //home/oguz/opt/boost/lib)
set(CONTRACTS  //home/oguz/Applications/eos/contracts)
set(EXTERNALS //home/oguz/Applications/eos/externals/magic_get/include/)
set(EOSIOLIB ${CONTRACTS}/eosiolib)

include_directories(BEFORE ${CONTRACTS} ${Boost_INCLUDE_DIR} ${EXTERNALS})

add_subdirectory(../tests)
link_directories(${Boost_LIBRARY_DIR})
add_executable(${SOURCE_FILES} IEnemyTable.h ../tests/test_playertable.cpp enemytable.cpp enemytable.h ../tests/test_enemytable.cpp battleshipplayer.cpp battleshipplayer.h ../battleship.cpp user.h TableMapping.h)
