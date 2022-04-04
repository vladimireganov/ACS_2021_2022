#ifndef BUZZER_H
#define BUZZER_H
#include "LED.h"
#include "ArduinoTimer.h"

class Buzzer {
    private:
        int pin = 5;
        unsigned int end_duration = millis();

        bool active = false;
        bool task = false;

        int frequency = 0;
        int duration = 0;
        int sound_repeats = 0;
        unsigned int sound_delay = 0;

    
        void create(int frequency, int duration) {
            if (!active) {
                active = true;
                buzzOn();
                end_duration = millis() + duration;
            }
        }

        void create_repeating(int frequency, int duration, int sound_repeats, int sound_delay) {
            if (!active) {
                this->frequency = frequency;
                this->duration = duration;
                this->sound_repeats = sound_repeats;
                this->sound_delay = sound_delay;

                active = true;
                buzzOn();
                
                end_duration = millis() + duration;
            }
        }

    public:
    Buzzer() {}

    Buzzer(int pin) {
        this->pin = pin;
    }

    void delay(int duration) {
        usleep(duration*10e3);
    }

    void run() {
        if (millis() > end_duration) {
            
            if (sound_repeats > 0) {
                if (!active) {
                    create_repeating(this->frequency, this->duration, this->sound_repeats, this->sound_delay);
                } else {
                    active = false;
                    buzzOff();

                    sound_repeats--;

                    if (sound_repeats > 0) {
                        end_duration = millis() + sound_delay;
                    } else {
                        task = false;
                    }
                }
            } else {
                active = false;
                task = false;
                buzzOff();
            }
        }
    }

    void sound(int frequency, int duration) {
        if (!task) {
            active = true;
            buzzOn();
            delay(duration);
            buzzOff();
            active = false;
        }
    }

    void multiple_sound(int frequency, int duration, int repeats, int sound_delay) {
        if (!task) {
            task = true;
            for (int i = 0; i < repeats; i++) {
                buzzOn();
                delay(duration);
                buzzOff();
                delay(sound_delay);
            }
            task = false;
        }
    }

    void async_sound(int frequency, int duration) {
        if(!task) {
            task = true;
            create(frequency, duration);
        }
    }

    void multiple_async_sound(int frequency, int duration, int sound_repeats, int sound_delay) {
        if (!task) {
            task = true;
            create_repeating(frequency, duration, sound_repeats, sound_delay);
        }
    }
};
#endif