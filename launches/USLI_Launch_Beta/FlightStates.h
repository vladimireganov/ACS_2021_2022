/**
 * @file flight_states.h
 * @author Kazybek Mizam (kzm0099@auburn.edu)
 * @brief Controller library of rocket flight states
 * used for transitioning from one flight state to another that is
 * needed to make some procedures in exact states
 * 
 * There are 4 main states
 * 1. Lauched - motor ignited rocket launched
 * 2. Burnt out - motor burnt out, free fall with initial velocity
 * 3. Apogee - reached highest altitude, fixed after some dt
 * 4. Landed - no altitude change and movement of rocket
 * 
 * @version 0.3
 * @date 2022-04-09
 * 
 * @copyright Copyright (c) AURA Embedded Systems 2021
 * 
 */

#ifndef ROCKET_FLIGHT_STATES_H
#define ROCKET_FLIGHT_STATES_H
#include <math.h>
#include <stdlib.h>

#define LAUNCH_GUARD_TIMER 0.2f                      // in seconds
#define LAUNCH_NET_ACCELERATION_THRESHOLD 30.0f      // m/s^2
#define LAUNCH_VERTICAL_VELOCITY_THRESHOLD 5.0f      // m/s
#define LAUNCH_ALTITUDE_THRESHOLD 10.0f              // meter

#define BURNT_OUT_GUARD_TIMER 0.2f                   // in seconds
#define BURNT_OUT_ALTITUDE_THRESHOLD 50.0f           // meter
#define BURNT_OUT_NET_ACCELERATION_THRESHOLD 11.0f   // m/s^2

#define APOGEE_GUARD_TIMER 5.0f                      // in seconds
#define APOGEE_ALTITUDE_DIFFERENCE_MINIMUM 3.0f      // meter

#define LAND_GUARD_TIMER 5.0f                        // in seconds
#define LAND_MAX_ALTITUDE_DIFFERENCE 2.0f            // meter
#define LAND_MAX_ACCELERATION_VIBRATION 12.0f        // m/s^2

enum FlightState {
    PRE_LAUNCH,
    LAUNCHED,
    BURNT_OUT,
    LANDING,
    LANDED,
};

class RocketFlightStates {
    float elapsed_time;
    float max_altitude;
    float land_altitude;

public:
    FlightState current_state = PRE_LAUNCH;
    FlightState previous_state = PRE_LAUNCH;
    
    void setup_timer(float current_time) {
        elapsed_time = current_time;
    }

    /**
     * @brief Function that checks wether rocket has launched, 
     * uses either altitude change or vertical motion detection
     * 
     * @param current_altitude current altitude
     * @param net_acceleration (x^2+y^2+z^2)^(1/2) of acceleration
     * @param current_time current board time or any elapsing time unit
     * @return true if laucnhed
     * @return false if not launched
     */
    bool if_launched_by_altitude(float current_altitude, float net_acceleration, float current_time) {
        // (vertical_acceleration >= 20) && (dh >= 50) // for testing
        if (net_acceleration >= LAUNCH_NET_ACCELERATION_THRESHOLD 
            && (current_altitude >= LAUNCH_ALTITUDE_THRESHOLD)) {
            if (elapsed_time + LAUNCH_GUARD_TIMER < current_time) {
                return true;
            }
            return false;
        }
        elapsed_time = current_time;
        return false;
    }

    /**
     * @brief Function that checks wether rocket motor burnt out, 
     * uses either altitude change or negative acceleration
     * 
     * @param current_altitude current altitude
     * @param net_acceleration (x^2+y^2+z^2)^(1/2) of acceleration
     * @param current_time current board time or any elapsing time unit
     * @return true if motor burnt out
     * @return false if motor not burnt out
     */
    bool if_burnt_out(float current_altitude, float net_acceleration,
        float current_time) {
        // return false if vertical acceleration is less than 0
        // Velocity unit is m/s and accel m/s^2
        // (vertical_velocity <= 100) && (vertical_acceleration <= 0.0)
        if ((net_acceleration <= BURNT_OUT_NET_ACCELERATION_THRESHOLD)
            && (current_altitude >= BURNT_OUT_ALTITUDE_THRESHOLD)){
            if (elapsed_time + BURNT_OUT_GUARD_TIMER < current_time) {
                return true;
            }
            return false;
        }
        this->elapsed_time = current_time;
        return false;
    }

    /**
     * @brief Function that checks wether rocket has reached its apogee, 
     * uses altitude and time of max altitude record time, to check how
     * long it was fixed.
     * 
     * @param current_altitude current altitude value
     * @param current_time current board time or any elapsing time unit
     * @return true if max altitude is verified for time period
     * @return false if not apogee
     */
    bool if_reached_apogee(float current_altitude, float current_time) {
        // std::cout << "Max altitude " << max_altitude << std::endl;
        // std::cout << "Current altitude " << current_altitude << std::endl;
        // std::cout << "Current time " << current_time << std::endl;
        // std::cout << "Elapsed time " << elapsed_time << std::endl;
        // std::cout << std::endl << std::endl;

        if (max_altitude < current_altitude) {
            max_altitude = current_altitude;
            elapsed_time = current_time;
            // std::cout << "Max altitude is less than current_altitude" << std::endl;
            // std::cout << max_altitude << std::endl;
            return false;
        }
        
        float dh = max_altitude - current_altitude;
        float dt = current_time - elapsed_time;
        // Apogee is same as max altitude
        if (dh >= APOGEE_ALTITUDE_DIFFERENCE_MINIMUM && dt >= APOGEE_GUARD_TIMER) {
            return true;
        }
        
        return false;
    }

    /**
     * @brief Function that checks wether rocket has landed.
     * It changes land time and altitude if rocket still flying to current data
     * 
     * @param current_altitude current altitude value
     * @param net_linear_acceleration (x^2+y^2+z^2)^(1/2) of acceleration value
     * @param current_time current board time or any elapsing time unit
     * @return true if land detected
     * @return false if land not detected
     */
    bool if_landed(float current_altitude, float net_linear_acceleration,
        float current_time)  {
        float dh = current_altitude - land_altitude;
        float dt = current_time - elapsed_time;
        if (fabs(dh) <= LAND_MAX_ALTITUDE_DIFFERENCE
            && net_linear_acceleration <= LAND_MAX_ACCELERATION_VIBRATION) {
            if(dt >= LAND_GUARD_TIMER) {
                return true;
            }
            return false;
        } else {
            land_altitude = current_altitude;
            elapsed_time = current_time;
        }
        return false;
    }

    void process_next_state(float current_altitude, float net_acceleration, float current_time) {
        previous_state = current_state;
        if (current_state == PRE_LAUNCH) {
            if (this->if_launched_by_altitude(current_altitude, net_acceleration, current_time)) {
                current_state = LAUNCHED;
            }
        }
        else if (current_state == LAUNCHED) {
            if (this->if_burnt_out(current_altitude, net_acceleration, current_time)) {
                current_state = BURNT_OUT;
            }
        }
        else if (current_state == BURNT_OUT) {
            if (this->if_reached_apogee(current_altitude, current_time)) {
                current_state = LANDING;
            }
        }
        else if (current_state == LANDING) {
            if (this->if_landed(current_altitude, net_acceleration, current_time)) {
                current_state = LANDED;
            }
        }
    }

    bool if_state_changed() {
        if (previous_state != current_state) {
            return true;
        }

        return false;
    }
};
#endif