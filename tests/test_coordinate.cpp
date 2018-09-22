//
// Created by oguz on 23.09.2018.
//

#include "../coordinate.cpp"
#include <iostream>
#include <sstream>

int main()
{
    coordinate deneme = coordinate(3,4);
    int points = 0;

    if(deneme.getx() == 3 && deneme.gety() == 4)
    {
        points += 10;
    }

    bool inTable = deneme.inTable();
    if(inTable)
    {
        points += 10;
    }
    coordinate xOutside = coordinate(20,5);
    coordinate yOutside = coordinate(9,20);
    coordinate onEdge = coordinate(0,9);

    bool firstIsAtOutside = !xOutside.inTable();
    bool secondIsAtOutside = !yOutside.inTable();
    bool thirdIsInTable = onEdge.inTable();

    if(firstIsAtOutside && secondIsAtOutside && thirdIsInTable)
    {
        points += 20;
    }

    if(deneme == coordinate(3,4))
    {
        points += 10;
    }

    std::stringstream a;
    a << coordinate(3,4);

    std::string as = a.str();

    if(as == "Coordinate (3,4)"){
        points +=50;
    }

    std::cout << points << "/100." << std::endl;


}
