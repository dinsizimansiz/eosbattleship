//
// Created by oguz on 23.09.2018.
//

#include <sstream>
#include "playertable.h"
#include "ship.cpp"

playertable::playertable()
{
    _initalizeTable();
}

void playertable::_initalizeTable()
{
    for(char& c : _playerTable)
    {
        c = TableMapping["empty"];
    }
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
    char shipCharacter = TableMapping[ship1.getName()];
    char emptyCharacter = TableMapping["empty"];
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
    char shipCharacter = TableMapping[ship1.getName()];

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
    char shipCharacter = TableMapping[ship1.getName()];

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

const char playertable::operator[](int index) const
{
    const char returnChar = _playerTable[index];
    return returnChar;
}

const char playertable::operator[](coordinate coordinate1) const
{
    int index = coordinate1.convertToIndex();
    const char returnChar = _playerTable[index];
    return returnChar;
}


std::ostream& operator<<(std::ostream & os , playertable pt)
{
    std::string ptString = pt.toString();
    os << ptString;
    return os;
}



