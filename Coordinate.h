//
// Created by oguz on 23.09.2018.
//

#ifndef BATTLESHIP_COORDINATE_H
#define BATTLESHIP_COORDINATE_H

//Includes business logic functions

class Coordinate {
public:

    Coordinate(int x, int y);

    unsigned int getx() const;

    unsigned int gety() const;

    static bool inTable(Coordinate);

    bool inTable();

    bool operator==(Coordinate) const;

    static int convertToIndex(Coordinate);

    int convertToIndex() const;

private:
    unsigned int x;
    unsigned int y;

};

#endif //BATTLESHIP_COORDINATE_H
