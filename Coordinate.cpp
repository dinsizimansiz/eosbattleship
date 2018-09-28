//
// Created by oguz on 23.09.2018.
//

#include "Coordinate.h"


    bool Coordinate::inTable() {
        return Coordinate::inTable(*this);
    }

    bool Coordinate::inTable(Coordinate coord) {
        const unsigned int xCoord = coord.getx();
        const unsigned int yCoord = coord.gety();

        bool firstCondition = xCoord >= 0 && xCoord <= 9;
        bool secondCondition = yCoord >= 0 && yCoord <= 9;

        return firstCondition && secondCondition;
    }

    unsigned int Coordinate::gety() const {
        return y;
    }

    unsigned int Coordinate::getx() const {
        return x;
    }

    bool Coordinate::operator==(Coordinate otherCoord) const {
        bool firstCondition = this->getx() == otherCoord.getx();
        bool secondCondition = this->gety() == otherCoord.gety();

        return firstCondition && secondCondition;
    }

    Coordinate::Coordinate(int x, int y) : x(x), y(y) {

    }

    std::ostream &operator<<(std::ostream &os, const Coordinate coord) {
        os << "Coordinate (" << coord.getx() << "," << coord.gety() << ")";
        return os;
    }

    int Coordinate::convertToIndex(Coordinate coord) {
        int index = coord.gety() * 10 + coord.getx();
        return index;
    }

    int Coordinate::convertToIndex() const {
        return Coordinate::convertToIndex(*this);
    }
