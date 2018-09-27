//
// Created by oguz on 24.09.2018.
//

#include "../battleship/enemytable.cpp"
#include "../battleship/coordinate.cpp"

int main()
{
    enemytable et = enemytable();
    int points = 0;

    et.setTableCell(coordinate(0,0),"empty");
    et.setTableCell(coordinate(0,1),"empty");

    bool isAttacked = et.isAttacked(coordinate(0,1));

    if(isAttacked)
    {
        points += 50;
    }

    isAttacked = et.isAttacked(coordinate(2,2));

    if(!isAttacked)
    {
        points += 50;
    }

    std::cout << points << "/100" << std::endl;

    return 0;
}