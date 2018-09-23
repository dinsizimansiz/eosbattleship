//
// Created by oguz on 23.09.2018.
//

#include "ship.h"
#include "coordinate.h"
ship::ship(int size):size(size)
{

}

const std::vector<coordinate> ship::getCoords(direction dir, coordinate coords)
{
    std::vector<coordinate> returnVector;
    const  int xCoord = coords.getx();
    const  int yCoord = coords.gety();

    switch(dir)
    {
        case UP :
            for(int i = 0;i < size ; i++)
            {
                returnVector.emplace_back(xCoord,yCoord - i);
            }
            break;

        case RIGHT:
            for(int i = 0;i < size; i++)
            {
                returnVector.emplace_back(xCoord + i, yCoord );
            }
            break;

        case DOWN:
            for(int i = 0;i < size; i++)
            {
                returnVector.emplace_back(xCoord,yCoord+i);
            }
            break;

        case LEFT:
            for(int i = 0; i < size ; i++)
            {
                returnVector.emplace_back(xCoord - i,yCoord);
            }
    }

    return returnVector;
}

const unsigned int ship::getSize() const
{
    return size;
}

/*
 *   0 1 2 3 4 5 6 7 8 9
 * 0 0 0 0 0 0 0 0 0 0 0
 * 1 0 0 0 0 0 0 0 0 0 0
 * 2 0 0 0 0 0 0 0 0 0 0
 * 3 0 0 0 0 0 0 0 0 0 0
 * 4 x 0 0 0 0 0 0 0 0 0
 * 5 x 0 0 0 0 0 0 0 0 0
 * 6 x 0 0 0 0 0 0 0 0 0
 * 7 x 0 0 0 0 0 0 0 0 0
 * 8 x 0 0 0 0 0 0 0 0 0
 * 9 0 0 0 0 0 0 0 0 0 0
 *
 *  this placement can be done in two ways :
 *
 *   1-) getCoords(DOWN,coordinate(0,4))
 *
 *   2-) getCoords(UP,coordinate(0,8))
 *
 */