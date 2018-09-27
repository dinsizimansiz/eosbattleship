//
// Created by oguz on 25.09.2018.
//


#include "user.h"

#include<eosiolib/eosio.hpp>

//TODO Game beginning will be handled by a higher degree class.
//TODO Turns will be handled by higher degree class.
//TODO If coordinate is outside the map , then handle it at a higher degree class.

#define MAX_NUMBER_OF_ATTACKS 17

class [[eosio::table]] gameproto
{
public:

    gameproto(account_name gameid,account_name host,account_name competitor)
    :host(host),competitor(competitor),finished(false),started(false),turn(0)
    {
        this->gameid = gameid;
    }



    account_name primary_key() const
    {
        return gameid;
    }

    account_name host_key() const
    {
        return host.owner;
    }

    account_name competitor_key() const
    {
        return competitor.owner;
    }

    bool is_turn(account_name name)
    {
        if(name == host.owner && ! turn % 2  )
        {
            return true;
        }
        else if(name == competitor.owner && turn % 2)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    user& get_player(account_name name)
    {
        if(name == host.owner)
        {
            return host;
        }
        else if(name == competitor.owner)
        {
            return competitor;
        }
    }

    user& get_opponent(account_name name)
    {
        if(name == host.owner)
        {
            return competitor;
        }
        else if(name == competitor.owner)
        {
            return host;
        }
    }

    bool place_ship(account_name name,std::string shipName,coordinate coordinate1,direction direction1)
    {
        user &player = get_player(name);
        ship s = ship(shipName);
        bool returnValue = player.bsplayer.placeShip(s,coordinate1,direction1);
        return returnValue;
    }

    std::string get_remaining_ships(account_name name )
    {
        user &player = get_player(name);
        std::vector<ship> shipYard = player.bsplayer.getRemainingShips();
        std::stringstream ss;

        for(const ship& s : shipYard )
        {
            ss << s << "\n";
        }

        return ss.str();
    }

    bool getReady(account_name name)
    {
        user &player = get_player(name);
        bool canBeReady = player.bsplayer.getReady();
        return canBeReady;
    }

    void getUnready(account_name name)
    {
        user& player = get_player(name);
        player.bsplayer.getUnready();
    }

    bool remove_ship(account_name name,ship ship1)
    {
        user& player = get_player(name);
        bool isRemoved = player.bsplayer.removeShip(ship1);
        return isRemoved;
    }

    bool makeMove(account_name name ,  coordinate& coordinate1) noexcept(false)
    {
        user& player = get_player(name);
        user& opponent = get_opponent(name);

        if(player.bsplayer.isAttacked(coordinate1))
        {
            return false;
        }
        else
        {
            char cell = opponent.bsplayer.getCell(coordinate1);
            player.bsplayer.setCell(coordinate1,cell);
            tryToFinish(name, cell);
            turn++;
            return true;
        }
    }

    std::string get_enemy_table(account_name name)
    {
        user& player = get_player(name);
        return player.bsplayer.printenemytable();
    }

    std::string get_player_table(account_name name)
    {
        user& player = get_player(name);
        return player.bsplayer.printplayertable();
    }

    std::string get_both_tables(account_name name)
    {
        user& player = get_player(name);
        return player.bsplayer.printbothtables();
    }

    bool isFinished() const
    {
        return finished;
    }

    bool isStarted() const
    {
        return started;
    }


private:

    void tryToFinish(account_name name, char cellType) noexcept(false)
    {
        if(cellType != TableMapping["empty"] && cellType != TableMapping["unknown"])
        {
            user& player = get_player(name);
            player.bsplayer.hitShip();
            unsigned int numberOfHits = player.bsplayer.getNumberOfHits();
            if(numberOfHits == MAX_NUMBER_OF_ATTACKS)
            {

                throw(finish(name));
            }
        }
    }

    bool finished ;

    bool started ;

    int turn;

    user host;

    user competitor;

    account_name gameid;

};


