//
// Created by oguz on 24.09.2018.
//

#include <sstream>
#include "enemytable.h"



bool enemytable::isAttacked(coordinate coordinate1)
{
    int index = coordinate1.convertToIndex();
    bool isAttacked = _enemyTable[index] != tableMapping["unknown"];
    return isAttacked;
}

void enemytable::setTableCell(coordinate coordinate1, char shipCharacter)
{
    int index = coordinate1.convertToIndex();
    _enemyTable[index] = shipCharacter;
}

enemytable::enemytable()
{
    _initalizeMap();
    _initializeTable();
}

void enemytable::_initalizeMap()
{
    tableMapping.insert(std::make_pair("empty",'0'));
    tableMapping.insert(std::make_pair("unknown",'X'));
    tableMapping.insert(std::make_pair("carrier",'1'));
    tableMapping.insert(std::make_pair("battleship",'2'));
    tableMapping.insert(std::make_pair("cruiser",'3'));
    tableMapping.insert(std::make_pair("submarine",'4'));
    tableMapping.insert(std::make_pair("destroyer",'5'));
}

void enemytable::_initializeTable()
{
    for(char &c : _enemyTable)
    {
        c = tableMapping["unknown"];
    }
}

std::string enemytable::toString()
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
            ss << _enemyTable[i*10+j];
        }
        ss << "\n";
    }
    return ss.str();
}

std::ostream& operator<<(std::ostream& os,enemytable& et)
{
    std::string etString = et.toString();
    os << etString;
    return os;
}