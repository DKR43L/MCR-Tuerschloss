#include <servo.h>

ServoControl::ServoControl(int servoPin, int buttonPin)
    : _servoPin(servoPin), _buttonPin(buttonPin), _pos(0), _buttonState(false), _lastButtonState(false) {}

void ServoControl::begin()
{
    Serial.begin(115200);
    pinMode(_buttonPin, INPUT_PULLUP);
    _myServo.attach(_servoPin);
    _myServo.write(_pos);
}

void ServoControl::update()
{
    _buttonState = digitalRead(_buttonPin) == LOW;

    if (_buttonState != _lastButtonState)
    {
        if (_buttonState)
        {
            _pos = (_pos == 0) ? 90 : 0;
            _myServo.write(_pos);
            delay(500);
        }
        _lastButtonState = _buttonState;
    }
}
