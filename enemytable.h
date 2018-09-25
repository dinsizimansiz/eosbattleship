//
// Created by oguz on 24.09.2018.
//

#ifndef BATTLESHIP_ENEMYTABLE_H
#define BATTLESHIP_ENEMYTABLE_H


#include "IEnemyTable.h"
#include <map>
#include "TableMapping.h"


class enemytable : IEnemyTable
{
public:

    enemytable();

    bool isAttacked(coordinate ) const override  ;

    void setTableCell(coordinate , char ) override;

    std::string toString() ;

    friend std::ostream& operator<<(std::ostream&,enemytable&);

private:

    void _initializeTable();

    char _enemyTable[100];

};


#endif //BATTLESHIP_ENEMYTABLE_H
