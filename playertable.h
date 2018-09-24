//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_GAMETABLE_H
#define BATTLESHIP_GAMETABLE_H

#include "IPlayerTable.h"
#include "IPrintMethods.h"
#include <map>


class playertable : public IPlayerTable{


public:

    playertable();

    bool placeShip(ship ,direction, coordinate ) override;

    bool removeShip(ship ) override;

    bool shipExists(ship) override;

    std::string toString() ;

    friend std::ostream& operator<<(std::ostream&, playertable);

private:

    bool _placeShip(ship,direction,coordinate);

    void _initializeMap() ;

    std::map<std::string,char> ShipToInt;

    char _playerTable[100];

};



#endif //BATTLESHIP_GAMETABLE_H
