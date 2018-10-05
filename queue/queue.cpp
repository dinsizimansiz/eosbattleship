//
// Created by oguz on 10/1/18.
//

#include "queue.h"

void queue::enqueue(account_name name) {
    auto itr = std::find(_queue.begin(),_queue.end(),name);
    eosio_assert(itr == _queue.end(),"User is already in queue.");
    _queue.emplace_back(name);

    if(_queue.size() == 2)
    {
        account_name host = _queue[0];
        account_name challenger = _queue[1];
        _queue.clear();
        creategame(host,challenger);
    }

}

void queue::dequeue(account_name name) {
    auto itr = std::find(_queue.begin(),_queue.end(),name);
    eosio_assert(itr != _queue.end(),"User is not in list.");

    _queue.erase(itr);
}

void queue::creategame(account_name host, account_name challenger) {

    action(
            permission_level {get_self(),N(active)},
            N(battleship),
            N(creategame),
            std::make_tuple(host,challenger)
    ).send();
}