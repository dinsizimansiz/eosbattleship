//
// Created by oguz on 24.09.2018.
//

#ifndef BATTLESHIP_BATTLESHIPPLAYER_H
#define BATTLESHIP_BATTLESHIPPLAYER_H

#include "IPrintMethods.h"
#include "enemytable.h"
#include "playertable.h"

class battleshipplayer : IPrintMethods
{
public:

    battleshipplayer();

    std::string printenemytable() override;

    std::string printplayertable() override;

    std::string printbothtables() override;

    char getCell(coordinate) const ;

    bool placeShip(ship&,coordinate,direction);

    void setCell(coordinate,char);

    bool removeShip(ship&);

private:

    enemytable _enemyTable;

    playertable _playerTable;

};


#endif //BATTLESHIP_BATTLESHIPPLAYER_H
