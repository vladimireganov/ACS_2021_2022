#ifndef ARDUINO_TIMER
#define ARDUINO_TIMER
#include <chrono>

extern std::chrono::time_point<std::chrono::high_resolution_clock> start_chrono_time;

double seconds();

size_t millis();

size_t micros();

#endif