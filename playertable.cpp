//
// Created by oguz on 23.09.2018.
//

#include <sstream>
#include "playertable.h"
#include "ship.cpp"

playertable::playertable()
{

    _initializeMap();
    for(char& c : _playerTable)
    {
        c = '0';
    }
}

void playertable::_initializeMap()
{
    ShipToInt.insert(std::make_pair("empty",'0'));
    ShipToInt.insert(std::make_pair("unknown",'X'));
    ShipToInt.insert(std::make_pair("carrier",'1'));
    ShipToInt.insert(std::make_pair("battleship",'2'));
    ShipToInt.insert(std::make_pair("cruiser",'3'));
    ShipToInt.insert(std::make_pair("submarine",'4'));
    ShipToInt.insert(std::make_pair("destroyer",'5'));
}

bool playertable::placeShip(ship ship1, direction direction1,coordinate coordinate1) {

    if(shipExists(ship1))
    {
        return false;
    }
    else
    {
        bool isPlaced = _placeShip(ship1,direction1,coordinate1);
        return isPlaced;
    }


}

bool playertable::removeShip(ship ship1)
{
    char shipCharacter = ShipToInt[ship1.getName()];
    char emptyCharacter = ShipToInt["empty"];
    bool isRemoved = false;

    for(char & c :_playerTable)
    {
        if(c == shipCharacter)
        {
            c = emptyCharacter;
            isRemoved = true;
        }
    }
    return isRemoved;
}

bool playertable::shipExists(ship ship1)
{
    char shipCharacter = ShipToInt[ship1.getName()];

    for(char &c : _playerTable)
    {
        if(c == shipCharacter)
        {
            return true;
        }
    }
    return false;
}

bool playertable::_placeShip(ship ship1,direction direction1,coordinate coordinate1)
{
    std::vector<coordinate> coords = ship1.getCoords(direction1,coordinate1);
    char shipCharacter = ShipToInt[ship1.getName()];

    for(auto &coord : coords)
    {
        if(!coord.inTable() || _playerTable[coord.convertToIndex()] != '0')
        {
            return false;
        }
    }


    for(auto &coord : coords)
    {
        _playerTable[coord.convertToIndex()] = shipCharacter;
    }

    return true;
}

std::string playertable::toString()
{
    std::stringstream ss;

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
            ss << _playerTable[i*10+j];
        }
        ss << "\n";
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream & os , playertable pt)
{
    std::string ptString = pt.toString();
    os << ptString;
    return os;
}

