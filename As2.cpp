// As2.cpp - Implementation of the game 'Simon'
// by Matthew Hirsch and Dylan Herrel
// CSC 113 - Assignment 2
// 10/4/17

#include "Button.h"
#include <Arduino.h>

// Define the output pins for each of the four LEDs
const int ledBlue = 10;    // Blue LED connected to digital pin 10
const int ledGreen = 12;   // Green LED connected to digital pin 12
const int ledYellow = 11;  // Yellow LED connected to digital pin 11
const int ledRed = 13;     // Red LED connected to digital pin 13
// Define the input pins for each of the four push buttons
const int inPinBlue = 4;   // Blue pushbutton connected to digital pin 4
const int inPinGreen = 5;  // Green pushbutton connected to digital pin 5
const int inPinYellow = 6; // Yellow pushbutton connected to digital pin 6
const int inPinRed = 7;    // Red pushbutton connected to digital pin 7

// Create four Button objects, each with an LED output pin and a button input pin
Button btnBlue(ledBlue, inPinBlue);
Button btnGreen(ledGreen, inPinGreen);
Button btnYellow(ledYellow, inPinYellow);
Button btnRed(ledRed, inPinRed);

int val = 0; // Value of current button being pressed: 1 = not pressed, 2 = Blue, 3 = Green, 4 = Yellow, 5 = Red
// Old values of each button press, so each button can debounce independently of each other
int oldValBlue = -1;
int oldValGreen = -1;
int oldValYellow = -1;
int oldValRed = -1;

int numButtonPress = 0; // Counts the number of button presses per level
int randColor = 0; // Used to randomize which color LED will light up
int level = 0; // The level number the user has reached
int sequence[10]; // The array of integers used to hold the sequence of LED blinks
bool boolCorrect = true; // Used to test if user pushes correct buttons
bool arrayEnd = false; // Used to test if end of blink array has been reached

// Make a sequence of 10 random integers between 2 and 5.
// That sequence is used to blink the LEDs at the start of each level.
int makeSequence(int seq[10], int currLevel) {
	if (currLevel < 10) { // If it's level 1 through 9
		for(int i = 0; i < currLevel; i++){
			seq[i] = (int) random(2,6); // Add a random number between 2 and 5 to each element of the array.
			Serial.println(seq[i]); // Print out the contents of the array to check the LED blinks
		}
	}else{ // Once level 10 is reached, don't add more blinks to the sequence.
		for(int j = 0; j < 10; j++){
			seq[j] = (int) random(2,6); // Add a random number between 2 and 5 to each element of the array.
		}
	}
	return 1;
}

// Checks correct value in sequence against user's guess
// Returns 1 if correct. Else, returns false.
int checkValue(int correctVal, int userChoice){
	// If value in sequences matches the user's guess, they are correct
	if(correctVal == userChoice){
		return 1;
	}
	else{ // The user is not correct
		return 0;
	}
}

// Called to execute the blink sequence at the start of each level
void blinkLEDS(int *seq, int currLevel){
	int levelDelay = 1000; // Delay between each LED blink, This values scales down as level increases past 10.
	int blinkDelay = 300; // Length of each individual LED blink (time between HIGH and LOW signal).
	if (currLevel < 10) { // If it's level 1 through 9
		for (int i = 0; i < currLevel; i++){
			// Scan through the blink array and blink the corresponding LED
			// Value of 2 = Blue, 3 = Green, 4 = Yellow, and 5 = Red
			if (seq[i] == 2) {digitalWrite(ledBlue, 1); delay(blinkDelay); digitalWrite(ledBlue, 0);}
			else if (seq[i] == 3) {digitalWrite(ledGreen, 1); delay(blinkDelay); digitalWrite(ledGreen, 0);}
			else if (seq[i] == 4) {digitalWrite(ledYellow, 1); delay(blinkDelay); digitalWrite(ledYellow, 0);}
			else if (seq[i] == 5) {digitalWrite(ledRed, 1); delay(blinkDelay); digitalWrite(ledRed, 0);}
			delay(levelDelay);
		}
	}
	else{ // If level 10 or higher is reached
		for (int i = 0; i < currLevel; i++){
			// Scan through the blink array and blink the corresponding LED
			// Value of 2 = Blue, 3 = Green, 4 = Yellow, and 5 = Red
			if (seq[i] == 2) {digitalWrite(ledBlue, 1); delay(blinkDelay); digitalWrite(ledBlue, 0);}
			else if (seq[i] == 3) {digitalWrite(ledGreen, 1); delay(blinkDelay); digitalWrite(ledGreen, 0);}
			else if (seq[i] == 4) {digitalWrite(ledYellow, 1); delay(blinkDelay); digitalWrite(ledYellow, 0);}
			else if (seq[i] == 5) {digitalWrite(ledRed, 1); delay(blinkDelay); digitalWrite(ledRed, 0);}
			if ((levelDelay - 100*currLevel) >= 300) { // If scaled down delay exceeds 300 ms
			delay(levelDelay - 100*currLevel);} // Use shorter delay
			else{delay(300);} // Never let blink delay be less than 300 ms
		}
	}

}

// Restart the game - called when a user guesses incorrectly
void restartGame(){
	if (!boolCorrect) { // If user guessed wrong
		// Reset variables as if game has just begun
		level = 0; // Reset level to 0
		boolCorrect = true; // Reset initial guess value to true
		numButtonPress = 0; // Reset number of button presses to 0
		arrayEnd = false; // Reset so that the array's end hasn't been reached yet
	}
}

// When the user passes a level, flash a success pattern on the LEDs
void success(int level){
	for(int i = 0; i < level; i++){
		digitalWrite(ledBlue, 1);
		delay(30);
		digitalWrite(ledBlue, 0);
		delay(30);
		digitalWrite(ledGreen, 1);
		delay(30);
		digitalWrite(ledGreen, 0);
		delay(30);
		digitalWrite(ledYellow, 1);
		delay(30);
		digitalWrite(ledYellow, 0);
		delay(30);
		digitalWrite(ledRed, 1);
		delay(30);
		digitalWrite(ledRed, 0);
		delay(30);
	}
}

void setup() {
	Serial.begin(115200); // Set the data rate for serial data transmission
	// Call the pinMode() function for each of the four Button objects
	btnBlue.setPinMode();
	btnGreen.setPinMode();
	btnYellow.setPinMode();
	btnRed.setPinMode();
}

void loop() {
	// Turn off all 4 LEDs before next level begins
	digitalWrite(ledBlue, 0);
	digitalWrite(ledGreen, 0);
	digitalWrite(ledYellow, 0);
	digitalWrite(ledRed, 0);
	restartGame(); // Restart the game
	delay(1000);
	level++; // Increment the level the user is playing
	Serial.print("Level: "); // Print out the current level the user has reached
	Serial.println(level);
	int temp = level; // Temp variable use to compare current level value

	while(level == temp){ // Test this condition to exit level once it's completed
		success(level); // Light the success LED pattern at the start of each level to signal transition
		delay(500);
		temp = temp + makeSequence(sequence, level); // Generate random blink sequence
		blinkLEDS(sequence, level); // Blink the LED sequence
		numButtonPress = 0; // Reset number of button presses
		arrayEnd = false; // Reset end of array value to play next level

		while(boolCorrect && !arrayEnd){ // While user is pushing correct buttons
			delay(10); // Delay of 10 ms between button presses to debounce them as a whole
				// If the number of button presses equals the level number,
				// the end of the blink array has been reached, and the next level will begin.
				if (numButtonPress == level) {arrayEnd = true;}

				// Blue button push protocol
				val = digitalRead(inPinBlue); // Read value of Blue button
				digitalWrite(ledBlue, !val); // Light Blue LED if button is pressed
				if (val == 0) { // If button is pushed
					val = 2; // val is 2 when Blue button is pushed
					if (val != oldValBlue){ // If Blue button is pressed after is was not pressed
						numButtonPress++; // Increment button press counter by 1
						Serial.print("Number of Button Presses: "); // Print number of button presses at this point in a level
						Serial.println(numButtonPress);
						Serial.print("val = "); // Print the value of the current button press
						Serial.println(val);
						// Check the button press with the number in the blink sequence
						// If correct, stay in this loop and continue checking further presses
						if (checkValue(sequence[numButtonPress - 1], val) == 1){boolCorrect = true;}
						else{ // You lose
							// Blink the four LEDs 3 times to signify an incorrect guess. Game Over.
							for(int i = 0; i < 3; i++){
								digitalWrite(ledBlue, 1);
								digitalWrite(ledGreen, 1);
								digitalWrite(ledYellow, 1);
								digitalWrite(ledRed, 1);
								delay(500);
								digitalWrite(ledBlue, 0);
								digitalWrite(ledGreen, 0);
								digitalWrite(ledYellow, 0);
								digitalWrite(ledRed, 0);
								delay(500);
							}
							boolCorrect = false; // Set this to false to exit this while loop and restart the game
						}

					}
				}
				oldValBlue = btnBlue.debounce(val, oldValBlue); // Debounce the Blue button press

				// Green button push protocol
				val = digitalRead(inPinGreen); // Read value of Green button
				digitalWrite(ledGreen,!val); // Light Green LED if button is pressed
				if (val == 0) { // If button is pushed
					val = 3; // val is 3 when Green button is pushed
					if (val != oldValGreen){ // If Green button is pressed after is was not pressed
						numButtonPress++; // Increment button press counter by 1
						Serial.print("Number of Button Presses: "); // Print number of button presses at this point in a level
						Serial.println(numButtonPress);
						Serial.print("val = "); // Print the value of the current button press
						Serial.println(val);
						// Check the button press with the number in the blink sequence
						// If correct, stay in this loop and continue checking further presses
						if (checkValue(sequence[numButtonPress - 1], val) == 1){boolCorrect = true;}
						else{ // You lose
							// Blink the 4 LEDs 3 times to signify an incorrect guess. Game Over.
							for(int i = 0; i < 3; i++){
								digitalWrite(ledBlue, 1);
								digitalWrite(ledGreen, 1);
								digitalWrite(ledYellow, 1);
								digitalWrite(ledRed, 1);
								delay(500);
								digitalWrite(ledBlue, 0);
								digitalWrite(ledGreen, 0);
								digitalWrite(ledYellow, 0);
								digitalWrite(ledRed, 0);
								delay(500);
							}
							boolCorrect = false; // Set this to false to exit this while loop and restart the game
						}
					}
				}
				oldValGreen = btnGreen.debounce(val, oldValGreen); // Debounce the Green button press

				// Yellow button push protocol
				val = digitalRead(inPinYellow); // Read value of Yellow button
				digitalWrite(ledYellow, !val); // Light Yellow LED if button is pressed
				if (val == 0) { // If button is pushed
					val = 4; // val is 4 when Yellow button is pushed
					if (val != oldValYellow){ // If Yellow button is pressed after is was not pressed
						numButtonPress++; // Increment button press counter by 1
						Serial.print("Number of Button Presses: "); // Print number of button presses at this point in a level
						Serial.println(numButtonPress);
						Serial.print("val = "); // Print the value of the current button press
						Serial.println(val);
						// Check the button press with the number in the blink sequence
						// If correct, stay in this loop and continue checking further presses
						if (checkValue(sequence[numButtonPress - 1], val) == 1){boolCorrect = true;}
						else{ // You lose
							// Blink the 4 LEDs 3 times to signify an incorrect guess. Game Over.
							for(int i = 0; i < 3; i++){
								digitalWrite(ledBlue, 1);
								digitalWrite(ledGreen, 1);
								digitalWrite(ledYellow, 1);
								digitalWrite(ledRed, 1);
								delay(500);
								digitalWrite(ledBlue, 0);
								digitalWrite(ledGreen, 0);
								digitalWrite(ledYellow, 0);
								digitalWrite(ledRed, 0);
								delay(500);
							}
							boolCorrect = false; // Set this to false to exit this while loop and restart the game
						}
					}
				}
				oldValYellow = btnYellow.debounce(val, oldValYellow); // Debounce the Yellow button press

				// Red button push protocol
				val = digitalRead(inPinRed); // Read value of Red button
				digitalWrite(ledRed, !val); // Light Red LED if button is pressed
				if (val == 0) { // If button is pushed
					val = 5; // val is 5 when Red button is pushed
					if (val != oldValRed){ // If Red button is pressed after is was not pressed
						numButtonPress++; // Increment button press counter by 1
						Serial.print("Number of Button Presses: "); // Print number of button presses at this point in a level
						Serial.println(numButtonPress);
						Serial.print("val = "); // Print the value of the current button press
						Serial.println(val);
						// Check the button press with the number in the blink sequence
						// If correct, stay in this loop and continue checking further presses
						if (checkValue(sequence[numButtonPress - 1], val) == 1){boolCorrect = true;}
						else{ // You lose
							// Blink the 4 LEDs 3 times to signify an incorrect guess. Game Over.
							for(int i = 0; i < 3; i++){
								digitalWrite(ledBlue, 1);
								digitalWrite(ledGreen, 1);
								digitalWrite(ledYellow, 1);
								digitalWrite(ledRed, 1);
								delay(500);
								digitalWrite(ledBlue, 0);
								digitalWrite(ledGreen, 0);
								digitalWrite(ledYellow, 0);
								digitalWrite(ledRed, 0);
								delay(500);
							}
							boolCorrect = false; // Set this to false to exit this while loop and restart the game
						}
					}
				}
				oldValRed = btnRed.debounce(val, oldValRed); // Debounce the Red button press

		}
	}
}
