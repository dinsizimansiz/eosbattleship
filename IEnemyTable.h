//
// Created by oguz on 24.09.2018.
//

#ifndef BATTLESHIP_IENEMYTABLE_H
#define BATTLESHIP_IENEMYTABLE_H

#include "coordinate.h"
#include <vector>

class IEnemyTable
{
public:

    IEnemyTable() = delete;

    bool makeMove(coordinate) = 0;

protected:
    std::vector<int>  enemyTable;
};

#endif //BATTLESHIP_IENEMYTABLE_H
