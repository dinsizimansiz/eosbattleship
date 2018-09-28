//
// Created by oguz on 23.09.2018.
//

#include "../Coordinate.cpp"
#include <iostream>
#include <sstream>

int main()
{
    Coordinate deneme = Coordinate(3,4);
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
    Coordinate xOutside = Coordinate(20,5);
    Coordinate yOutside = Coordinate(9,20);
    Coordinate onEdge = Coordinate(0,9);

    bool firstIsAtOutside = !xOutside.inTable();
    bool secondIsAtOutside = !yOutside.inTable();
    bool thirdIsInTable = onEdge.inTable();

    if(firstIsAtOutside && secondIsAtOutside && thirdIsInTable)
    {
        points += 20;
    }

    if(deneme == Coordinate(3,4))
    {
        points += 10;
    }

    std::stringstream a;
    a << Coordinate(3,4);

    std::string as = a.str();

    if(as == "Coordinate (3,4)"){
        points +=50;
    }

    std::cout << points << "/100." << std::endl;


}
