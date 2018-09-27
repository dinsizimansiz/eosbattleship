//
// Created by oguz on 22.09.2018.
//

#ifndef BATTLESHIP_ITABLEMETHODS_H
#define BATTLESHIP_ITABLEMETHODS_H



#include "ship.h"

class IPlayerTable
{
public:

    virtual bool placeShip(ship,direction,coordinate) = 0;
    
    virtual bool removeShip(ship) = 0;

    virtual bool shipExists(ship) = 0;

};

#endif //BATTLESHIP_ITABLEMETHODS_H
