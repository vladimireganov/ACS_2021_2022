#include "ArduinoTimer.h"

std::chrono::time_point<std::chrono::high_resolution_clock> start_chrono_time = std::chrono::high_resolution_clock::now();

double seconds() {
    auto current_chrono_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::seconds>(current_chrono_time - start_chrono_time).count();
}

size_t millis() {
    auto current_chrono_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(current_chrono_time - start_chrono_time).count();
}

size_t micros() {
    auto current_chrono_time = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(current_chrono_time - start_chrono_time).count();
}