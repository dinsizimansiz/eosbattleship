//
// Created by oguz on 23.09.2018.
//

#include "../Ship.cpp"

int main()
{
    unsigned int points = 0;

    Ship a("submarine");

    if(a.getSize() == 3)
    {
        points += 5;
    }

    std::vector<Coordinate> resultVector ;

    std::vector<Coordinate> upCoords = a.getCoords(UP,Coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(3,4);
    resultVector.emplace_back(3,3);

    if(resultVector == upCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::vector<Coordinate> rightCoords = a.getCoords(RIGHT,Coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(4,5);
    resultVector.emplace_back(5,5);

    if(resultVector == rightCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::vector<Coordinate> downCoords = a.getCoords(DOWN,Coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(3,6);
    resultVector.emplace_back(3,7);

    if(resultVector == downCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::vector<Coordinate> leftCoords = a.getCoords(LEFT,Coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(2,5);
    resultVector.emplace_back(1,5);

    if(resultVector == leftCoords)
    {
        points += 19;
    }

    resultVector.clear();

    //Handling of invalid coordinates isn't waited from Ship class.
    std::vector<Coordinate> invalidCoords = a.getCoords(UP,Coordinate(-1,-1));
    resultVector.emplace_back(-1,-1);
    resultVector.emplace_back(-1,-2);
    resultVector.emplace_back(-1,-3);

    if(resultVector == invalidCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::cout << points << "/100" << std::endl;

    return 0;
}