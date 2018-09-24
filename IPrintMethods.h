//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_IPRINTMETHODS_H
#define BATTLESHIP_IPRINTMETHODS_H

#include <string>

class IPrintMethods{
public:

    IPrintMethods() = delete;

    virtual std::string PrintEnemyTable() = 0;

    virtual std::string PrintPlayerTable() = 0;

    virtual std::string PrintBothTables() = 0;
};

#endif //BATTLESHIP_IPRINTMETHODS_H
