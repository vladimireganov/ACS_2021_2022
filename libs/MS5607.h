// class for MS5607 sensor
#define MS5607testing // for testing
// comment MS5607testing for prod
class MS5607
{
private:
    /* data */
    float pressure;
    float altitude;
public:
    MS5607(/* args */);
    ~MS5607();
    float get_pressure();
    float get_altitude();
};

MS5607::MS5607(/* args */)
{
    // add starting of the connection
}

MS5607::~MS5607()
{
    // add proper closing of the connection
}
float MS5607::get_pressure(){
    #ifdef MS5607testing
    return 1.0; //
    #endif
    
}