//
// Created by oguz on 10/1/18.
//


#pragma once


#include <map>
#include <vector>
#include <string>
#include <tuple>

using std::tuple;
using std::string;
using std::vector;
using std::make_tuple;
using std::map;
using std::get;

map<string,string> tableMapping = {{"empty","0"},{"unknown","X"},{"cruiser","1"},{"battleship","2"},{"submarine","3"},{"cruiser","4"},{"destroyer","5"}};


struct Utils {



    static uint8_t shipsize(string shipname) {
        if (shipname == "carrier")
        {
            return 5;
        }
        else if (shipname == "battleship")
        {
            return 4;
        }
        else if (shipname == "submarine")
        {
            return 3;
        }
        else if (shipname == "cruiser")
        {
            return 3;
        }
        else if (shipname == "destroyer")
        {
            return 2;
        }
    }

    static string getshipname(string shipchar) {
        if (shipchar == "1") {
            return "carrier";
        } else if (shipchar == "2") {
            return "battleship";
        } else if (shipchar == "3") {
            return "submarine";
        } else if (shipchar == "4") {
            return "cruiser";
        } else if (shipchar == "5") {
            return "destroyer";
        }
    }

    static inline uint8_t convertoindex(uint8_t x, uint8_t y) {
        return y * 10 + x;
    }

    static vector <tuple<uint8_t ,uint8_t >> getcoords(string shipname, uint8_t x, uint8_t y, string direction) {
        uint8_t shipwidth = shipsize(shipname);
        vector <tuple<uint8_t,uint8_t>> retVector;
        if (direction == "n" || direction == "north") {
            for (uint8_t i = 0; i < shipwidth; i++)
                retVector.emplace_back(x,y-i);

        } else if (direction == "e" || direction == "east") {
            for (uint8_t i = 0; i < shipwidth; i++)
                retVector.emplace_back(x+i,y);

        } else if (direction == "s" || direction == "south") {
            for (uint8_t i = 0; i < shipwidth; i++)
                retVector.emplace_back(x,y+i);

        } else if (direction == "w" || direction == "west") {
            for (uint8_t i = 0; i < shipwidth; i++)
                retVector.emplace_back(x-i,y);

        }
        return retVector;
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

};