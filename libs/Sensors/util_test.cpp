//
// Created by jtc19 on 2/3/2022.
//

#include "pi_util.h

int main() {

   if (init_GPIO()) {

       shift_bmx_Addr();

   }

   beginSensors();

   return 0;



}