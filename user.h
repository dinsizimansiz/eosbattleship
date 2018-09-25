//
// Created by oguz on 25.09.2018.
//

#ifndef BATTLESHIP_USER_H
#define BATTLESHIP_USER_H

#include <eosiolib/eosio.hpp>
#include "battleshipplayer.h"

struct user
{
    user(account_name owner) : owner(owner) ,bsplayer()
    {
        ;
    }

    account_name owner;
    battleshipplayer bsplayer;
};


struct finish : public std::exception
{
    finish(account_name winner) : winner(winner)
    {

    }

    account_name winner ;
};

#endif //BATTLESHIP_USER_H
