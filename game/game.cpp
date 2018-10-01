//
// Created by oguz on 10/1/18.
//

#include "game.h"

void battleship::creategame(account_name host, account_name challenger) {

    require_auth(N(queue));

    _games.emplace(get_self(),[&](game& gm){
        gm.gameid = _games.available_primary_key();
        gm.host.playerid = host;
        gm.host.playertable = vector<string>(100,"0");
        gm.host.enemytable = vector<string>(100,"X");
        gm.challenger.playerid = host;
        gm.challenger.playertable = vector<string>(100,"0");
        gm.challenger.enemytable = vector<string>(100,"X");
    });
}

void battleship::ingame(account_name name)
{
    require_auth(name);
    eosio_assert(!_ingame(name),"User is in the game.");
}

void battleship::removeship(account_name name, string shipName) {

    require_auth(name);
    eosio_assert(Utils::isship(shipName),"Ship is not valid.");
    eosio_assert(_ingame(name),"User is not in game.");
    account_name gameid = getgameid(name);
    auto itr = _games.find(gameid);
    _games.modify(itr,get_self(),[&](game& g){
        eosio_assert(g.started,"Game has started.");
        player& pl = g.getplayer(name);
        g.removeship(pl, shipName);
    });
}

void battleship::makemove(account_name name, uint8_t x, uint8_t y) {

    require_auth(name);
    eosio_assert(_ingame(name),"User is not in a game.");
    eosio_assert(Utils::inmap(x,y),"Invalid x and y.");
    account_name gameid = getgameid(name);
    auto itr = _games.find(gameid);
    bool isFinished = false;
    _games.modify(itr,get_self(),[&](game& g){
        bool isStarted = g.started;
        eosio_assert(!isStarted,"Game is not started.");
        bool moveMade = g.makemove(name,x,y);
        eosio_assert(moveMade,"Given coordinates are occupied.");
        isFinished = g.isfinished(name);
    });

    if(isFinished)
    {
        action(
                permission_level {get_self(),N(active)},
                N(battleship),
                N(finishgame()),
                std::make_tuple(gameid)
        ).send();
    }
}

void battleship::placeship(account_name name, string shipName, uint8_t x, uint8_t y, string direction) {

    require_auth(get_self());
    eosio_assert(_ingame(name),"User is not in a game.");
    eosio_assert(Utils::isship(shipName),"Invalid ship name.");
    eosio_assert(Utils::inmap(x,y),"Coordinates are not in board.");
    eosio_assert(Utils::isdirection(direction),"Invalid direction");
    account_name gameid = getgameid(name);
    auto itr = _games.find(gameid);
    _games.modify(itr,get_self(),[&](game& g){
        bool isStarted = g.started;
        eosio_assert(isStarted,"Game is started.");
        player& pl = g.getplayer(name);
        bool isPlaced = g.placeship(pl, shipName, x, y, direction);
        eosio_assert(isPlaced,"Ship cannot be placed.");
    });
}

void battleship::finishgame(account_name gameid) {

    require_auth(get_self());
    auto itr = _games.find(gameid);
    account_name host,challenger;
    _games.modify(itr,get_self(),[&](game &g) {
        host = g.host.playerid;
        challenger = g.challenger.playerid;
    });
    require_recipient(host);
    require_recipient(challenger);
    _games.erase(itr);
}

void battleship::ready(account_name name) {

    require_auth(name);
    eosio_assert(_ingame(name),"User is not in a game.");
    account_name gameid = getgameid(name);
    auto itr = _games.find(gameid);
    _games.modify(itr,get_self(),[&](game& g){
        player& pl = g.getplayer(name);
        bool canBeReady = canbeready(pl);
        eosio_assert(canBeReady,"Not placed all of the ships.");
        pl.ready = true;
    });

}

void battleship::unready(account_name name) {

    require_auth(name);
    eosio_assert(_ingame(name),"User is not in a game.");
    account_name gameid = getgameid(name);
    auto itr = _games.find(gameid);
    _games.modify(itr,get_self(),[&](game& g){
        player& pl = g.getplayer(name);
        pl.ready = false;
    });
}