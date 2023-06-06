//By Terrien-DJV5
//ButtonPISO.h
//Date Created: april 20 2023 4:28PM

#ifndef MY_BUTTONPISO_H
#define MY_BUTTONPISO_H
#include <Arduino.h>
class ButtonPISO {
  private:
    byte dataInPin;
    byte clockInPin;
    byte loadPin;
    uint8_t buttonState;
    uint8_t buttonLastState;
    
  public:
    ButtonPISO(byte dataInPin, byte clockInPin, byte loadPin);
    void init();
    void update();
    byte getState(byte buttonID);
    bool isPressed(byte buttonID);
    bool isHeld(byte buttonID);
    bool isTapped(byte buttonID);
    byte getRAWState();
    void overrideButtonState(byte buttonID, bool newState);
    
};
#endif
