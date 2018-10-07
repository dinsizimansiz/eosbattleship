cmake_minimum_required(VERSION 3.12)
project(battleship)

set(CMAKE_CXX_STANDARD 14)
set(SOURCE_FILES main.cpp IPlayerTable.h playertable.cpp playertable.h Coordinate.cpp Coordinate.h Ship.cpp Ship.h ../tests/test_coordinate.cpp)
set(Boost_INCLUDE_DIR //home/oguz/opt/boost/include)
set(Boost_LIBRARY_DIR //home/oguz/opt/boost/lib)
set(EOSIO_CDT /usr/local/eosio.cdt/include/)

include_directories(BEFORE ${EOSIO_CDT} ${Boost_INCLUDE_DIR} ${EXTERNALS})
add_executable(NESE contract.cpp)

link_directories(${Boost_LIBRARY_DIR})




