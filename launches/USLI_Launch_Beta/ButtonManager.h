#ifndef _BUTTON_MANAGER_H_
#define _BUTTON_MANAGER_H_

#include <vector>
#include "ArduinoTimer.h"
#include "SharedData.h"
#include "LogManager.h"

#include "hardware/LED.h"
#include "hardware/SerialLinux.h"

#define MIN_TIME_TO_START_PROCESSING 500 //ms

#define HALF_SECOND 500
#define ONE_SECOND 1000
#define THREE_SECONDS 3000
#define FIVE_SECONDS 5000

enum class ButtonState {
    OFF,
    ON
};

enum class ButtonRequest {
    CLICK,
    LONG_CLICK,
    HOLD,
    LONG_HOLD
};


class ButtonEvent {
public:
    ButtonState state;
    size_t elapsedTime;

    ButtonEvent(ButtonState state, size_t elapsedTime) {
        this->state = state;
        this->elapsedTime = elapsedTime;
    }
};


class ButtonManager
{
private:
    size_t startTime;
    ButtonState previousButtonState = ButtonState::OFF;

    std::vector<ButtonEvent> previousEvents;
    std::vector<ButtonRequest> buttonRequests;

    Configuration *configuration;
    LogManager *logManager;

    void handleClickRequest();
    void handleLongClickRequest();
    void handleHoldRequest();
    void handleLongHoldRequest();
public:
    ButtonManager(Configuration *configuration, LogManager *logManager);

    void listen();
    void process();
    void handle();

    bool start();
    void run();
    void stop();
};



#endif _BUTTON_MANAGER_H_