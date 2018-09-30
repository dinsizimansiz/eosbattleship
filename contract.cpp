//
// Created by oguz on 9/28/18.
//



#include <eosiolib/eosio.hpp>
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
    void version()
    {
        print("1.0.0");
    }

    ///@abi action
    void enqueue(account_name player)
    {
        require_auth(player);
        auto itr = find(_queue.begin(),_queue.end(),player);
        eosio_assert(itr == _queue.end(),"Player is already in queue.");


        creategame();
    }

    ///@abi action
    void dequeue(account_name player)
    {
        require_auth(player);
        auto itr = find(_queue.begin(),_queue.end(),player);
        eosio_assert(itr != _queue.end(),"Player is not in queue.");
        _queue.erase(itr);
    }

    ///@abi action
    void ingame(account_name player)
    {
        require_auth(player);
        if(_ingame(player))
        {
            print("Player is in game.");
        }
        else
        {
            print("Player is not in a game.");
        }
    }

    ///@abi action
    void yourboard(account_name player)
    {
        require_auth(player);

        eosio_assert(_ingame(player),"Player is not in a game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        string printedString;

        _games.modify(itr,get_self(),[&](game& g){
            printedString = g.printboard(player);
        });

        print(printedString);
    }

    ///@abi action
    void enemyboard(account_name player)
    {
        require_auth(player);

        eosio_assert(_ingame(player),"Player is not in a game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        string printedString;

        _games.modify(itr,get_self(),[&](game& g){
            printedString = g.enemyboard(player);
        });

        print(printedString);
    }

    ///@abi action
    void everyboard(account_name player)
    {
        require_auth(player);

        eosio_assert(_ingame(player),"Player is not in a game.");

        account_name gameid = getgameid(player);
        auto itr = _games.find(gameid);
        string printedString;

        _games.modify(itr,get_self(),[&](game& g){
            printedString = g.everyboard(player);
        });

        print(printedString);
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
        });

        if(flag)
        {
            print("Successfully placed the ship.");
        }
        else
        {
            print("Unsuccessful placement.");
        }

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
            flag = g.makemove(player,x,y);
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

        if(flag)
        {
            print("Successfully made the move.");
        }
        else
        {
            print("You've already fired to cell.");
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
        });

        if(flag)
        {
            print("Successfully deleted the ship.");
        }
        else
        {
            print("Ship does not exist.");
        }
    }

    ///@abi action
    void remships(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Not in game");

        account_name gameid = getgameid(player);

        auto itr = _games.find(gameid);
        string s;

        _games.modify(itr,get_self(),[&](game& g){
            s = g.remainingships(player);
        });

        print(s);

    }

    ///@abi action
    void curships(account_name player)
    {
        require_auth(player);
        eosio_assert(_ingame(player),"Not in game");

        account_name gameid = getgameid(player);

        auto itr = _games.find(gameid);
        string s;

        _games.modify(itr,get_self(),[&](game& g){
            s = g.currentships(player);
        });

        print(s);

    }

private:

    void creategame()
    {
        if(_queue.size() == 2)
        {
            _games.emplace(get_self(),[&](game& g){
                g.gameid = _games.available_primary_key();
                g.host.userid = _queue[0];
                g.host.playertable = vector<string>(100,"0");
                g.host.enemytable = vector<string>(100,"X");
                g.challenger.userid = _queue[1];
                g.challenger.playertable = vector<string>(100,"0");
                g.challenger.enemytable = vector<string>(100,"X");
                g.started = false;
            });

            _queue.clear();
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

        account_name primary_key() const {return userid;}

        EOSLIB_SERIALIZE(user,(userid)(playertable)(enemytable));
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
        EOSLIB_SERIALIZE(game,(gameid)(host)(challenger)(started));

        string currentships(account_name player)
        {
            if(player == host.userid)
            {
                return _currentships(host);
            }
            else
            {
                return _currentships(challenger);
            }
        }

        string remainingships(account_name player)
        {
            if(player == host.userid)
            {
                return _remainingships(host);
            }
            else
            {
                return _remainingships(challenger);
            }
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

        string printboard(account_name player)
        {
            if(player == host.userid)
            {
                return _printboard(host);
            }
            else
            {
                return _printboard(challenger);
            }
        }

        string enemyboard(account_name player)
        {
            if(player == host.userid)
            {
                return _printboard(host);
            }
            else
            {
                return _printboard(challenger);
            }

        }

        string everyboard(account_name player)
        {
            if(player == host.userid)
            {
                return _everyboard(host);
            }
            else
            {
                return _everyboard(challenger);
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

    private:

        string _currentships(const user& player)
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

        string _remainingships(const user& player)
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

            string shipstr = getshipstring(shipname);

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

        string _printboard(user& account)
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
                    retString.append(account.playertable[i*10+j]);
                }
                retString += "\n";
            }

            return retString;
        }

        string _everyboard(const user& account)
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
                    retString.append(account.playertable[i*10+j]);
                    retString += space;
                    retString += " ";
                    retString.append(account.enemytable[i*10+j]);
                }
                retString += "\n";
            }

            return retString;
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


    typedef vector<account_name> userbase;
    typedef multi_index<N(games),game> gamebase;

    userbase _queue;
    gamebase _games;

};

EOSIO_ABI(battleship,(version)(enqueue)(dequeue)(ingame)(yourboard)(enemyboard)(everyboard)(placeship)(removeship)(makemove)(curships)(remships));
    //EOSIO_ABI(battleship,(ready))
//  EOSIO_ABI(battleship,(isready)(enqueue)(dequeue)(ready)(unready)(placeship)(removeship)(placedships)(freeships)(makemove)(bothtables)(enemytable)(playertable));

