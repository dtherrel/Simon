// Button.h - Button class definition takes place here
// by Matthew Hirsch and Dylan Herrel
// CSC 113 - Assignment 2
// 10/4/17

#ifndef BUTTON_H_ // If Button.h is not already defined
#define BUTTON_H_ // Define Button.h

// definition of class Button
class Button {
  private:
  	int ledPin; // Output pin used to light LED
  	int inPin;  // Input pin used to read user input from given button
  public:
  	// Default Constructor: initializes ledPin to 10 and inPin to 4
  	Button();

  	// Specific Constructor: initializes pins used for LED output and button input
  	Button(const int newLedPin, const int newInPin);

  	// Calls pinMode() function for a button/LED pair
  	void setPinMode();

  	// Debounces button press of specific button
  	int debounce(int currVal, int oldVal);
};

#endif /* BUTTON_H_ */
