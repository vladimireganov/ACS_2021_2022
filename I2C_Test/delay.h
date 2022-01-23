//
// Created by jtc19 on 1/23/2022.
//

#include <chrono>
using namespace std:chrono

void delay(long millis) {

    long finished = millis;
    long elapsed = 0
    std::chrono::time_point<std::chrono::steady_clock> t1 = std::chrono::steady_clock::now();
    std::chrono::time_point<std::chrono::steady_clock> t2;
    do {

        t2 = std::chrono::steady_clock::now();
        elapsed = std::chrono:duration_cast<milliseconds>(t2 - t1).count();


    }
    while(elapsed < finished);

}
