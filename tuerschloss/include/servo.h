#ifndef servo.h
#define servo.h

#include <Arduino.h>
#include <ESP32Servo.h>

class ServoControl
{
public:
    ServoControl(int servoPin, int buttonPin);
    void begin();
    void update();

private:
    int _servoPin;
    int _buttonPin;
    Servo _myServo;
    int _pos;
    bool _buttonState;
    bool _lastButtonState;
};

#endif
