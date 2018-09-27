//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_IPRINTMETHODS_H
#define BATTLESHIP_IPRINTMETHODS_H

#include <string>

class IPrintMethods
{
public:

    virtual std::string printenemytable() = 0;

    virtual std::string printplayertable() = 0;

    virtual std::string printbothtables() = 0;

};

#endif //BATTLESHIP_IPRINTMETHODS_H
