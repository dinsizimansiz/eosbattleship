//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_SHIP_H
#define BATTLESHIP_SHIP_H

#include <vector>
#include "coordinate.cpp"
#include "direction.h"
class ship {
    
public:
    explicit ship(int);

    const unsigned int getSize() const;

    const std::vector<coordinate> getCoords(direction,coordinate);

private:
    const unsigned int size;

};


#endif //BATTLESHIP_SHIP_H
