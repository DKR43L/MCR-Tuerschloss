#include "lcd1602.h"


lcd1602::lcd1602() {
    lcd = new LiquidCrystal_I2C(0x27, 16, 2);
}   

void lcd1602::clear() {
    lcd->clear();
}

void lcd1602::setCursor(int x, int y) {
    lcd->setCursor(x, y);
}   

void lcd1602::print(String message) {
    lcd->print(message);
}   
void lcd1602::setupLCD() {
    lcd->init();
    lcd->backlight();
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Ready!");
    delay(1000);
    lcd->clear();
}

void lcd1602::displayDefaultMessage() {
    lcd->clear();
    lcd->setCursor(0, 0);
    lcd->print("Enter Combination:");
}