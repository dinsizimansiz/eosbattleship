//
// Created by oguz on 24.09.2018.
//

#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/dispatcher.hpp>
#include <eosiolib/contract.hpp>
#include "battleship/gameproto.cpp"
#include "battleship/user.h"

#define __BATTLESHIP_VERSION__  "1.0.0"

using namespace eosio;

struct client
{
    account_name key;
    account_name primary_key() const {return key;}
};

//TODO Game beginning will be handled by a higher degree class.
//TODO Turns will be handled by higher degree class.
//TODO If coordinate is outside the map , then handle it at a higher degree class.


struct game
{
    gameproto gp;

    account_name primary_key() const {return gp.primary_key();}
    account_name host_key() const {return gp.host_key();}
    account_name competitor_key() const {return gp.competitor_key();}

};

typedef multi_index<N(game),game,
        indexed_by<N(hostid),const_mem_fun<game,account_name ,&game::host_key>>,
        indexed_by<N(competitorid),const_mem_fun<game,account_name,&game::competitor_key>>> gamebase;


typedef multi_index<N(client),client> userbase;

class battleship : public contract
{
public:

    battleship(account_name self) : contract(self), _games(self, self), _queue(self, self)
    {
        _queueLength = 0;
    }

    [[action]]
    void version()
    {
        print("Version : ", __BATTLESHIP_VERSION__);
    }

    [[action]]
    void enqueue(account_name player)
    {
        require_auth(player);

        auto itr = _queue.find(player);
        bool inQueue = itr != _queue.end();

        if (!inQueue)
        {
            _queue.emplace(get_self(), [&](client &c)
            {
                c.key = player;
            });
            print("Succesfully enqueued.");
            _queueLength++;
            if (_queueLength == 2) {
                matchmaking();
            }
        }
    }


    [[action]]
    void dequeue(account_name player)
    {
        require_auth(player);

        auto itr = _queue.find(player);
        bool inQueue = itr != _queue.end();

        if (inQueue)
        {
            _queue.erase(itr);
            _queueLength--;
            print("Successfully dequeued.");
        }
    }

    [[action]]
    void placeship(account_name player, std::string shipname, int x, int y, std::string direction)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");
        eosio_assert(isdirectory(direction), "Invalid directory.");

        coordinate coord = coordinate(x, y);
        eosio_assert(coord.inTable(), "Given coordinates are not in table.");

        account_name gameid = getgameid(player);

        bool isPlaced;

        auto itr = _games.find(gameid);

        _games.modify(itr, get_self(), [&](game &g)
        {
            isPlaced = g.gp.place_ship(player, shipname, coord, stringtodir(direction));
        });

        eosio_assert(isPlaced, "Ship cannot be placed.");
    }

    [[action]]
    void getready(account_name player)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        account_name gameid = getgameid(player);
        bool canBeReady;
        auto itr = _games.find(gameid);

        _games.modify(itr, get_self(), [&](game &g)
        {
            canBeReady = g.gp.getReady(player);
        });

        eosio_assert(canBeReady,"Player cannot be ready.");
    }

    [[action]]
    void getunready(account_name player)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);

        _games.modify(itr, get_self(), [&](game &g)
        {
            g.gp.getUnready(player);
        });
    }

    [[action]]
    void printtables(account_name player)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        std::string table;

        _games.modify(itr, get_self(), [&](game &g) {
            table = g.gp.get_both_tables(player);
        });

        print(table);
    }

    [[action]]
    void getplayertable(account_name player)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        std::string table;

        _games.modify(itr, get_self(), [&](game &g)
        {
            table = g.gp.get_player_table(player);
        });

        print(table);
    }

    [[action]]
    void getenemytable(account_name player)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        std::string table;

        _games.modify(itr, get_self(), [&](game &g)
        {
            table = g.gp.get_enemy_table(player);
        });

        print(table);
    }

    [[action]]
    void remainingships(account_name player)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        std::string table;

        _games.modify(itr, get_self(), [&](game &g)
        {
            table = g.gp.get_remaining_ships(player);
        });

        print(table);
    }

    [[action]]
    void makemove(account_name player, int x, int y)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");

        coordinate coord = coordinate(x, y);
        eosio_assert(coord.inTable(), "Given coordinates are not in table.");

        account_name gameid = getgameid(player);

        auto itr = _games.find(gameid);
        bool moveMade;

        try {
            _games.modify(itr, get_self(), [&](game &g)
            {
                eosio_assert(g.gp.is_turn(player),"It is not player's turn.");
                moveMade = g.gp.makeMove(player, coord);
            });
        }
        catch (finish f) {
            print("The game is over.");
        }

        eosio_assert(moveMade, "Move cannot be made successfully.");
    }

    [[action]]
    void removeship(account_name player, std::string shipName)
    {
        require_auth(player);
        eosio_assert(ingame(player), "Given player is not in an active game.");
        eosio_assert(isship(shipName),"Given ship does not exist.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        bool moveMade;
        ship shp = ship(shipName);

        _games.modify(itr, get_self(), [&](game &g)
        {
            moveMade = g.gp.remove_ship(player, shp);
        });

        eosio_assert(moveMade, "Move cannot be made successfully.");
    }

private:


    void addgame(account_name hostid, account_name competitorid) {

        _games.emplace(get_self(), [&](game &g) {
            g.gp = gameproto(_games.available_primary_key(), hostid, competitorid);
        });
    }

    void matchmaking() {
        std::vector<client> arr;

        for (auto itr = _queue.begin(); itr != _queue.end(); ++itr) {
            arr.emplace_back(*itr);
        }
        _queue.erase(arr[0]);
        _queue.erase(arr[1]);

        addgame(arr[0].key, arr[1].key);
    }


    bool ingame(account_name player) {
        for (const game &gm : _games) {
            if (gm.host_key() == player || gm.competitor_key() == player) {
                if (gm.gp.isFinished()) {
                    return true;
                } else {
                    continue;
                }

            }
        }
        return false;

    }


    account_name getgameid(account_name player) {
        account_name gameid;
        for (const game &gm : _games) {
            if (gm.host_key() == player || gm.competitor_key() == player) {
                if (gm.gp.isFinished()) {
                    continue;
                } else {
                    gameid = gm.primary_key();
                    return gameid;
                }

            }
        }
    }

    direction stringtodir(std::string dir) {
        if (dir == "UP")
        {
            return UP;
        }
        else if (dir == "DOWN")
        {
            return DOWN;
        }
        else if (dir == "LEFT")
        {
            return LEFT;
        }
        else if (dir == "RIGHT")
        {
            return RIGHT;
        }
    }

    bool isdirectory(std::string dir)
    {

        if (dir != "UP" || dir != "DOWN" || dir != "LEFT" || dir != "RIGHT")
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    bool isship(std::string shipName)
    {
        if(shipName != "battleship" || shipName != "carrier" || shipName != "cruiser" || shipName != "destroyer" || shipName != "carrier")
        {
            return false;
        }
        else
        {
            return true;
        }
    }

    gamebase _games;

    userbase _queue;

    int _queueLength ;
};


