/*
 * Class "LEDEffects" body
 * File name: LEDEffects.cpp
 *
 * Created on 14 July 2016
 * by Alain Le Marchand
 *
 * (C) Copyright 2016, Alain Le Marchand
 *
 * History
 * v1 (14/07/2016) - Creation
 */

#include "Arduino.h"
#include "LEDEffects.h"

	// Constructor
	LEDEffects::LEDEffects(){
		/* do nothing */
	} // constructor

	/*****************************************************************************************
	* function name: attach()
	* arguments: pin number
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
		endTime_ = LEDEFF_NOTIMER ;
		altBlinkTime_ = LEDEFF_NOTIMER ;
	} // function: attach

	uint8_t LEDEffects::readPin(){
	 return pin_;
	}	 // function: readPin

	/*****************************************************************************************
	* function name: update()
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
	} // function: update

	/*****************************************************************************************
	* function name: handleBlinking()
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
		else // fixed lighting - allumage fixe
		{
			status_ = setpoint_ & LEDEFF_ON ; // status: LED on or off depending on setpoint first bit.
		}
	}  // function: handleBlinking

	// Turn off LED
	void LEDEffects::off(){
			setpoint_ &= ~LEDEFF_ON ; // setpoint: LED off - consigne: extinction de la LED
	} // function: off

	// Set LED on - Light up!
	void LEDEffects::on(){
			setpoint_ |= LEDEFF_ON ; // setpoint: LED on - consigne: allumage de la LED
	} // function: on

	// Set the LED setpoint to be permanent (no time out) - overrides setTimer
	void LEDEffects::setPermanent(){
			setpoint_ &= ~LEDEFF_TIMED ; // setpoint: no time out - consigne: pas de temporisation
	} // function: setPermanent

	// Set timer, expressed in ms - overrides setPermanent
	void LEDEffects::setTimer(unsigned long timer){
			endTime_ = millis() + timer;  // set the end time (time out)
			setpoint_ |= LEDEFF_TIMED ; // setpoint: timer on - consigne: temporisation
	} // function: setTimer

	// Set fixed lighting - overrides setBlink and other effects.
	void LEDEffects::setFixed(){
			setpoint_ &= ~LEDEFF_BLINK ; // setpoint: no blink - consigne: pas de clignotement
	} // function: setFixed

	// Set blink period, expressed in ms - LED in ON during "BlinkTime", then OFF during "BlinkTime", etc.
	// will start blinking using the current setpoint (starting cycle OFF if setpoint is OFF and vice versa)
	void LEDEffects::setBlink(uint16_t blinkTime){
			blinkTime_	= blinkTime;
			altBlinkTime_ = millis() ;  // use the current (absolute) time in order to force blink alternate at next cycle
			setpoint_ |= LEDEFF_BLINK ; // setpoint: blinking on - consigne: clignotement
			status_ = !(setpoint_ & LEDEFF_ON); // set status to inverse of setpoint, as it will be toggled immediately at next cycle
	} // function: setBlink

	// memorize setpoint ON
	void LEDEffects::memOn(){
			setpointMemo_	= LEDEFF_ON;
	} // function: memOn

	// memorize setpoint OFF
	void LEDEffects::memOff(){
			setpointMemo_	= LEDEFF_OFF;
	} // function: memOff

	// restore memorized setpoint
	void LEDEffects::restoreSetpoint(){
			setpoint_ = setpointMemo_;
	} // function: restoreStatus

