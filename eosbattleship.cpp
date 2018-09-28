//
// Created by oguz on 9/28/18.
//



#include <eosiolib/eosio.hpp>
#include <eosiolib/multi_index.hpp>
#include <vector>
#include <sstream>
#include <map>
#include "ship.cpp"


namespace battleship
{
    using std::vector ;
    using std::string;
    using std::stringstream;
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

        [[action]]
        void enqueue(account_name user1)
        {
            require_auth(user1);

            auto itr = find(_queue.begin(),_queue.end(),user1);
            eosio_assert(itr != _queue.end(),"User is already waiting.");

            _queue.emplace_back(user1);
            matchmaking();
        }

        [[action]]
        void dequeue(account_name user1)
        {
            require_auth(user1);

            auto itr = find(_queue.begin(),_queue.end(),user1);
            eosio_assert(itr == _queue.end(),"User is not in queue.");

            _queue.erase(itr);
        }

        [[action]]
        void placeship()
        {

        }

        [[action]]
        void removeship()
        {

        }

        [[action]]
        void unplacedships()
        {

        }

        [[action]]
        void placedships()
        {

        }

        [[action]]
        void makemove()
        {

        }

        [[action]]
        void bothtables()
        {

        }

        [[action]]
        void enemytable()
        {

        }

        [[action]]
        void playertable()
        {

        }

        [[action]]
        void ready()
        {

        }

        [[action]]
        void unready()
        {

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

        struct [[eosio::table]] game
        {
            game()
            {
                hosttable = vector<uint8_t >(100,0);
                hostenemy = vector<uint8_t >(100,0);
                compettable = vector<uint8_t >(100,0);
                competenemy = vector<uint8_t >(100,0);
                finished = false;
                started = false;
                round = 0;
            }

            typedef vector<uint8_t > table;

            account_name host;
            account_name compet;
            table hostenemy;
            table hosttable;
            table compettable;
            table competenemy;
            bool started;
            bool finished;


            int round;

            account_name primary_key() const {return host;   }

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
                }else if(iscompet(name1) && round%2)
                {
                    return true;
                } else{
                    return false;
                }
            }


            bool placeShip(account_name name1, ship shp , coordinate k,direction dir)
            {
                table& playerTable = getPlayerTable(name1);

                auto coords = shp.getCoords(dir,k);
                for(coordinate& coord : coords)
                {
                    if(coord.inTable() || playerTable[coord.convertToIndex()] != playerTableMapping["empty"])
                    {
                        return false;
                    }
                }

                for(coordinate& coord : coords)
                {
                    playerTable[coord.convertToIndex()] = playerTableMapping[shp.getName()];
                }

                return true;
            }

            bool removeShip(account_name name1, ship shp)
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
                return isRemoved;
            }

            //TODO : isturn will be handled in battleship class.
            //TODO : is in board will be handled in battleship class.

            bool makemove(account_name name1, coordinate k)
            {
                table& enemyTable = getEnemyTable(name1);

                if(enemyTable[k.convertToIndex()] != enemyTableMapping["unknown"])
                {
                    return false;
                } else{

                    table enemyPlayerTable = getPlayerTable(getOpponent(name1));
                    enemyTable[k.convertToIndex()] = enemyPlayerTable[k.convertToIndex()];
                    round++;
                    return true;
                }

            }



            string getPlacedShips(account_name name1)
            {
                const table& playerTable = getPlayerTable(name1);
                vector<string> strVector;
                stringstream ss;

                for(const uint8_t &i : playerTable)
                {
                    string shipName = reversePlayerTableMapping[i];
                    auto itr = find(strVector.begin(),strVector.end(),shipName);
                    if(isship(shipName) && itr != strVector.end())
                    {
                        strVector.erase(itr);
                    }
                }

                for(string shipName :  strVector)
                {
                    ss << shipName << endl;
                }

                return ss.str();
            }

            string getUnplacedShips(account_name name1)
            {
                const table& playerTable = getPlayerTable(name1);
                vector<string> strVector;
                stringstream ss;

                for(const int &i : playerTable)
                {
                    string shipName = reversePlayerTableMapping[i];
                    if(isship(shipName) && find(strVector.begin(),strVector.end(),shipName) == strVector.end())
                    {
                        strVector.emplace_back(shipName);
                    }
                }

                for(string shipName :  strVector)
                {
                    ss << shipName << endl;
                }

                return ss.str();
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

                string tmp;

                stringstream tableStream(playerTableString);
                stringstream enemyStream(enemyTableString);
                stringstream outputStream;

                vector<string> playerTableAllLines;
                vector<string> enemyTableAllLines;

                outputStream << "\n\t\t" << "Your Table" << "\t\t\t\t" << "            " << "Enemy Table" << "\n\n";

                while(getline(tableStream,tmp))
                    playerTableAllLines.emplace_back(tmp);

                while(getline(enemyStream,tmp))
                    enemyTableAllLines.emplace_back(tmp);


                for(auto it1 = playerTableAllLines.begin(),it2 = enemyTableAllLines.begin();it1 != playerTableAllLines.end(); ++it1,++it2 )
                {
                    outputStream << "\t\t";
                    outputStream << *it1 ;
                    outputStream << "\t\t\t\t";
                    outputStream << *it2 ;
                    outputStream << "\n";
                }

                return outputStream.str();
            }

        private:

            account_name getOpponent(account_name name1)
            {
                if(name1 == host)
                {
                    return compet;
                }else
                {
                    return host;
                }

            }

            string toString(table tbl,map<uint8_t ,char> mapping)
            {
                stringstream ss;

                ss << " ";
                for(int i = 0; i <= 9;i++)
                {
                    ss << " ";
                    ss << i;
                }
                ss << "\n";
                for(int i = 0; i <= 9;i++)
                {
                    ss << i;

                    for(int j = 0; j <= 9;j++)
                    {
                        ss << " ";
                        ss << mapping[tbl[i*10+j]];
                    }
                    ss << "\n";
                }
                return ss.str();
            }

            table& getPlayerTable(account_name name1)
            {
                if(ishost(name1))
                {
                    return hosttable;
                } else{
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


        public:

            EOSLIB_SERIALIZE(game,(host)(compet)(hosttable)(hostenemy)(compettable)(competenemy)(started)(finished)(round));

        };





        static direction stringtodir(string dir) {
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

        static bool isdirectory(string dir)
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

        static bool isship(string shipName)
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



        typedef vector<account_name> userbase;

        typedef multi_index<N(games),game> gamebase;

        gamebase _games;

        userbase _queue;
    };

    EOSIO_ABI(battleship,(enqueue)(dequeue)(ready)(unready)(placeship)(removeship)(placedships)(unplacedships)(makemove)(bothtables)(enemytable)(playertable));
}

