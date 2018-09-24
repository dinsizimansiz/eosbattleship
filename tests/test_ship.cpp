//
// Created by oguz on 23.09.2018.
//

#include "../ship.cpp"

int main()
{
    unsigned int points = 0;

    ship a("submarine");

    if(a.getSize() == 3)
    {
        points += 5;
    }

    std::vector<coordinate> resultVector ;

    std::vector<coordinate> upCoords = a.getCoords(UP,coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(3,4);
    resultVector.emplace_back(3,3);

    if(resultVector == upCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::vector<coordinate> rightCoords = a.getCoords(RIGHT,coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(4,5);
    resultVector.emplace_back(5,5);

    if(resultVector == rightCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::vector<coordinate> downCoords = a.getCoords(DOWN,coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(3,6);
    resultVector.emplace_back(3,7);

    if(resultVector == downCoords)
    {
        points += 19;
    }

    resultVector.clear();

    std::vector<coordinate> leftCoords = a.getCoords(LEFT,coordinate(3,5));
    resultVector.emplace_back(3,5);
    resultVector.emplace_back(2,5);
    resultVector.emplace_back(1,5);

    if(resultVector == leftCoords)
    {
        points += 19;
    }

    resultVector.clear();

    //Handling of invalid coordinates isn't waited from ship class.
    std::vector<coordinate> invalidCoords = a.getCoords(UP,coordinate(-1,-1));
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