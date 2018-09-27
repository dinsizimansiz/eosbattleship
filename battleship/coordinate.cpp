//
// Created by oguz on 23.09.2018.
//

#include "coordinate.h"

bool coordinate::inTable()
{
    return coordinate::inTable(*this);
}

bool coordinate::inTable(coordinate coord)
{
    const unsigned int xCoord = coord.getx();
    const unsigned int yCoord = coord.gety();

    bool firstCondition = xCoord >= 0 && xCoord <= 9;
    bool secondCondition = yCoord >= 0 && yCoord <= 9;

    return firstCondition && secondCondition;
}

unsigned int coordinate::gety() const
{
    return y;
}

unsigned int coordinate::getx() const
{
    return x;
}

bool coordinate::operator==(coordinate otherCoord) const
{
    bool firstCondition = this -> getx() == otherCoord.getx();
    bool secondCondition = this -> gety() == otherCoord.gety();

    return firstCondition && secondCondition;
}

coordinate::coordinate(int x, int y):x(x),y(y)
{

}

std::ostream& operator<<(std::ostream& os, const coordinate coord)
{
    os << "Coordinate (" << coord.getx() <<"," << coord.gety()  << ")";
    return os;
}

int coordinate::convertToIndex(coordinate coord)
{
    int index = coord.gety() * 10 + coord.getx();
    return index;
}

int coordinate::convertToIndex() const
{
    return coordinate::convertToIndex(*this);
}

