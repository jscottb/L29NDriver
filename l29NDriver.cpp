/*
  L29N Motor driver Library
  based on the Adafruit Motor shield library
  this code is public domain, so enjoy!
*/

#if (ARDUINO >= 100)
   #include "Arduino.h"
#else
   #include <avr/io.h>
   #include "WProgram.h"
#endif

#include "L29NDriver.h" 

L29N_Motor::L29N_Motor(uint8_t in_1_pin, uint8_t in_2_pin, uint8_t enable_pin) {
   motor_in_1 = in_1_pin;
   motor_in_2 = in_2_pin;
   motor_enable = enable_pin;
   pinMode(motor_in_1, OUTPUT); 
   pinMode(motor_in_2, OUTPUT); 
   if (motor_enable) {
      pinMode(motor_enable, OUTPUT); 
   }
}

bool L29N_Motor::run(uint8_t cmds) {
   if (((FORWARD | BACKWARD) & cmds) == (FORWARD | BACKWARD)) {
      return false;
   }

   if (((BRAKE | RELEASE) & cmds) == (BRAKE | RELEASE)) {
      return false;
   }

   if ((cmds & FORWARD) == FORWARD) {
      if (motor_enable) {
         analogWrite(motor_enable, currentspeed);
      }

      digitalWrite(motor_in_1, LOW);
      digitalWrite(motor_in_2, HIGH);
      motorflags = (motorflags & (BRAKE | RELEASE)) | FORWARD;
   }

   if ((cmds & BACKWARD) == BACKWARD) {
      if (motor_enable) {
         analogWrite(motor_enable, currentspeed);
      }

      digitalWrite(motor_in_1, HIGH);
      digitalWrite(motor_in_2, LOW);
      //motorflags = (motorflags & (BRAKE | RELEASE)) | BACKWARD;
      motorflags = motorflags | BACKWARD;
   }

   if ((cmds & RELEASE) == RELEASE) {
      if (motor_enable) {
         if ((motorflags & NOPWM) == NOPWM)
            digitalWrite(motor_enable, (currentspeed > 0) ? HIGH : LOW);
         else 
            analogWrite(motor_enable, currentspeed);
      }
     
      if ((motorflags & FORWARD) == FORWARD) {
         digitalWrite(motor_in_1, LOW);
         digitalWrite(motor_in_2, HIGH);
      }

      if ((motorflags & BACKWARD) == BACKWARD) {
         digitalWrite(motor_in_1, HIGH);
         digitalWrite(motor_in_2, LOW);
      } 

      //motorflags = (motorflags & (FORWARD | BACKWARD)) | RELEASE;
      motorflags = motorflags | RELEASE;
   }

   if ((cmds & BRAKE) == BRAKE) {
      if (motor_enable) {
         digitalWrite(motor_enable, HIGH);
      }

      digitalWrite(motor_in_1, HIGH);
      digitalWrite(motor_in_2, HIGH);
      //motorflags = (motorflags & (FORWARD | BACKWARD)) | BRAKE;
      motorflags = motorflags | BRAKE;
   }

   return true;
}

void L29N_Motor::setSpeed(uint8_t speed) {
   if (speed > 255) speed = 255;
   else if (speed < 0) speed = 0;
   currentspeed = speed;
   if (motor_enable) {
      if ((motorflags & NOPWM) == NOPWM)
         digitalWrite(motor_enable, (currentspeed > 0) ? HIGH : LOW);
      else 
         analogWrite(motor_enable, currentspeed);
   }

   if ((motorflags & FORWARD) == FORWARD) {
      digitalWrite(motor_in_1, LOW);
      digitalWrite(motor_in_2, HIGH);
   }

   if ((motorflags & BACKWARD) == BACKWARD) {
      digitalWrite(motor_in_1, HIGH);
      digitalWrite(motor_in_2, LOW);
   } 
}

uint8_t L29N_Motor::getSpeed(void) {
   return (uint8_t) currentspeed;
}

uint8_t L29N_Motor::getState(void) {
   return motorflags & (BRAKE | RELEASE);
}

uint8_t L29N_Motor::getDirection(void) {
   return motorflags & (FORWARD | BACKWARD);
}
