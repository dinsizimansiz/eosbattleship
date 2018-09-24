//
// Created by oguz on 24.09.2018.
//

#ifndef BATTLESHIP_ENEMYTABLE_H
#define BATTLESHIP_ENEMYTABLE_H


#include "IEnemyTable.h"
#include <map>

class enemytable : IEnemyTable
{
public:

    enemytable();

    bool isAttacked(coordinate coordinate1) override;

    void setTableCell(coordinate coordinate1, std::string string) override;

    std::string toString() ;

    friend std::ostream& operator<<(std::ostream&,enemytable);

private:

    void _initalizeMap();

    void _initializeTable();

    char _enemyTable[100];

    std::map<std::string,char>  tableMapping;
};


#endif //BATTLESHIP_ENEMYTABLE_H
