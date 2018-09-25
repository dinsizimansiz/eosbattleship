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

    void setCell(coordinate, char);

    bool removeShip(ship&);

    bool placedAllShips() const;

    bool getReady();

    std::vector<ship> getRemainingShips() const;

    void getUnready();

    bool isAttacked(coordinate&) const ;

    void hitShip();

    unsigned int getNumberOfHits() const;

private:

    void _initializeShipYard();

    enemytable _enemyTable;

    playertable _playerTable;

    std::vector<ship> _shipYard;

    bool ready;

    unsigned int numberOfHits;
};


#endif //BATTLESHIP_BATTLESHIPPLAYER_H
