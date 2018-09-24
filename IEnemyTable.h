//
// Created by oguz on 24.09.2018.
//

#ifndef BATTLESHIP_IENEMYTABLE_H
#define BATTLESHIP_IENEMYTABLE_H

#include "coordinate.h"

class IEnemyTable
{
public:

    virtual bool isAttacked(coordinate) = 0;

    virtual void setTableCell(coordinate,std::string) = 0 ;




};

#endif //BATTLESHIP_IENEMYTABLE_H
