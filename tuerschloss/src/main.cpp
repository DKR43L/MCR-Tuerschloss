#include <Arduino.h>
#include <lcd1602.h>
#include <servo.h>
#include <rfid.h>
#include <key.h>

lcd1602 lcd;           // Das Display-Objekt
byte allowedUID[] = {0xF5, 0x5E, 0x9, 0x1};
RFID_CP cpRFID(allowedUID, 4);
ServoControl servo(13); // Servo an Pin 13

void setup() {
  Serial.begin(115200); // Startet serielle Kommunikation für Debugging
  Serial.println("Setup beginnt...");
  lcd.setupLCD();       // Initialisiert das LCD
  Serial.println("LCD initialisiert");
  servo.begin();        // Initialisiert den Servo
  Serial.println("Servo initialisiert");
  //pinMode(13, OUTPUT);  // Servo-Pin als Ausgang (redundant, da servo.begin() das macht)
}

void loop() {
  // RFID-Transponder prüfen
  if (cpRFID.isTransponderNear()) {
    if (cpRFID.readTransponder()) {
      Serial.println("Korrekt");
      if(currentState == STATE_LOCKED)
        state_machine(CODE_CORRECT); // Tür entriegeln, wenn RFID korrekt
      else{
        state_machine(CODE_INCORRECT);
      }  
    } else {
      Serial.println("Falsch");
      state_machine(CODE_INCORRECT); // Verriegelt bleiben, wenn RFID falsch
    }
    cpRFID.clear(); // RFID-Status zurücksetzen
  }

  // Keypad-Eingabe verarbeiten
  handleKeypadInput(); // Verwaltet Keypad-Eingaben und Zustandsübergänge

  // Debugging: Aktuellen Zustand vor switch ausgeben

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
      Serial.println("Zustand: OPEN - Servo auf 90°");
      break;
    case STATE_CLOSED:
      servo.setPosition(0); // Tür wieder verriegeln (0°)
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Door Closed");
      Serial.println("Zustand: CLOSED - Servo auf 0°");
      break;
    default:
      Serial.println("Unbekannter Zustand");
      break;
  }
}