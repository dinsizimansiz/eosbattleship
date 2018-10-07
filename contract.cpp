//
// Created by oguz on 9/28/18.
//



#include <eosiolib/eosio.hpp>
#include <eosiolib/print.hpp>
#include <eosiolib/multi_index.hpp>
#include <vector>
#include <string>

using namespace eosio;
using std::string;
using std::vector;
using std::find;

class battleship : public contract
{

public:

    using contract::contract;


    battleship(account_name self)
    : contract(self),_games(self,self),_queue(self,self)
    {

    }




    ///@abi action
    void version(account_name player)
    {
        print("1.0.0");
    }

    ///@abi action
    void enqueue(account_name player)
    {
        require_auth(player);
        auto itr = _queue.find(player);
        eosio_assert(!_ingame(player),"Player is in the game.");
        eosio_assert(itr == _queue.end(),"Player is already in queue.");
        _queue.emplace(get_self(),[&](user& us){
            us.userid = player;

        });

        creategame();
    }

    ///@abi action
    void dequeue(account_name player)
    {
        require_auth(player);
        auto itr = _queue.find(player);
        eosio_assert(itr != _queue.end(),"Player is not in queue.");
        eosio_assert(!_ingame(player),"Player is in the game.");
        _queue.erase(itr);
    }

    ///@abi action
    void ingame(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"User is in the game.");
    }



    ///@abi action
    void playertable(account_name playerid) {

        require_auth(playerid);
        eosio_assert(_ingame(playerid),"User is not in a game.");
        account_name gameid = getgameid(playerid);
        auto itr = _games.find(gameid);
        string table;
        _games.modify(itr,get_self(),[&](game& g){
            user& pl = g.getplayer(playerid);
            table = g.printboard(pl.playertable);
        });
        print(table);
    }

    ///@abi action
    void enemytable(account_name playerid) {

        require_auth(playerid);
        eosio_assert(_ingame(playerid),"User is not in a game.");
        account_name gameid = getgameid(playerid);
        auto itr = _games.find(gameid);
        string table;
        _games.modify(itr,get_self(),[&](game& g){
            user& pl = g.getplayer(playerid);
            table = g.printboard(pl.enemytable);
        });
        print(table);
    }

    ///@abi action
    void bothtables(account_name playerid) {

        require_auth(playerid);
        eosio_assert(_ingame(playerid),"User is not in a game.");
        account_name gameid = getgameid(playerid);
        auto itr = _games.find(gameid);
        string bothTables;
        _games.modify(itr,get_self(),[&](game& g){
            eosio_assert(!g.started,"Game is already started");
            user& pl = g.getplayer(playerid);
            bothTables = g.everyboard(pl.playertable,pl.enemytable);
        });
        print(bothTables);

    }

    ///@abi action
    void remships(account_name playerid) {

        require_auth(playerid);
        eosio_assert(_ingame(playerid),"User is not in a game.");
        account_name gameid = getgameid(playerid);
        auto itr = _games.find(gameid);
        string remShips;
        _games.modify(itr,get_self(),[&](game& g){
            eosio_assert(!g.started,"Game is already started");
            user& pl = g.getplayer(playerid);
            remShips = g.remainingships(pl);
        });
        print(remShips);
    }

    ///@abi action
    void curships(account_name playerid) {

        require_auth(playerid);
        eosio_assert(_ingame(playerid),"User is not in a game.");
        account_name gameid = getgameid(playerid);
        auto itr = _games.find(gameid);
        string curShips;
        _games.modify(itr,get_self(),[&](game& g){
            eosio_assert(!g.started,"Game is already started");
            user& pl = g.getplayer(playerid);
            curShips= g.remainingships(pl);
        });
        print(curShips);
    }

    ///@abi action
    void isturn(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Player is not in a game.");
        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        _games.modify(itr,get_self(),[&](game& g) {
            bool turn;
            turn = g.isTurn(player);
            eosio_assert(turn,"It is not player's turn.");
        });
    }

    ///@abi action
    void placeship(account_name player,string shipname,uint8_t x,uint8_t y,string direction)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Not in game");
        eosio_assert(game::isship(shipname) && game::isdirection(direction) && game::inmap(x,y),"Bad arguments");
        account_name gameid = getgameid(player);

        auto itr = _games.find(gameid);
        bool flag;
        _games.modify(itr,get_self(),[&](game& g){
            flag = g.placeship(player,shipname,x,y,direction);
            eosio_assert(flag,"Ship cannot be placed.");
        });


    }

    ///@abi action
    void playing(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Player is not in a game.");
        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        _games.modify(itr,get_self(),[&](game& g){
           bool gameStarted = g.started;
           eosio_assert(g.started,"Game is not started.");
        });
    }



    ///@abi action
    void ready(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Player is not in a game.");
        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        _games.modify(itr,get_self(),[&](game& g){
            bool canBeReady = g.setReady(player);
            eosio_assert(canBeReady,"Player cannot be ready.");
        });
    }
    ///@abi action
    void unready(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Player is not in a game.");
        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        _games.modify(itr,get_self(),[&](game& g){
            g.setUnready(player);
        });

    }

    ///@abi action
    void makemove(account_name player,uint8_t x,uint8_t y)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Not in game");
        eosio_assert( game::inmap(x,y),"Bad arguments");
        account_name gameid = getgameid(player);

        auto itr = _games.find(gameid);
        bool flag;
        bool gamefinished;
        account_name host,challenger;
        _games.modify(itr,get_self(),[&](game& g){
            eosio_assert(g.started,"Game is not started.");
            bool isPlayersTurn = g.isTurn(player);
            eosio_assert(isPlayersTurn,"It is not player's turn.");
            flag = g.makemove(player,x,y);
            eosio_assert(flag,"Move cannot be made.");
            gamefinished = g.isfinished(player);
            host = g.host.userid;
            challenger = g.challenger.userid;
        });



        if(gamefinished)
        {
            require_recipient(challenger);
            require_recipient(host);
            _games.erase(itr);
        }

    }

    ///@abi action
    void removeship(account_name player,string shipname)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Not in game");
        eosio_assert(game::isship(shipname),"Bad arguments.");

        account_name gameid = getgameid(player);

        auto itr = _games.find(gameid);
        bool flag;
        _games.modify(itr,get_self(),[&](game& g){
            flag = g.removeship(player,shipname);
            eosio_assert(flag, "Ship cannot be removed.");
        });

    }


private:

    void creategame()
    {
        uint8_t counter = 0;
        vector<account_name> users;
        for(auto usr : _queue)
        {
            counter++;
            users.emplace_back(usr.userid);
        }
        if(counter == 2)
        {
            auto hostIter = _queue.find(users[0]);
            _queue.erase(hostIter);
            auto challengerIter = _queue.find(users[1]);
            _queue.erase(challengerIter);
            _games.emplace(get_self(),[&](game& g){
                g.started = false;
                g.gameid = _games.available_primary_key();
                g.host.userid = users[0];
                g.host.playertable = vector<string>(100,"0");
                g.host.enemytable = vector<string>(100,"X");
                g.challenger.userid = users[1];
                g.challenger.playertable = vector<string>(100,"0");
                g.challenger.enemytable = vector<string>(100,"X");
            });
        }

    }


    bool _ingame(account_name player)
    {
        for(const game& g : _games)
        {
            if(player == g.host.userid || g.challenger.userid == player)
            {
                return true;
            }
        }
        return false;
    }

    account_name getgameid(account_name player)
    {
        for(const game& g : _games)
        {
            if(player == g.host.userid || g.challenger.userid == player)
            {
                return g.gameid;
            }
        }
    }


    ///@abi table
    struct user
    {
        account_name userid;
        std::vector<string> playertable;
        std::vector<string> enemytable;
        bool ready = false;
        account_name primary_key() const {return userid;}

        EOSLIB_SERIALIZE(user,(userid)(playertable)(enemytable)(ready));
    };

    ///@abi table
    struct game
    {
        account_name gameid;
        user host;
        user challenger;
        bool started;
        uint8_t round = 0;

        account_name primary_key() const {return gameid;}
        EOSLIB_SERIALIZE(game,(gameid)(host)(challenger)(started)(round));

        bool isTurn(account_name player)
        {
            if(player == host.userid)
            {
                return round%2 == 0;
            }
            else
            {
                return round%2 == 1;
            }

        }




        void setUnready(account_name player)
        {
            if(host.userid == player)
            {
                host.ready = false;
            }
            else
            {
                host.ready = false;
            }

        }

        bool setReady(account_name player)
        {
            if(canBeReady(player))
            {
                if(host.userid == player)
                {
                    host.ready = true;
                }
                else
                {
                    challenger.ready = true;
                }
                startgame();
                return true;
            }
            else
            {
                return false;
            }
        }


        void startgame()
        {
            if(challenger.ready && host.ready)
            {
                started = true;
            }
        }

        bool canBeReady(account_name player)
        {
            if(host.userid == player)
            {
                return _canBeReady(host.playertable);
            }
            else
            {
                return _canBeReady(challenger.playertable);
            }

        }

        bool _canBeReady(vector<string> playerTable)
        {
            uint8_t counter = 0;
            for(const string& c : playerTable)
            {
                if(c != "0")
                {
                    counter++;
                }
            }
            //17 is the total number of ship cells.
            return counter == 17;
        }


        bool isfinished(account_name player)
        {
            if(player == host.userid)
            {
                return _isfinished(host);
            }
            else
            {
                return _isfinished(challenger);
            }
        }

        static bool isship(string shipname)
        {
            if(shipname == "carrier")
            {
                return true;
            }
            else if(shipname == "battleship")
            {
                return true;
            }
            else if(shipname == "destroyer")
            {
                return true;
            }
            else if(shipname == "submarine")
            {
                return true;
            }
            else if(shipname == "cruiser")
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static bool isdirection(string direction)
        {
            if(direction == "n" || direction == "north" )
            {
                return true;
            }
            else if(direction == "e" || direction == "east")
            {
                return true;
            }
            else if(direction == "s" || direction == "south")
            {
                return true;
            }
            else if(direction == "w" || direction == "west")
            {
                return true;
            }
            else
            {
                return false;
            }

        }

        static bool inmap(uint8_t x,uint8_t y)
        {
            if((x >= 0 && x < 10) && (y >= 0 && y <10))
            {
                return true;
            }
            else
            {
                return false;
            }
        }


        bool placeship(account_name user,string shipname,uint8_t x,uint8_t y,string direction)
        {
            if(user == host.userid)
            {
                return _placeship(host,shipname,x,y,direction);
            }
            else
            {
                return _placeship(challenger,shipname,x,y,direction);
            }
        }

        bool removeship(account_name user,string shipname)
        {
            if(user == host.userid)
            {
                return _removeship(host,shipname);
            }
            else
            {
                return _removeship(challenger,shipname);
            }
        }

        bool makemove(account_name user,uint8_t x,uint8_t y)
        {
            uint8_t index = convertoindex(x,y);

            if(user == host.userid)
            {
                string cellchar = challenger.playertable[index];
                return _makemove(host,index,cellchar);
            }
            else
            {
                string cellchar = host.playertable[index];
                return _makemove(challenger,index,cellchar);
            }
        }

        user& getplayer(account_name name)
        {
            if(name == host.userid)
            {
                return host;
            }
            else
            {
                return challenger;
            }
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

        string currentships(const user& player)
        {
            vector<string> allships = {"carrier","battleship","submarine","cruiser","destroyer"};
            string retString;
            for(const string &s : player.playertable)
            {
                if(s == "1" || s == "2" || s == "3" || s == "4" || s == "5")
                {
                    string shipname = getship(s);
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

        string remainingships(const user& player)
        {
            string retString;
            vector<string> remships;
            for(const string &s : player.playertable)
            {
                if(s == "1" || s == "2" || s == "3" || s == "4" || s == "5")
                {
                    string shipname = getship(s);
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

    private:


        bool _isfinished(const user& player)
        {
            uint8_t hits = 0;
            for(const string s : player.enemytable)
            {
                if(s == "1" || s == "2" || s == "3" || s == "4" || s == "5")
                {
                    hits++;
                }
            }
            return hits == 17;
        }

        bool _makemove(user& player,uint8_t index,string cellchar)
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

        bool _placeship(user& account,string shipname,uint8_t x,uint8_t y,string direction)
        {
            auto coords = getcoords(shipname,x,y,direction);
            string shipstr = getshipstring(shipname);

            for(auto cell : account.playertable)
            {
                if(cell == shipstr)
                {
                    return false;
                }

            }

            for(auto vec : coords)
            {
                if(!inmap(vec[0],vec[1]))
                {
                    return false;
                }
                else if(account.playertable[convertoindex(vec[0],vec[1])] != "0")
                {
                    return false;
                }
            }


            for(auto vec : coords)
            {
                uint8_t index = convertoindex(vec[0],vec[1]);
                account.playertable[index] = shipstr;
            }

            return true;

        }

        bool _removeship(user& account,string shipname)
        {
            bool flag = false;
            string shipstr = getshipstring(shipname);

            for(string& s : account.playertable)
            {
                if(s == shipname)
                {
                    s = "0";
                    flag = true;
                }
            }

            return flag;
        }


        vector<vector<uint8_t>> getcoords(string shipname,uint8_t x,uint8_t y,string direction)
        {
            uint8_t shipwidth = shipsize(shipname);
            vector<vector<uint8_t>> retVector ;
            if(direction == "n" || direction == "north")
            {
                for(uint8_t i = 0;i < shipwidth;i++)
                {
                    vector<uint8_t> k ;
                    k.push_back(x);
                    k.push_back(y-i);
                    retVector.push_back(k);
                }
            }
            else if(direction == "e" || direction == "east")
            {
                for(uint8_t i = 0;i < shipwidth;i++)
                {
                    vector<uint8_t> k ;
                    k.push_back(x+i);
                    k.push_back(y);
                    retVector.push_back(k);
                }
            }
            else if(direction == "s" || direction == "south")
            {
                for(uint8_t i = 0;i < shipwidth;i++)
                {
                    vector<uint8_t> k ;
                    k.push_back(x);
                    k.push_back(y+i);
                    retVector.push_back(k);
                }
            }
            else if(direction == "w" || direction == "west")
            {
                for(uint8_t i = 0;i < shipwidth ;i++)
                {
                    vector<uint8_t> k ;
                    k.push_back(x-i);
                    k.push_back(y);
                    retVector.push_back(k);
                }
            }
            return retVector;
        }

        uint8_t shipsize(string shipname)
        {
            if(shipname == "carrier")
            {
                return 5;
            }
            else if(shipname == "battleship")
            {
                return 4;
            }
            else if(shipname == "submarine")
            {
                return 3;
            }
            else if(shipname == "cruiser")
            {
                return 3;
            }
            else if(shipname == "destroyer")
            {
                return 2;
            }
        }

        string getshipstring(string shipname)
        {
            if(shipname == "carrier")
            {
                return "1";
            }
            else if(shipname == "battleship")
            {
                return "2";
            }
            else if(shipname == "submarine")
            {
                return "3";
            }
            else if(shipname == "cruiser")
            {
                return "4";
            }
            else if(shipname == "destroyer")
            {
                return "5";
            }
        }

        string getship(string shipchar)
        {
            if(shipchar == "1")
            {
                return "carrier";
            }
            else if(shipchar == "2")
            {
                return "battleship";
            }
            else if(shipchar == "3")
            {
                return "submarine";
            }
            else if(shipchar == "4")
            {
                return "cruiser";
            }
            else if(shipchar == "5")
            {
                return "destroyer";
            }
        }




        inline uint8_t convertoindex(uint8_t x,uint8_t y)
        {
            return y*10 + x;
        }



    };


    typedef multi_index<N(user),user> userbase;
    typedef multi_index<N(games),game> gamebase;

    userbase _queue;
    gamebase _games;

};

EOSIO_ABI(battleship,(version)(enqueue)(dequeue)(ingame)(placeship)(removeship)(makemove)(isturn)(ready)(unready)(playertable)(enemytable)(bothtables)(remships)(curships)(playing));
    //EOSIO_ABI(battleship,(ready))

