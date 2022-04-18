#ifndef _SHARED_DATA_H_
#define _SHARED_DATA_H_

class Configuration
{
public:
    bool run_servo_sweep = false;
    bool send_real_time_data = false;
    bool arm = false;
    bool shutdown = false;
};

#endif