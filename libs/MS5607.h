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
}

MS5607::~MS5607()
{
}
