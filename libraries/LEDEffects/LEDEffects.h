/*
 * Class "LEDEffects" declaration
 * File name: LEDEffects.h
 *
 * Created on 14 July 2016
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
 * v1 (14/07/2016) - Creation
 */

#ifndef LEDEFFECTS_H
#define LEDEFFECTS_H

#define LEDEFF_OFF     	0 // bit 0 - equivalent to LOW
#define LEDEFF_ON  			1 // bit 0 - equivalent to HIGH
#define LEDEFF_TIMED  	2 // bit 1
#define LEDEFF_BLINK  	4 // bit 2

#define LEDEFF_NOBLINK  0 // set value to 0 for no blinking
#define LEDEFF_NOTIMER  0 // set value to 0 for permanent setpoint (not timed out)

class LEDEffects
{
public:
	// Constructor
	LEDEffects();

	// initialize the Pin to which the LED is attached.
	void attach(uint8_t pin);

	// returns the number of the Pin to which the LED is attached.
	uint8_t readPin();

	// Update LED status based on millis() timebase and on settings.
	// To be called at each cycle in the main loop of the Arduino program.
	void update();

	// Turn off LED
	void off();

	// Set LED on - Light up!
	void on();

	// Set the LED setpoint to be permanent (no time out) - overrides setTimer
	void setPermanent();

	// Set timer, expressed in ms - overrides setPermanent
	void setTimer(unsigned long timer);

	// Set fixed lighting - overrides setBlink and other effects.
	void setFixed();

	// Set blink period, expressed in ms - LED in ON during "BlinkTime", then OFF during "BlinkTime", etc.
	// will start blinking using the current setpoint (starting cycle OFF if setpoint is OFF and vice versa)
	void setBlink(uint16_t blinkTime);

	// memorize setpoint ON
	void memOn();

	// memorize setpoint OFF
	void memOff();

	// restore memorized setpoint
	void restoreSetpoint();

private:
	// Arduino pin assigned to the LED
	/* pin Arduino assigné à la LED */
	uint8_t pin_;

	// Current status of the LED (LOW = false = off, HIGH = = true = on)
	/* état courant de la LED (LOW = éteint, HIGH = allumé) */
	bool status_;

	// LED lighting setpoint
	/* 	Bit 0 = On (1) = LEDEFF_ON / Off (0) = LEDEFF_OFF
	 * 	Bit 1 = Timed (1) = LEDEFF_TIMED / Permanent (0)
	 * 	Bit 2 = Blinking (1) = LEDEFF_BLINK / Fixed (0)
	 */
	/* consigne d'allumage:	 */
	/*	Bit 0 = Allumé (1) = LEDEFF_ON / Eteint (0) = LEDEFF_OFF
	 *	Bit 1 = Temporisé (1) = LEDEFF_TIMED / Permanent (0)
	 *	Bit 2 = Clignotant (1) = LEDEFF_BLINK / Fixe (0)
	 */
	uint8_t setpoint_;

	// Blinking timer value, in ms (On time = Off time) - ignored is setpoint if permanent On.
	/* valeur de la temporisation pour le clignotement, en ms - ignoré si la consigné est allumage fixe. */
	// default = LEDEFF_NOBLINK
  uint16_t blinkTime_; // unsigned int

	// Memorized status of the LED, to be used when some function lead to temporary extinction of all LEDs
	/* Mémorisation du statut courant de la LED, pour utilisation lorsque certaines fonctions conduisent à éteindre temporairement toutes les LEDs */
	bool setpointMemo_;

	// LED On end ansolute time, in ms (uses time reference millis()) - ignored if setpoint is permanent On
	/* temps absolu de fin d'allumage, en ms (utilise la référence de temps millis()) - ignoré si la consigne est allumage permanent. */
	unsigned long endTime_;

	// Blinking alternate absolute time, in ms (uses time reference millis()) - ignored if setpoint is permanent On
	/* temps absolu pour alternance clignotement, en ms (utilise la référence de temps millis()) - ignoré si la consigne est allumage fixe. */
	unsigned long altBlinkTime_;

	// handles blinking of the LED, if required
	void handleBlinking();

}; // class LEDEffects

#endif // LEDEFFECTS_H