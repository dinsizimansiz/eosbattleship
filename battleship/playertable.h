//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_GAMETABLE_H
#define BATTLESHIP_GAMETABLE_H

#include "IPlayerTable.h"
#include "IPrintMethods.h"
#include "TableMapping.h"
#include <map>


class playertable : public IPlayerTable{


public:

    playertable();

    bool placeShip(ship ,direction, coordinate ) override;

    bool removeShip(ship ) override;

    bool shipExists(ship) override;

    std::string toString() ;

    friend std::ostream& operator<<(std::ostream&, playertable);

    const char operator[](int) const;

    const char operator[](coordinate) const ;

private:

    bool _placeShip(ship,direction,coordinate);

    void _initalizeTable();

    char _playerTable[100];

};



#endif //BATTLESHIP_GAMETABLE_H
