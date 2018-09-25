//
// Created by oguz on 24.09.2018.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/dispatcher.hpp>
#include "battleshipplayer.h"
#include "game.cpp"

using namespace eosio;

namespace battleship
{


    class battleship : public contract
    {
    public:

        battleship(account_name self):contract(self)
        {

        }


    };


}