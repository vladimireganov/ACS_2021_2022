//
// Created by jtc19 on 1/23/2022.
//

#include <chrono>
using namespace std::chrono;

void delay(long millis) {

    long finished = millis;
    long elapsed;
    time_point<system_clock> t1 = system_clock::now();
    time_point<system_clock> t2;
    do {

        t2 = system_clock::now();
        elapsed = duration_cast<milliseconds>(t2 - t1).count();


    }
    while(elapsed < finished);

}

