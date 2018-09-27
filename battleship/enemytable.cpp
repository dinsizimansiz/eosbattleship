//
// Created by oguz on 24.09.2018.
//

#include <sstream>
#include "enemytable.h"
#include "TableMapping.h"


enemytable::enemytable()
{
    _initializeTable();
}

bool enemytable::isAttacked(coordinate coordinate1) const
{
    int index = coordinate1.convertToIndex();
    bool isAttacked = _enemyTable[index] != TableMapping["unknown"];
    return isAttacked;
}

void enemytable::setTableCell(coordinate coordinate1, char shipCharacter)
{
    int index = coordinate1.convertToIndex();
    _enemyTable[index] = shipCharacter;
}

void enemytable::_initializeTable()
{
    for(char &c : _enemyTable)
    {
        c = TableMapping["unknown"];
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