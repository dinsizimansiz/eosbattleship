//
// Created by oguz on 24.09.2018.
//

#include "sstream"
#include <string>
#include "battleshipplayer.h"
#include "playertable.cpp"
#include "enemytable.cpp"

battleshipplayer::battleshipplayer() :_playerTable(),_enemyTable()
{

}

std::string battleshipplayer::printenemytable()
{
    return _enemyTable.toString();
}

std::string battleshipplayer::printplayertable()
{
    return _playerTable.toString();
}

std::string battleshipplayer::printbothtables()
{
    std::string playerTableString = _playerTable.toString();
    std::string enemyTableString = _enemyTable.toString();

    std::string tmp;

    std::stringstream tableStream(playerTableString);
    std::stringstream enemyStream(enemyTableString);
    std::stringstream outputStream;

    std::vector<std::string> playerTableAllLines;
    std::vector<std::string> enemyTableAllLines;

    outputStream << "\n\t\t" << "Your Table" << "\t\t\t\t" << "            " << "Enemy Table" << "\n\n";

    while(std::getline(tableStream,tmp))
        playerTableAllLines.emplace_back(tmp);

    while(std::getline(enemyStream,tmp))
        enemyTableAllLines.emplace_back(tmp);


    for(auto it1 = playerTableAllLines.begin(),it2 = enemyTableAllLines.begin();it1 != playerTableAllLines.end(); ++it1,++it2 )
    {
        outputStream << "\t\t";
        outputStream << *it1 ;
        outputStream << "\t\t\t\t";
        outputStream << *it2 ;
        outputStream << "\n";
    }

    return outputStream.str();
}

char battleshipplayer::getCell(coordinate coordinate1) const
{
    int index = coordinate1.convertToIndex();
    return _playerTable[index];
}

bool battleshipplayer::placeShip(ship& ship1, coordinate coordinate1, direction direction1)
{
    _playerTable.placeShip(ship1,direction1,coordinate1);
}

void battleshipplayer::setCell(coordinate coordinate1, char addedChar)
{
    _enemyTable.setTableCell(coordinate1,addedChar);
}

bool battleshipplayer::removeShip(ship& ship1)
{
    _playerTable.removeShip(ship1);
}
