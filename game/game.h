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

    //@abi action
    void playertable(account_name playerid);

    //@abi action
    void enemytable(account_name playerid);

    //@abi action
    void bothtables(account_name playerid);

    //@abi action
    void remships(account_name playerid);

    //@abi action
    void curships(account_name playerid);

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

        string printboard(const vector<string>& vec)
        {
            uint8_t counter = 0;
            string retString ;
            retString.append(" ");
            for(uint8_t i = 0; i < 10;i++)
            {
                retString += " ";
                retString.append(std::to_string(i));
            }
            retString += "\n";
            for(uint8_t i = 0;i < 10 ; i++)
            {
                retString.append(std::to_string(i));
                for(uint8_t j = 0;j < 10; j++)
                {
                    retString += " ";
                    retString.append(vec[i*10+j]);
                }
                retString += "\n";
            }
            return retString;
        }
        string everyboard(const vector<string>& playertable,const vector<string>& enemytable)
        {
            uint8_t counter = 0;
            string retString ;
            string space = "\t\t\t\t\t\t";
            retString.append(" ");
            for(uint8_t i = 0; i < 10;i++)
            {
                retString += " ";
                retString.append(std::to_string(i));
                retString += space;
                retString += " ";
                retString.append(std::to_string(i));
            }
            retString += "\n";
            for(uint8_t i = 0;i < 10 ; i++)
            {
                retString.append(std::to_string(i));
                for(uint8_t j = 0;j < 10; j++)
                {
                    retString += " ";
                    retString.append(playertable[i*10+j]);
                    retString += space;
                    retString += " ";
                    retString.append(enemytable[i*10+j]);
                }
                retString += "\n";
            }
            return retString;
        }

        string currentships(const player& player)
        {
            vector<string> allships = {"carrier","battleship","submarine","cruiser","destroyer"};
            string retString;
            for(const string &s : player.playertable)
            {
                if(s == "1" || s == "2" || s == "3" || s == "4" || s == "5")
                {
                    string shipname = reverseTableMapping[s];
                    auto itr = find(allships.begin(),allships.end(),shipname);
                    if(itr != allships.end())
                    {
                        allships.erase(itr);
                    }
                }
            }
            for(const string &s : allships)
            {
                retString += "\n";
                retString += s;
            }
            retString += "\n";
            return retString;
        }

        string remainingships(const player& player)
        {
            string retString;
            vector<string> remships;
            for(const string &s : player.playertable)
            {
                if(s == "1" || s == "2" || s == "3" || s == "4" || s == "5")
                {
                    string shipname = reverseTableMapping[s];
                    auto itr = find(remships.begin(),remships.end(),shipname);
                    remships.emplace_back(shipname);
                }
            }
            for(const string &s : remships)
            {
                retString += "\n";
                retString += s;
            }
            retString += "\n";
            return retString;
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

EOSIO_ABI(battleship,(creategame)(placeship)(removeship)(makemove)(ingame)(ready)(unready)(finishgame)(playertable)(enemytable)(bothtables)(remships)(curships))




#endif //BATTLESHIP_PREGAME_H
