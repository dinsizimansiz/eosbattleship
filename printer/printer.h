//
// Created by oguz on 10/3/18.
//

#ifndef BATTLESHIP_PRINTER_H
#define BATTLESHIP_PRINTER_H

#include <eosiolib/eosio.hpp>
#include <vector>
#include <string>
#include "../game/utils.h"

using namespace eosio;
using std::vector;
using std::string;

class printer : public contract{

    printer(account_name self) : contract(self)
    {

    }

    ///@abi action
    void playertable( vector<string> playertable);

    ///@abi action
    void enemytable(vector<string> enemytable);

    ///@abi action
    void alltables(vector<string> playertable , vector<string> enemytable);

    ///@abi action
    void remships(vector<string> playertable);

    ///@abi action
    void placedships(vector<string> playertable);


};


#endif //BATTLESHIP_PRINTER_H
