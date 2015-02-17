/*
  L293 and L298 MotorDriver Library
  based on the Adafruit Motor shield library
  this code is also public domain, enjoy!
*/

#ifndef _L29NDriver_h_
#define _L29NDriver_h_

#include <inttypes.h>
//#include <avr/io.h>

#define FORWARD 1
#define BACKWARD 2
#define BRAKE 4
#define RELEASE 8
#define NOPWM 128

class L29N_Motor {
public:
   L29N_Motor(uint8_t in_1_pin, uint8_t in_2_pin, uint8_t enable_pin);
   bool run(uint8_t);
   void setSpeed(uint8_t);
   uint8_t getSpeed(void);
   uint8_t getState(void);
   uint8_t getDirection(void);
      	
private:
   uint8_t motor_in_1, motor_in_2, motor_enable, motorflags, currentspeed;
};

#endif
