//
// Created by oguz on 25.09.2018.
//

#ifndef BATTLESHIP_TABLEMAPPING_H
#define BATTLESHIP_TABLEMAPPING_H
#include <vector>
#include <string>

std::vector<std::string,char> TableMapping = {
        std::make_pair("empty",'0'),
        std::make_pair("unknown",'X'),
        std::make_pair("carrier",'1'),
        std::make_pair("battleship",'2'),
        std::make_pair("cruiser",'3'),
        std::make_pair("submarine",'4'),
        std::make_pair("destroyer",'5')
};



#endif //BATTLESHIP_TABLEMAPPING_H
