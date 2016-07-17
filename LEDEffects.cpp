/*
 * Class "LEDEffects" body
 * File name: LEDEffects.cpp
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
 * v1.0 (14/07/2016) - Creation
 * v1.1 (17/07/2016) - Add flash method
 */

#include "Arduino.h"
#include "LEDEffects.h"

	// Constructor
	LEDEffects::LEDEffects(){
		/* do nothing */
	} // constructor

	/*****************************************************************************************
	* method name: attach()
	* arguments: pin number (byte/uint8_t)
	* returns: nothing (void)
	*
	* Description:
	* initialize the Pin to which the LED is attached, as well as all internal variables.
	* Initialise le pin auquel la LED est assigné, ainsi que les variables internes.
	*****************************************************************************************/
	void LEDEffects::attach(uint8_t pin){
		pin_ = pin;
    pinMode(pin_, OUTPUT); // declaration as Output
		status_ = LOW ;	// initialize LED to off
    digitalWrite(pin_, status_); // write value to Arduino
		setpoint_ = LEDEFF_OFF ;
		setpointMemo_ = setpoint_ ;
		blinkTime_ = LEDEFF_NOBLINK ;
		flashTime_ = LEDEFF_NOBLINK ;
		endTime_ = LEDEFF_NOTIMER ;
		altBlinkTime_ = LEDEFF_NOTIMER ;
	} // method: attach

	/*****************************************************************************************
	* method name: readPin()
	* arguments: none
	* returns: pin number (byte/uint8_t)
	*
	* Description:
	* returns pin number to which LED is attached.
	* retourne le numéro de pin auquel la LED est assignée.
	*****************************************************************************************/
	uint8_t LEDEffects::readPin(){
	 return pin_;
	}	 // method: readPin

	/*****************************************************************************************
	* method name: update()
	* arguments: none
	* returns: nothing (void)
	*
	* IMPORTANT: To be called at each cycle in the main loop of the Arduino program
	*
	* Description:
	* Depending on the setpoint of the LED and of its timers (on time, blink time) values,
	* sets the LED to LOW (off) or HIGH (on) state.
	*
	* Works with the millis() timebase, expressed in milliseconds.
	* millis() is initialized at Arduino startup.
	* As this is an "unsigned long" (32 bits) variable type, this allows to get an
	* absolute timebase lasting about 50 days, which is sufficient for non permanent
	* applications.
	*
	* En fonction de la consigne assignée à chaque LED et des valeurs des timers
	* associés (durée d'allumage, fréquence de clignotement),
	* positionne chaque LED à l'état bas (LOW / éteint) ou haut (HIGH / allumé).
	*
	* Fonctionne sur la base de temps millis() exprimée en millisecondes.
	* millis() s'initialise à zéro au démarrage de l'Arduino.
	* S'agissant d'une variable de type "unsigned long" (32 bits), permet de disposer d'une
	* base de temps absolu d'environ 50 jours, ce qui est suffisant pour les applications
	* non permanentes.

	* see: https://www.arduino.cc/en/Reference/Millis
	*****************************************************************************************/
	void LEDEffects::update(){

		if (setpoint_ & LEDEFF_TIMED)  // Is lighting timed? - Est-ce que l'allumage est temporisé ?
		// Timed lighting - Allumage temporisé
		{
		if (millis() < endTime_)  // Are we over the lighting timer? Est-ce que l'on a dépassé la tempo de fin d'allumage ?
			// we are still in the lighting period. On est toujours dans la période d'allumage
			{
				handleBlinking(); // set status depending on blinking
			} // end handling timed on period - fin du traitement dans la période d'allumage temporisé
			else // time out - tempo de fin atteinte
			{
				setpoint_ = LEDEFF_OFF;  // setpoint reset to off, no blink, permanent - consigne de la LED réinitialisée à éteint, fixe, permanent
				status_ = setpoint_ & LEDEFF_ON ; // status: turn off light - état: extinction de la LED
			} // end test timed lighting - fin du test de temporisation d'allumage
		}
		else // Permanent lighting - Allumage permanent
		{
			handleBlinking(); // set status depending on blinking
		} // end test timed or not - fin du test d'allumage temporisé ou pas.

		// To be executed at all times
		digitalWrite(pin_, status_); // send the LED status to Arduino
	} // method: update

	/*****************************************************************************************
	* method name: handleBlinking()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* handles blinking of the LED, if required. Otherwise turn on LED.
	* gère le clignotement de la LED, si besoin. Sinon allume la LED.
	*****************************************************************************************/
	void LEDEffects::handleBlinking(){
		if (setpoint_ & LEDEFF_BLINK)  // Are we in blinking mode? - Est-ce que l'on est en mode clignotant ?
		// Blinking mode - Allumage clignotant
		{
			if (altBlinkTime_ < millis())  // Are we over the toggle blinking period time? - Est-ce que l'on a dépassé la tempo d'alternance de clignotement ?
				// Yes, need to the change blinking status - Oui, il faut changer le clignotement
			{
				status_ = !status_; // toggle LED status - inversion de l'état de la LED
				altBlinkTime_ = (unsigned long) (millis() +  blinkTime_); // next value for blink time - prochaine valeur pour la temporisation
			 } // end of test for toggle blinking  - fin de test pour l'alternance de clignotement
		}
		else // not blinking mode
		{
			if (setpoint_ & LEDEFF_FLASH)  // Are we in flashing mode? - Est-ce que l'on est en mode flash ?
			// flash mode
			{
				if (altBlinkTime_ < millis())  // Are we over the toggle flashing period time? - Est-ce que l'on a dépassé la tempo d'alternance de flash ?
					// Yes, need to the change flash status - Oui, il faut changer le flash
				{
					if (status_ == LEDEFF_OFF){ // alternate to flash on
						altBlinkTime_ = (unsigned long) (millis() +  flashTime_); // next value for flash time - prochaine valeur pour la temporisation	 de flash
					}
					else { // turn off flash time during the remaining of cycle
						altBlinkTime_ = (unsigned long) (millis() +  (blinkTime_-flashTime_)); // next value for flash time - prochaine valeur pour la temporisation de flash
					}
					status_ = !status_; // toggle LED status - inversion de l'état de la LED
				 } // end of test for toggle flash  - fin de test pour l'alternance de flash
			}
			else  // fixed lighting - allumage fixe
			{
				status_ = setpoint_ & LEDEFF_ON ; // status: LED on or off depending on setpoint first bit.
			} // test flashing
		} // test blinking
	}  // method: handleBlinking

	/*****************************************************************************************
	* method name: off()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* turn off LED.
	* éteint la LED.
	*****************************************************************************************/
	void LEDEffects::off(){
			setpoint_ &= ~LEDEFF_ON ; // setpoint: LED off - consigne: extinction de la LED
	} // method: off

	/*****************************************************************************************
	* method name: on()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* turn on LED. Light up!
	* allume la LED.
	*****************************************************************************************/
	void LEDEffects::on(){
			setpoint_ |= LEDEFF_ON ; // setpoint: LED on - consigne: allumage de la LED
	} // method: on

	/*****************************************************************************************
	* method name: setPermanent()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* Set the LED setpoint to be permanent (no time out) - overrides setTimer.
	* Consigne allumage permanent (pas de temporisation) - invalide setTimer.
	*****************************************************************************************/
	void LEDEffects::setPermanent(){
			setpoint_ &= ~LEDEFF_TIMED ; // setpoint: no time out - consigne: pas de temporisation
	} // method: setPermanent

	/*****************************************************************************************
	* method name: setTimer()
	* arguments: timer value in ms (unsigned long)
	* returns: nothing (void)
	*
	* Description:
	* Set timer, expressed in ms - overrides setPermanent.
	* Initialise la temporisation (en ms) - invalide setPermanent.
	*****************************************************************************************/
	void LEDEffects::setTimer(unsigned long timer){
			endTime_ = millis() + timer;  // set the end time (time out)
			setpoint_ |= LEDEFF_TIMED ; // setpoint: timer on - consigne: temporisation
	} // method: setTimer

	/*****************************************************************************************
	* method name: setFixed()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* Set fixed lighting - overrides setBlink and other effects.
	* Consigne allumage fixe - invalide setBlink et autres effets.
	*****************************************************************************************/
	void LEDEffects::setFixed(){
			setpoint_ &= ~LEDEFF_BLINK ; // setpoint: no blink - consigne: pas de clignotement
	} // method: setFixed

	/*****************************************************************************************
	* method name: setBlink()
	* arguments: blinking period in ms (unsigned int/uint16_t)
	* returns: nothing (void)
	*
	* Description:
	* Set blink period, expressed in ms
	* LED in ON during "BlinkTime", then OFF during "BlinkTime", etc. - overrides setFixed
	* will start blinking using the current setpoint (starting cycle OFF if setpoint is OFF and vice versa)
	* Initialise la période de clignotement (en ms)
	* la LED est allumée pendant "BlinkTime", puis éteinte pendant "BlinkTime", etc.  - invalide setFixed.
	* commencera à clignoter en utilisant la consigne courante (cycle commençant éteint si la consigne est à OFF et vice versa)
	*****************************************************************************************/
	void LEDEffects::setBlink(uint16_t blinkTime){
			blinkTime_	= blinkTime;
			altBlinkTime_ = millis() ;  // use the current (absolute) time in order to force blink alternate at next cycle
			setpoint_ |= LEDEFF_BLINK ; // setpoint: blinking on - consigne: clignotement
			setpoint_ &= ~LEDEFF_FLASH ; // setpoint: no flash - consigne: pas de flash
			status_ = !(setpoint_ & LEDEFF_ON); // set status to inverse of setpoint, as it will be toggled immediately at next cycle
	} // method: setBlink

	/*****************************************************************************************
	* method name: setFlash()
	* arguments: flash time and cycle period in ms (unsigned int/uint16_t)
	* returns: nothing (void)
	*
	* Description:
	* Set flash time and cycle period, expressed in ms
	* LED in ON during "flashTime", then OFF until "cycleTime" is over, etc. - overrides setFixed
	* cycle will start by a flash
	* Initialise la durée du flash et le cylce (en ms)
	* la LED est allumée pendant "flashTime", puis éteinte jusqu'à la fin du cyle "cycleTime", etc.  - invalide setFixed.
	* le cycle commencera par un flash
	*****************************************************************************************/
	void LEDEffects::setFlash(uint16_t flashTime, uint16_t cycleTime) {
			flashTime_	= flashTime;
			if (cycleTime > flashTime) { // cycleTime has to be higher than flashTime
				blinkTime_	= cycleTime;
			}
			else { // otherwise cycle time with be equal to flash time, i.e. no flash
				blinkTime_ = flashTime;
			}
			altBlinkTime_ = millis() ;  // use the current (absolute) time in order to force flash alternate at next cycle
			setpoint_ &= ~LEDEFF_BLINK ; // setpoint: no blink - consigne: pas de clignotement
			setpoint_ |= LEDEFF_FLASH ; // setpoint: flash on - consigne: flash
			status_ = LEDEFF_OFF ; // set status to Off, as it will be toggled immediately at next cycle
	}	// method: setFlash

	/*****************************************************************************************
	* method name: memSetpoint()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* memorize current setpoint.
	* mémorise la consigne courante.
	*****************************************************************************************/
	void LEDEffects::memSetpoint(){
			setpointMemo_	= setpoint_;
	} // method: memSetpoint

	/*****************************************************************************************
	* method name: memOn()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* memorize setpoint ON, fixed, permanent.
	* mémorise l'état allumé, fixe et permanent.
	*****************************************************************************************/
	void LEDEffects::memOn(){
			setpointMemo_	= LEDEFF_ON;
	} // method: memOn

	/*****************************************************************************************
	* method name: memOff()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* memorize setpoint OFF, fixed, permanent.
	* mémorise l'état éteint, fixe et permanent.
	*****************************************************************************************/
	void LEDEffects::memOff(){
			setpointMemo_	= LEDEFF_OFF;
	} // method: memOff

	/*****************************************************************************************
	* method name: restoreSetpoint()
	* arguments: none
	* returns: nothing (void)
	*
	* Description:
	* restore memorized setpoint.
	* rétablit l'état mémorisé.
	*****************************************************************************************/
	// restore memorized setpoint
	void LEDEffects::restoreSetpoint(){
			setpoint_ = setpointMemo_;
	} // method: restoreStatus

