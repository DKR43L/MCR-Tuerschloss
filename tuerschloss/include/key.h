#ifndef KEY_H
#define KEY_H

#include <Arduino.h>
#include <Keypad.h>  

#define ROW_NUM 4
#define COLUMN_NUM 4

// Keymap for the 4x4 keypad
extern char keys[ROW_NUM][COLUMN_NUM];

// Define the row and column pins
extern uint8_t pin_rows[ROW_NUM];
extern uint8_t pin_column[COLUMN_NUM];

// Passwort
extern const String password;
extern String input;

// Zustände
typedef enum {
    STATE_LOCKED,
    STATE_UNLOCKED,
    STATE_OPEN,
    STATE_CLOSED
} DoorState;

// Übergänge
typedef enum {
    CODE_CORRECT,
    CODE_INCORRECT,
    OPEN_DOOR,
    CLOSE_DOOR,
    AUTO_LOCK
} Transition;

// Aktueller Zustand
extern DoorState currentState;

// Funktionsdeklarationen
void state_machine(Transition transition);
void handleKeypadInput();

#endif // KEY_H
