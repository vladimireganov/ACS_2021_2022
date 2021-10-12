#include "libs/MS5607.h"
#include "libs/Data.h"
MS5607 altimetr;
Data data;

int main(){
    data.pressure = altimetr.get_pressure();

    save_data(&data);

    return 0;
}