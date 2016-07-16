LEDEffects
==========

An Arduino library to handle multiple LEDs in parallel without blocking the Arduino.

Turn on, off, blink, either permanently or during a given time.

Works with the millis() timebase, expressed in milliseconds.
As this is an "unsigned long" (32 bits) variable type, this allows to get an absolute timebase lasting about 50 days, which is sufficient for non permanent applications.

See page https://github.com/AlanUS/Arduino-LEDEffects/wiki for details.
