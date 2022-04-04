#ifndef ALTITUDE_CONTROL_SYSTEM
#define ALTITUDE_CONTROL_SYSTEM

class AltitudeControlSystem {
    private:
    float target_altitude = 914.4f; // in m

    public:
    AltitudeControlSystem(/* args */);
    void set_target_altitude(float altitude);
    float get_target_altitude();
    int estimate_angle(float projected_altitude);
};

AltitudeControlSystem::AltitudeControlSystem() {
}

void AltitudeControlSystem::set_target_altitude(float altitude) {
    target_altitude = altitude;
}

float AltitudeControlSystem::get_target_altitude() {
    return target_altitude;
}

int AltitudeControlSystem::estimate_angle(float projected_altitude) {
    float delta_alt = projected_altitude - target_altitude;

    if (delta_alt > 15.0) {
        return 40;
    }
    else if (delta_alt > 10.0 && delta_alt <= 15.0) {
        return 30;
    }
    else if (delta_alt < 5.0 && delta_alt <= 10.0) {
        return 20;
    }
    else if (delta_alt > 3.0 && delta_alt <= 5.0) {
        return 10;
    }
    else {
        return 0;
    }
}

AltitudeControlSystem acs = AltitudeControlSystem();
#endif