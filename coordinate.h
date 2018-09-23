//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_COORDINATE_H
#define BATTLESHIP_COORDINATE_H

#include <iostream>

//Includes business logic functions


class coordinate {
public:

    coordinate(int x,int y);

    const unsigned int getx() const;

    const unsigned int gety() const ;

    static bool inTable(coordinate) ;

    bool inTable();

    friend std::ostream& operator<<(std::ostream&, coordinate);

    bool operator==(coordinate) const ;





private:
    const unsigned int x ;
    const unsigned int y ;

};


#endif //BATTLESHIP_COORDINATE_H
