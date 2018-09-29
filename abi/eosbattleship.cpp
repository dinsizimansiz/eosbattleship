//
// Created by oguz on 9/28/18.
//



#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <vector>
#include <map>
#include "../Ship.cpp"

#define  __MAX_TOTAL_HITS__ 17
#define __MAX_SHIP_CELLS__ 17

namespace battleship
{
    using std::vector ;
    using std::string;
    using std::map;
    using std::make_pair;
    using std::find;
    using std::endl;
    using namespace eosio;

    map<uint8_t,char> playerPrintMapping = {{0,'0'},{1,'1'},{2,'2'},{3,'3'},{4,'4'},{5,'5'}};
    map<uint8_t,char> enemyPrintMapping = {{0,'X'},{1,'0'},{2,'1'},{3,'2'},{4,'3'},{5,'4'},{6,'5'}};


    map<string,uint8_t> playerTableMapping = {{"empty",0},{"carrier",1},{"battleship",2},{"cruiser",3},{"submarine",4},{"destroyer",5}};
    map<string,uint8_t> enemyTableMapping = {{"unknown",0},{"empty",1},{"carrier",2},{"battleship",3},{"cruiser",4},{"submarine",5},{"destroyer",6}};

    map<uint8_t,string> reversePlayerTableMapping = {{0,"empty"},{1,"carrier"},{2,"battleship"},{3,"cruiser"},{4,"submarine"},{5,"destroyer"}};



    class battleship : public contract
    {

    public:

        battleship(account_name self)
        : contract(self),_games(self,self) , _queue(self,self)
        {

        }

        ///@abi action
        void enqueue(account_name user1)
        {
            require_auth(user1);

            auto itr = find(_queue.begin(),_queue.end(),user1);
            eosio_assert(itr != _queue.end(),"User is already waiting.");

            _queue.emplace_back(user1);
            matchmaking();
        }

        ///@abi action
        void dequeue(account_name user1)
        {
            require_auth(user1);

            auto itr = find(_queue.begin(),_queue.end(),user1);
            eosio_assert(itr == _queue.end(),"User is not in queue.");

            _queue.erase(itr);
        }

        ///@abi action
        void placeship(account_name name1,string shipName,uint8_t x,uint8_t y,string dir)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");
            eosio_assert(isShip(shipName),"Invalid Ship name.");
            eosio_assert(isDirection(dir),"Invalid directory name.");

            Coordinate coord = Coordinate(x,y);
            Direction direction = stringToDir(dir);
            Ship ship = Ship(shipName);

            eosio_assert(shipCoordsInTable(ship, coord, direction),"Ship coordinates are out of range.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);
            bool canBePlaced;
            _games.modify(itr,get_self(),[&](game& g){
                canBePlaced = g.placeShip(name1,ship,coord,direction);
            });

            eosio_assert(canBePlaced,"Ship cannot be placed.");
        }

        ///@abi action
        void removeship(account_name name1 ,string shipName)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");
            eosio_assert(isShip(shipName),"Invalid Ship name.");

            Ship ship = Ship(shipName);

            account_name host = getHost(name1);
            auto itr = _games.find(host);
            bool canBeRemoved;
            _games.modify(itr,get_self(),[&](game& g){
                canBeRemoved = g.removeShip(name1,ship);
            });

            eosio_assert(canBeRemoved,"Ship is not found.");

        }

        ///@abi action
        void freeships(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);

            _games.modify(itr,get_self(),[&](game& g){
                string remShips = g.getUnplacedShips(name1);
                print(remShips);
            });

        }

        ///@abi action
        void placedships(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);

            _games.modify(itr,get_self(),[&](game& g){
                string ships = g.getPlacedShips(name1);
                print(ships);
            });
        }

        ///@abi action
        void makemove(account_name name1, uint8_t x , uint8_t y)
        {
            require_auth(name1);

            Coordinate coord = Coordinate(x,y);

            eosio_assert(isPlaying(name1),"User is not in the game.");
            eosio_assert(coord.inTable(),"Coordinates are not in table.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);
            bool canBeMoved;

            _games.modify(itr,get_self(),[&](game& g){
                eosio_assert(g.isturn(name1),"It is not User's turn.");
                canBeMoved = g.makemove(name1,coord);
            });

            eosio_assert(canBeMoved,"Coordinates are already hit.");

        }

        ///@abi action
        void bothtables(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);

            _games.modify(itr,get_self(),[&](game& g){
                string tmp = g.printBothTables(name1);
                print(tmp);
            });
        }

        ///@abi action
        void enemytable(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);

            _games.modify(itr,get_self(),[&](game& g){
                string tmp = g.printEnemyTable(name1);
                print(tmp);
            });
        }

        ///@abi action
        void playertable(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);

            _games.modify(itr,get_self(),[&](game& g){
                string tmp = g.printEnemyTable(name1);
                print(tmp);
            });
        }

        ///@abi action
        void ready(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);
            bool canBeReady;

            _games.modify(itr,get_self(),[&](game& g){
                canBeReady = g.setPlayerReady(name1);
            });

            eosio_assert(canBeReady,"User cannot be ready");
        }

        ///@abi action
        void unready(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);

            _games.modify(itr,get_self(),[&](game& g){
                g.setPlayerUnready(name1);
            });
        }

        ///@abi action
        void isready(account_name name1)
        {
            require_auth(name1);

            eosio_assert(isPlaying(name1),"User is not in the game.");

            account_name host = getHost(name1);
            auto itr = _games.find(host);
            bool isReady;

            _games.modify(itr,get_self(),[&](game& g){
                isReady = g.playerIsReady(name1);
            });

            if(isReady)
            {
                print("User is ready.");
            }
            else
            {
                print("User is not ready.");
            }
        }

    private:

        void matchmaking()
        {
            if(_queue.size() == 2) {
                account_name host = _queue[0];
                account_name compet = _queue[1];

                _games.emplace(get_self(),[&](game& g){
                    g.host = host;
                    g.compet = compet;
                });

                _queue.clear();
            }
        }

        static Direction stringToDir(string dir) {
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



        static bool isDirection(string dir)
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

        static bool isShip(string shipName)
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

        bool isPlaying(account_name name1)
        {
            for(const game& gm : _games)
            {
                if((gm.compet == name1 || gm.host == name1) && !gm.finished)
                {
                    return true;
                }
            }
            return false;
        }

        bool shipCoordsInTable(Ship ship, Coordinate coord, Direction dir)
        {
            vector<Coordinate> coords = ship.getCoords(dir,coord);
            for(Coordinate coord:coords)
            {
                if(!coord.inTable())
                {
                    return false;
                }
            }

            return true;
        }

        account_name getHost(account_name name1)
        {
            for(const game& gm : _games)
            {
                if(gm.host == name1 && !gm.finished)
                {
                    return name1;
                }
                else if(gm.compet == name1 && !gm.finished)
                {
                    return gm.host;
                }
                else
                {
                    continue;
                }
            }
        }

        ///@abi table
        struct game
        {
            game()
            {
                hosttable = table(100,0);
                hostenemy = table(100,0);
                compettable = table(100,0);
                competenemy = table(100,0);
                finished = false;
                started = false;
                round = 0;
            }

            typedef vector<uint8_t > table;

            account_name host;
            account_name compet;
            account_name winner;
            table hostenemy;
            table hosttable;
            table compettable;
            table competenemy;
            bool started;
            bool finished;
            std::vector<bool> readyStates = {false,false};
            uint8_t round;

            account_name primary_key() const {return host; }

            bool ishost(account_name name1)
            {
                return host == name1;
            }

            bool iscompet(account_name name1)
            {
                return compet == name1;
            }

            bool isturn(account_name name1)
            {
                if(ishost(name1) && round%2 == 0)
                {
                    return true;
                }
                else if(iscompet(name1) && round%2)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }

            bool setPlayerReady(account_name name1)
            {
                if(playerCanBeReady(name1))
                {
                    if(ishost(name1))
                    {
                        readyStates[0] = true;
                    }
                    else
                    {
                        readyStates[1] = true;
                    }

                    return true;
                }
                else
                {
                    return false;
                }

            }

            void startTheGame()
            {
                if(readyStates[0] && readyStates[1])
                {
                    started = true;
                }
                else
                {
                    ;
                }

            }

            void setPlayerUnready(account_name name1)
            {
                if(ishost(name1))
                {
                    readyStates[0] = false;
                }
                else
                {
                    readyStates[1] = false;
                }
            }

            bool playerCanBeReady(account_name name1)
            {
                table playerTable = getPlayerTable(name1);
                uint8_t shipCells = 0;

                for(const uint8_t &i : playerTable)
                {
                    if(i != playerTableMapping["empty"])
                    {
                        shipCells++;
                    }
                }

                return shipCells == __MAX_SHIP_CELLS__;
            }

            bool playerIsReady(account_name name1)
            {
                return readyStates[name1];
            }

            bool placeShip(account_name name1, Ship shp , Coordinate k,Direction dir)
            {
                table& playerTable = getPlayerTable(name1);

                for(uint8_t i : playerTable)
                {
                    if(i == playerTableMapping[shp.getName()])
                    {
                        return false;
                    }
                }

                auto coords = shp.getCoords(dir,k);
                for(Coordinate& coord : coords)
                {
                    if(coord.inTable() || playerTable[coord.convertToIndex()] != playerTableMapping["empty"])
                    {
                        return false;
                    }
                }

                for(Coordinate& coord : coords)
                {
                    playerTable[coord.convertToIndex()] = playerTableMapping[shp.getName()];
                }

                return true;
            }

            bool removeShip(account_name name1, Ship shp)
            {
                table &playerTable = getPlayerTable(name1);
                bool isRemoved = false;


                for(uint8_t &i : playerTable)
                {
                    if(i == playerTableMapping[shp.getName()])
                    {
                        i = playerTableMapping["empty"];
                        isRemoved = true;
                    }
                }
                if(playerIsReady(name1) && isRemoved)
                {
                    setPlayerUnready(name1);

                }

                return isRemoved;
            }


            bool makemove(account_name name1, Coordinate k)
            {
                table& enemyTable = getEnemyTable(name1);

                if(enemyTable[k.convertToIndex()] != enemyTableMapping["unknown"])
                {
                    return false;

                }
                else
                {

                    table enemyPlayerTable = getPlayerTable(getOpponent(name1));
                    enemyTable[k.convertToIndex()] = enemyPlayerTable[k.convertToIndex()];
                    isItFinished(name1);
                    round++;
                    return true;
                }

            }

            void isItFinished(account_name name1)
            {
                table &enemyTable = getEnemyTable(name1);
                uint8_t totalHits;

                for(uint8_t &c : enemyTable)
                {
                    if(c != enemyTableMapping["unknown"] && c != enemyTableMapping["empty"])
                    {
                        totalHits++;
                    }
                }
                if(totalHits == __MAX_TOTAL_HITS__)
                {
                    finished = true;
                    winner = name1;
                }
            }


            string getPlacedShips(account_name name1)
            {
                const table& playerTable = getPlayerTable(name1);
                vector<string> strVector;
                string retString;

                for(const uint8_t &i : playerTable)
                {
                    string shipName = reversePlayerTableMapping[i];
                    auto itr = find(strVector.begin(),strVector.end(),shipName);
                    if(isShip(shipName) && itr != strVector.end())
                    {
                        strVector.erase(itr);
                    }
                }

                for(const string& shipName :  strVector)
                {
                    retString += shipName;
                    retString += "\n";
                }

                return retString;
            }

            string getUnplacedShips(account_name name1)
            {
                const table& playerTable = getPlayerTable(name1);
                vector<string> strVector;
                string retString;

                for(const uint8_t &i : playerTable)
                {
                    string shipName = reversePlayerTableMapping[i];
                    if(isShip(shipName) && find(strVector.begin(),strVector.end(),shipName) == strVector.end())
                    {
                        strVector.emplace_back(shipName);
                    }
                }

                for(string shipName :  strVector)
                {
                    retString += shipName ;
                    retString +="\n";
                }

                return retString;
            }

            string printPlayerTable(account_name name1)
            {
                table playerTable = getPlayerTable(name1);

                return toString(playerTable,playerPrintMapping);
            }

            string printEnemyTable(account_name name1)
            {
                table enemyTable = getEnemyTable(name1);

                return toString(enemyTable,enemyPrintMapping);
            }

            string printBothTables(account_name name1)
            {
                string playerTableString = printPlayerTable(name1);
                string enemyTableString = printEnemyTable(name1);
                string outputString ;
                string tmp;


                vector<string> playerTableAllLines = splitString(playerTableString,"\n");
                vector<string> enemyTableAllLines = splitString(enemyTableString,"\n");

                outputString += "\n\t\tYour Table\t\t\t\t            Enemy Table\n\n";



                for(auto it1 = playerTableAllLines.begin(),it2 = enemyTableAllLines.begin();it1 != playerTableAllLines.end(); ++it1,++it2 )
                {
                    outputString += "\t\t";
                    outputString += *it1 ;
                    outputString += "\t\t\t\t";
                    outputString += *it2 ;
                    outputString += "\n";
                }

                return outputString;
            }

        private:

            //Includes business logic.
            //Not a general split function.

            vector<string> splitString(string s, string delimiter)
            {
                vector<string> retVector;
                size_t pos = 0;
                std::string token;
                while ((pos = s.find(delimiter)) != std::string::npos) {
                    token = s.substr(0, pos);
                    retVector.emplace_back(token);
                    s.erase(0, pos + delimiter.length());
                }
                retVector.emplace_back(s);

                return retVector;
            }

            account_name getOpponent(account_name name1)
            {
                if(name1 == host)
                {
                    return compet;
                }
                else
                {
                    return host;
                }

            }

            string toString(table tbl,map<uint8_t ,char> mapping)
            {
                string n;

                n += " ";
                for(uint8_t i = 0; i <= 9;i++)
                {
                    n += " ";
                    n += i;
                }
                n += "\n";
                for(uint8_t i = 0; i <= 9;i++)
                {
                    n += i;

                    for(uint8_t j = 0; j <= 9;j++)
                    {
                        n += " ";
                        n += mapping[tbl[i*10+j]];
                    }
                    n += "\n";
                }
                return n;
            }

            table& getPlayerTable(account_name name1)
            {
                if(ishost(name1))
                {
                    return hosttable;
                }
                else
                {
                    return compettable;
                }
            }

            table& getEnemyTable(account_name name1)
            {
                if(ishost(name1))
                {
                    return hostenemy;
                }
                else
                {
                    return competenemy;
                }
            }


        };



        typedef vector<account_name> userbase;

        typedef multi_index<N(games),game> gamebase;

        gamebase _games;

        userbase _queue;
    };

    EOSIO_ABI(battleship,(isready)(enqueue)(dequeue)(ready)(unready)(placeship)(removeship)(placedships)(freeships)(makemove)(bothtables)(enemytable)(playertable));
}

