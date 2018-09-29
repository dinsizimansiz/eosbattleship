//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include <vector>
#include "Coordinate.cpp"
#include "Direction.h"
#include <string>

#define __CARRIER_SIZE__ 5
#define __BATTLESHIP_SIZE__ 4
#define __CRUISER_SIZE__ 3
#define __SUBMARINE_SIZE__ 3
#define __DESTROYER_SIZE__ 2


class Ship {

public:

    explicit Ship(std::string &);

    explicit Ship(std::string &&);

    unsigned int getSize() const;

    std::string getName() const;

    const std::vector<Coordinate> getCoords(Direction, Coordinate);

    bool operator==(Ship) const;

    friend std::ostream &operator<<(std::ostream &, const Ship &);

private:

    void _createShip();

    unsigned int size;

    std::string shipName;

};


#endif //BATTLESHIP_SHIP_H
