#ifndef _BUZZER_MANAGER_H_
#define _BUZZER_MANAGER_H_

#include <queue>
#include <string>

#include "LogManager.h"
#include "hardware/LED.h"

#define TASK_DELAY 3000 // 3s

enum class BuzzerState : int {
    PASSIVE,
    ACTIVE
};

class BuzzerRequest {
public:
    BuzzerState state = BuzzerState::PASSIVE;
    size_t duration = 0;

    BuzzerRequest(BuzzerState state, size_t duration) {
        this->state = state;
        this->duration = duration;
    }
};

class BuzzerManager
{
private:
    bool busy = false;
    std::queue<BuzzerRequest> buzzerRequests;
    size_t timeout = 0;

    LogManager *logManager;
public:
    BuzzerManager(LogManager *logManager);

    bool isBusy() {return busy;}
    void addRequest(BuzzerRequest buzzerRequest);
    void addRequest(BuzzerState state, size_t duration);
    void endOfSound();
    void addRepeatingSoundRequest(size_t duration, size_t delay, int repeats);

    void sosSound();
    void successSound();
    void unInterruptableConfirmationSound(double duration = 3.0);

    void confirmationOneSound();
    void confirmationTwoSound();
    void confirmationThreeSound();
    void confirmationFourSound();

    void armedSound();
    void disarmedSound();

    void realTimeDataEnabledSound();
    void realTimeDataDisabledSound();

    void servoSweepSound();

    bool start();
    void run();
    void stop();
};

#endif