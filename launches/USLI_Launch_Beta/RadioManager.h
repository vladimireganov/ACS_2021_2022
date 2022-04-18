#ifndef _RADIO_MANAGER_H_
#define _RADIO_MANAGER_H_

#include <string>
#include <vector>
#include "hardware/SerialLinux.h"
#include "LogManager.h"

typedef struct requests
{
    bool run_servo_sweep = false;
    bool send_real_time_data = false;
    bool arm = false;
    bool shutdown = false;
} Requests;


class RadioManager
{
private:
    const char START_BYTE = ":";
    const char END_BYTE = ";";

    std::vector<std::string> decoded_requests;

    std::string buffer = "";
    Requests *groundRequests;

    void collect();
    void handle();
public:
    RadioManager(Requests *groundRequests, LogManager *logManager);

    bool start();
    void run();
    void stop();
};

#endif