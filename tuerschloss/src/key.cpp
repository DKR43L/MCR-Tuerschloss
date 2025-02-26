#include "key.h"  
#include <Keypad.h>
#include <Arduino.h>

// Keymap for the 4x4 keypad
char keys[ROW_NUM][COLUMN_NUM] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Define the row and column pins
uint8_t pin_rows[ROW_NUM] = {12, 14, 27, 26};
uint8_t pin_column[COLUMN_NUM] = {25, 33, 32, 35};

// Initialize the Keypad library
Keypad keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Define the password
const String password = "1234";
String input = ""; // String to store user input

// Initialize the state
DoorState currentState = STATE_LOCKED;

// State machine function
void state_machine(Transition transition) {
    switch (currentState) {
        case STATE_LOCKED:
            if (transition == CODE_CORRECT) {
                currentState = STATE_UNLOCKED;
                Serial.println("State changed to: UNLOCKED");
            }
            break;

        case STATE_UNLOCKED:
            if (transition == OPEN_DOOR) {
                currentState = STATE_OPEN;
                Serial.println("State changed to: OPEN");
            } else if (transition == AUTO_LOCK) {
                currentState = STATE_LOCKED;
                Serial.println("State changed to: LOCKED");
            }
            break;

        case STATE_OPEN:
            if (transition == CLOSE_DOOR) {
                currentState = STATE_CLOSED;
                Serial.println("State changed to: CLOSED");
            }
            break;

        case STATE_CLOSED:
            if (transition == AUTO_LOCK) {
                currentState = STATE_LOCKED;
                Serial.println("State changed to: LOCKED");
            }
            break;
    }
}

// Function to handle keypad input
void handleKeypadInput() {
    char key = keypad.getKey();

    if (key) {
        if (key == '#') { // '#' is used to submit the input
            if (input == password) {
                Serial.println("Access granted!");
                state_machine(CODE_CORRECT); // Transition to UNLOCKED state
            } else {
                Serial.println("Access denied!");
                state_machine(CODE_INCORRECT); // Stay in LOCKED state
            }
            input = ""; // Reset the input after submission
        } else if (key == '*') { // '*' is used to clear the input
            input = "";
            Serial.println("Input cleared.");
        } else { // Append the key to the input
            input += key;
            Serial.println("Current input: " + input);
        }
    }

    // Simulate auto-locking after some time
    if (currentState == STATE_UNLOCKED) {
        delay(5000);
        state_machine(AUTO_LOCK);
    }
}
