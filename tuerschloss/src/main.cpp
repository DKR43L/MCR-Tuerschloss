#include <Arduino.h>
#include <lcd1602.h>
#include <servo.h>
#include <rfid.h>
#include <key.h>

lcd1602 lcd;           // Das Display-Objekt
byte allowedUID[] = {0xF5, 0x5E, 0x9, 0x1};
RFID_CP cpRFID(allowedUID, 4);
ServoControl servo(13); // Servo an Pin 13 (du kannst den Pin anpassen)

void setup() {
  Serial.begin(9600); // Startet serielle Kommunikation für Debugging
  lcd.setupLCD();     // Initialisiert das LCD
  servo.begin();      // Initialisiert den Servo
}

void loop() {
  // RFID-Transponder prüfen
  if (cpRFID.isTransponderNear()) {
    if (cpRFID.readTransponder()) {
      Serial.println("Korrekt");
      state_machine(CODE_CORRECT); // Tür entriegeln, wenn RFID korrekt
    } else {
      Serial.println("Falsch");
      state_machine(CODE_INCORRECT); // Verriegelt bleiben, wenn RFID falsch
    }
    cpRFID.clear(); // RFID-Status zurücksetzen
  }

  // Keypad-Eingabe verarbeiten
  handleKeypadInput(); // Verwaltet Keypad-Eingaben und Zustandsübergänge

  // Servo und LCD basierend auf aktuellem Zustand aktualisieren
  switch (currentState) {
    case STATE_LOCKED:
      servo.setPosition(0); // Tür verriegeln (0°)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Locked");
      lcd.setCursor(0, 1);
      lcd.print("Enter Code/RFID");
      break;
    case STATE_UNLOCKED:
      servo.setPosition(90); // Tür entriegeln (90°)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Unlocked");
      break;
    case STATE_OPEN:
      servo.setPosition(90); // Tür bleibt entriegelt (90°)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door Open");
      break;
    case STATE_CLOSED:
      servo.setPosition(0); // Tür wieder verriegeln (0°)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door Closed");
      break;
  }
}