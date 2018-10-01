//
// Created by oguz on 10/1/18.
//

#ifndef BATTLESHIP_QUEUE_H
#define BATTLESHIP_QUEUE_H

#include <eosiolib/eosio.hpp>
#include <string>
#include <vector>


using namespace eosio;
using std::string;
using std::vector;

class queue : public contract{

public :

    queue(account_name self) : contract(self)
    {

    }

    //@abi action
    void enqueue(account_name player);

    //@abi action
    void dequeue(account_name player);



private:

    typedef  vector<account_name> playerbase;

    playerbase _queue;

    void creategame(account_name host,account_name challenger);


};

EOSIO_ABI(queue,(enqueue)(dequeue))
#endif //BATTLESHIP_QUEUE_H
