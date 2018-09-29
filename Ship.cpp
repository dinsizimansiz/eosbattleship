//
// Created by oguz on 23.09.2018.
//
#include "Ship.h"
#include "Coordinate.h"


Ship::Ship(std::string &shipName) : shipName(shipName) {
    _createShip();
}

Ship::Ship(std::string &&shipName) : shipName(shipName) {
    _createShip();
}

const std::vector<Coordinate> Ship::getCoords(Direction dir, Coordinate coords) {
    std::vector<Coordinate> returnVector;
    const int xCoord = coords.getx();
    const int yCoord = coords.gety();

    switch (dir) {
        case UP :
            for (int i = 0; i < size; i++) {
                returnVector.emplace_back(xCoord, yCoord - i);
            }
            break;

        case RIGHT:
            for (int i = 0; i < size; i++) {
                returnVector.emplace_back(xCoord + i, yCoord);
            }
            break;

        case DOWN:
            for (int i = 0; i < size; i++) {
                returnVector.emplace_back(xCoord, yCoord + i);
            }
            break;

        case LEFT:
            for (int i = 0; i < size; i++) {
                returnVector.emplace_back(xCoord - i, yCoord);
            }
    }

    return returnVector;
}

unsigned int Ship::getSize() const {
    return size;
}

std::string Ship::getName() const {
    return shipName;
}

void Ship::_createShip() {
    if (shipName == "carrier") {
        size = __CARRIER_SIZE__;
    } else if (shipName == "battleship") {
        size = __BATTLESHIP_SIZE__;
    } else if (shipName == "cruiser") {
        size = __CRUISER_SIZE__;
    } else if (shipName == "submarine") {
        size = __SUBMARINE_SIZE__;
    } else if (shipName == "destroyer") {
        size = __DESTROYER_SIZE__;
    }
}

bool Ship::operator==(Ship ship1) const {
    return shipName == ship1.getName();
}



/*
 *   0 1 2 3 4 5 6 7 8 9
 * 0 0 0 0 0 0 0 0 0 0 0
 * 1 0 0 0 0 0 0 0 0 0 0
 * 2 0 0 0 0 0 0 0 0 0 0
 * 3 0 0 0 0 0 0 0 0 0 0
 * 4 x 0 0 0 0 0 0 0 0 0
 * 5 x 0 0 0 0 0 0 0 0 0
 * 6 x 0 0 0 0 0 0 0 0 0
 * 7 x 0 0 0 0 0 0 0 0 0
 * 8 x 0 0 0 0 0 0 0 0 0
 * 9 0 0 0 0 0 0 0 0 0 0
 *
 *  this placement can be done in two ways :
 *
 *   1-) getCoords(DOWN,Coordinate(0,4))
 *
 *   2-) getCoords(UP,Coordinate(0,8))
 *
 */