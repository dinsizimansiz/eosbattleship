//
// Created by oguz on 10/1/18.
//

#ifndef BATTLESHIP_PREGAME_H
#define BATTLESHIP_PREGAME_H

#include <eosiolib/eosio.hpp>
#include "utils.h"

using namespace eosio;
using std::string;
using std::vector;
using std::map;


class battleship : public contract{

public:

    battleship(account_name self) : contract(self),_games(self,self)
    {

    }

    //@abi action
    void creategame(account_name host,account_name challenger);

    //@abi action
    void placeship(account_name name,string shipName,uint8_t x,uint8_t y,string direction);

    //@abi action
    void removeship(account_name name,string shipName);

    //@abi action
    void makemove(account_name name,uint8_t x,uint8_t y);

    //@abi action
    void ingame(account_name name);

    //@abi action
    void ready(account_name name);

    //@abi action
    void unready(account_name name);

    //@abi action
    void finishgame(account_name gameid) ;


private:

    struct player{

        account_name playerid;
        vector<string> playertable;
        vector<string> enemytable;
        bool ready;

        account_name primary_key() const {return playerid;}

        EOSLIB_SERIALIZE(player,(playerid)(playertable)(enemytable))
    };

    //@abi table
    struct game{

        account_name gameid;
        player host;
        player challenger;
        uint8_t round = 0;
        bool started = false;

        account_name primary_key() const {return gameid;}


        bool makemove(account_name name,uint8_t x,uint8_t y)
        {
            uint8_t index = Utils::convertoindex(x,y);
            player& pl = getplayer(name);
            player& opponent = getopponent(name);
            string cellChar = opponent.playertable[index];
            return _makemove(pl,index,cellChar);
        }

        player& getplayer(account_name name)
        {
            if(host.playerid == name)
            {
                return host;
            }
            else if(challenger.playerid == name)
            {
                return challenger;
            }
        }

        player& getopponent(account_name name) {
            if (host.playerid == name)
            {
                return challenger;
            }
            else if (challenger.playerid == name)
            {
                return host;
            }

        }

        bool _makemove(player& player,uint8_t index,string cellchar)
        {
            if(player.enemytable[index] == "X")
            {
                player.enemytable[index] = cellchar;
                round++;
                return true;
            }
            else
            {
                return false;
            }

        }

        void removeship(player &pl, string ship)
        {
            string shipStr = tableMapping[ship];
            for(string& s : pl.playertable)
            {
                if(s == shipStr)
                {
                    s = tableMapping["empty"];
                }

            }
        }

        bool isfinished(account_name name)
        {
            uint8_t counter = 0;
            player& pl = getplayer(name);
            for(const string& s : host.playertable)
            {
                if(s != tableMapping["empty"])
                {
                    counter++;
                }
            }

            return counter == 17;
        }

        bool placeship(player &account, string shipname, uint8_t x, uint8_t y, string direction)
        {
            auto coords = Utils::getcoords(shipname,x,y,direction);
            for(auto vec : coords)
            {
                if(!Utils::inmap(get<0>(vec),get<1>(vec)))
                {
                    return false;
                }
                else if(account.playertable[Utils::convertoindex(get<0>(vec),get<1>(vec))] != "0")
                {
                    return false;
                }
            }

            string shipstr = tableMapping[shipname];

            for(const tuple<uint8_t,uint8_t>& vec : coords)
            {
                uint8_t index = Utils::convertoindex(get<0>(vec),get<0>(vec));
                account.playertable[index] = shipstr;
            }

            return true;

        }

        EOSLIB_SERIALIZE(game,(gameid)(host)(challenger)(round)(started))
    };

    bool _ingame(account_name name)
    {
        for(const game& gm : _games)
        {
            if(gm.host.playerid == name || gm.challenger.playerid == name)
                return true;
        }
        return false;
    }

    account_name getgameid(account_name name)
    {
        for(const game& gm : _games)
        {
            if(gm.host.playerid == name || gm.challenger.playerid == name)
            {
                return gm.gameid;
            }
        }
    }

    bool canbeready(player& pl)
    {
        uint8_t counter = 0;
        for(const string& s : pl.playertable)
        {
            if(s != tableMapping["empty"])
            {
                counter++;
            }
        }

        return counter == 17;
    }

    typedef multi_index<N(games),game> gamebase;

    gamebase _games;

};

EOSIO_ABI(battleship,(creategame)(placeship)(removeship)(makemove)(ingame)(ready)(unready)(finishgame))




#endif //BATTLESHIP_PREGAME_H
