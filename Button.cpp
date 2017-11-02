// Button.cpp - Button class implementation takes place here
// by Matthew Hirsch and Dylan Herrel
// CSC 113 - Assignment 2
// 10/4/17

#include "Button.h"
#include <Arduino.h>

unsigned long lastChangedTimeMilli = 0; // Last recorded time a button was pressed (in ms)
// Minimum time required to pass between two registered button presses (in ms)
const unsigned long MinChangeTimeMilli = 10;

// Default Constructor: initializes ledPin to 10 and inPin to 4
Button::Button(){
	ledPin = 10;
	inPin = 4;
}

// Specific Constructor: initializes pins used for LED output and button input
Button::Button(const int newLedPin, const int newInPin){
	ledPin = newLedPin;
	inPin = newInPin;
}

// Calls pinMode() function for a button/LED pair
void Button::setPinMode(){
	pinMode(ledPin, OUTPUT);      // sets the digital pin as output for the LED
	pinMode(inPin, INPUT_PULLUP); // sets the digital pin as input for the button
}

// Debounces button press of specific button
int Button::debounce(int currVal, int oldVal){
	if (currVal != oldVal){ // If button 'press' value is different (ex. It was not pressed and now it is.)
		unsigned long nowMilli = millis(); // Get the current time (in ms)
		// If time between most recent press and previous press is greater than 10 ms,
		// register the new button press
		if(nowMilli - lastChangedTimeMilli > MinChangeTimeMilli) {
			Serial.println(currVal); // Print out current button press value
			oldVal = currVal; // Assign new value to the "old value" so holding down a button only counts as one press.
			lastChangedTimeMilli = nowMilli; // Assign new time value to adjust for next press
		}
	}
	return oldVal;
}
