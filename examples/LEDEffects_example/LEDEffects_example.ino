/*
 * Example of use of library "LEDEffects"
 * File name: LEDEffects_example.ino
 *
 * Created on 17 July 2016
 * by Alain Le Marchand
 *
 * (C) Copyright 2016, Alain Le Marchand
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * History
 * v1.0 (14/07/2016) - Creation
 * v1.1 (17/07/2016) - Add flash method
 */
#include <LEDEffects.h>

#define LED1_PIN  13
#define LED2_PIN  11
#define LED3_PIN  10
#define LED4_PIN  9
#define LED5_PIN  8

  // create the LEDEffects objects.
  LEDEffects myLED1;
  LEDEffects myLED2;
  LEDEffects myLED3;
  LEDEffects myLED4;
  LEDEffects myLED5;

void setup() {
  // initialise and attach pin to each LED
  myLED1.attach(LED1_PIN);
  myLED2.attach(LED2_PIN);
  myLED3.attach(LED3_PIN);
  myLED4.attach(LED4_PIN);
  myLED5.attach(LED5_PIN);

  // Set LED 1 on permanent ON
  myLED1.on();
  myLED1.setPermanent();
  myLED1.setFixed();
  // memorize LED 1 setpoint
  myLED1.memOn();

  // Set LED 2 on permanent blinking (1s cycle)
  myLED2.on();
  myLED2.setPermanent();
  myLED2.setBlink(1000);
  // memorize LED 2 setpoint
  myLED2.memOn();

  // Set LED 3 on fast blinking during 10s
  myLED3.on();
  myLED3.setTimer(10000);
  myLED3.setBlink(250);
  // memorize LED 3 setpoint
  myLED3.memOff();

  // Set LED 4 on fast blinking during 5s, inverted phase to LED 3
  myLED4.off();
  myLED4.setTimer(5000);
  myLED4.setBlink(250);
  // memorize LED 4 setpoint
  myLED4.memOn();

  // Set LED 5 on flash during 35 s
  myLED5.off();
  myLED5.setTimer(35000);
  myLED5.setFlash(100, 1000);
}

void loop() {
  // update LED objects at each cycle of the main loop:
  myLED1.update();
  myLED2.update();
  myLED3.update();
  myLED4.update();
  myLED5.update();

  // Do what you have to do here. The LEDs will be handled without blocking your program.

  // After some time, change the LED behavior.
  if (millis() == 15000) {
      // Stop LED 1
      myLED1.off();
      // Turn on LED 2 during 10s
      myLED2.on();
      myLED2.setTimer(10000);
      myLED2.setFixed();
      // Set LED 3 on slow blinking permanently
      myLED3.on();
      myLED3.setPermanent();
      myLED3.setBlink(1000);
      // Set LED 4 on blinking during 5s (half of LED 3, start inverted)
      myLED4.off();
      myLED4.setTimer(5000);
      myLED4.setBlink(500);
    }

    // After another time, restore LEDs to their momorized setpoint.
    if (millis() == 30000) {
      myLED1.restoreSetpoint();
      myLED2.restoreSetpoint();
      myLED3.restoreSetpoint();
      myLED4.restoreSetpoint();
    }
}
