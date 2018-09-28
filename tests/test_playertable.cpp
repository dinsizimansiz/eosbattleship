//
// Created by oguz on 24.09.2018.
//

#include "../old/playertable.cpp"

int main()
{
    playertable pt = playertable();
    ship&& battleship = ship("battleship");
    ship&& carrier = ship("carrier");
    ship&& destroyer = ship("destroyer");
    ship&& submarine = ship("submarine");
    ship&& cruiser = ship("cruiser");
    int points = 0;

    //Cannot be placed.
    bool canBePlaced = pt.placeShip(cruiser,UP,coordinate(0,0));

    if(!canBePlaced)
    {
        points += 11;
    }

    //Can be placed.
    canBePlaced = pt.placeShip(cruiser,DOWN,coordinate(0,0));

    if(canBePlaced)
    {
        points += 11;
    }

    //Cannot be placed.
    canBePlaced = pt.placeShip(cruiser,DOWN,coordinate(5,5));

    if(!canBePlaced)
    {
        points += 11;
    }

    //Cannot be placed.
    canBePlaced = pt.placeShip(battleship,DOWN,coordinate(0,1));

    if(!canBePlaced)
    {
        points += 11;
    }

    //Can be placed
    canBePlaced = pt.placeShip(battleship,DOWN,coordinate(5,5));

    if(canBePlaced)
    {
        points += 11;
    }

    //Cannot be removed
    bool canBeRemoved = pt.removeShip(carrier);

    if(!canBeRemoved)
    {
        points += 11;
    }

    //Can be removed
    canBeRemoved = pt.removeShip(battleship);

    if(canBeRemoved)
    {
        points += 11;
    }

    //Cannot be removed.
    canBeRemoved = pt.removeShip(battleship);

    if(!canBeRemoved)
    {
        points += 11;
    }

    //Can be removed
    canBeRemoved = pt.removeShip(cruiser);

    if(canBeRemoved)
    {
        points += 12;
    }

    std::cout << "Current Result\n\n";
    std::cout << pt;

    std::cout << "\n\nCorrect Result\n\n";
    std::cout << "  0 1 2 3 4 5 6 7 8 9" << std::endl;
    std::cout << "1 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "2 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "3 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "4 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "5 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "6 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "7 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "8 0 0 0 0 0 0 0 0 0 0" << std::endl;
    std::cout << "9 0 0 0 0 0 0 0 0 0 0" << std::endl;

    std::cout << std::endl << points << "/100" << std::endl;


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
 *   1-) getCoords(DOWN,Coordinate(0,4))
 *
 *   2-) getCoords(UP,Coordinate(0,8))
 *
 */



    return 0;
}