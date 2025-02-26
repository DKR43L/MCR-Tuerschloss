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
uint8_t pin_column[COLUMN_NUM] = {25, 33, 32, 15};

// Initialize the Keypad library
Keypad keypad(makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM);

// Define the password
const String password = "1234";
String input = ""; // String to store user input

// Initialize the state
DoorState currentState = STATE_LOCKED;

// State machine function
void state_machine(Transition transition) {
    Serial.print("Aktueller Zustand vor Übergang: ");
    Serial.println(currentState); // Debugging: Zustand vor Änderung
    Serial.print("Übergang: ");
    Serial.println(transition);

    switch (currentState) {
        case STATE_LOCKED:
            if (transition == CODE_CORRECT) {
                currentState = STATE_UNLOCKED;
                Serial.println("State changed to: UNLOCKED");
            }
            break;

        case STATE_UNLOCKED:
            if(transition == CODE_INCORRECT)
            {
                currentState = STATE_LOCKED;
            }
            // if (transition == OPEN_DOOR) {
            //     currentState = STATE_OPEN;
            //     Serial.println("State changed to: OPEN");
            // } else if (transition == AUTO_LOCK) {
            //     currentState = STATE_LOCKED;
            //     Serial.println("State changed to: LOCKED");
            // }
            // else if(transition == CODE_INCORRECT)
            // {
            //     currentState = STATE_LOCKED;
            // }
            break;

        // case STATE_OPEN:
        //     if (transition == CLOSE_DOOR) {
        //         currentState = STATE_CLOSED;
        //         Serial.println("State changed to: CLOSED");
        //     }
        //     else if(transition == CODE_INCORRECT)
        //     {
        //         currentState = STATE_LOCKED;
        //     }
        //     break;

        // case STATE_CLOSED:
        //     if (transition == AUTO_LOCK) {
        //         currentState = STATE_LOCKED;
        //         Serial.println("State changed to: LOCKED");
        //     }
            break;
    }
    Serial.print("Neuer Zustand nach Übergang: ");
    Serial.println(currentState); // Debugging: Zustand nach Änderung
}

// Function to handle keypad input
void handleKeypadInput() {
    static unsigned long unlockTime = 0; // Zeitpunkt des Entriegelns

    char key = keypad.getKey();

    if (key) {
        if (key == '#') { // '#' bestätigt die Eingabe
            if (input == password) {
                state_machine(CODE_CORRECT);
                unlockTime = millis(); // Zeitpunkt speichern
            } else {
                state_machine(CODE_INCORRECT);
            }
            input = ""; // Eingabe zurücksetzen
        } else if (key == '*') { // '*' löscht die Eingabe
            input = "";
            Serial.println("Input cleared.");
        } else { // Zeichen hinzufügen
            input += key;
            Serial.println("Current input: " + input);
        }
    }

    // Auto-Lock nach 5 Sekunden
    // if (currentState == STATE_UNLOCKED && millis() - unlockTime >= 5000) {
    //     state_machine(AUTO_LOCK);
    // }
}