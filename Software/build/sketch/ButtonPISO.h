#line 1 "/home/hacktopubuntu/Documents/GitHub/Netogatchi-ESP32-V1/Software/tamagotchi_V0.0.0_for_Hardware_V1/ButtonPISO.h"
//By Terrien-DJV5
//ButtonPISO.h
//Date Created: april 20 2023 4:28PM

#ifndef MY_BUTTONPISO_H
#define MY_BUTTONPISO_H
#include <Arduino.h>
class ButtonPISO {
  private:
    volatile byte dataInPin;
    volatile byte clockInPin;
    volatile byte loadPin;
    volatile uint8_t buttonState;
    volatile uint8_t buttonLastState;
    byte getState(byte buttonID);
  public:
    ButtonPISO(byte dataInPin, byte clockInPin, byte loadPin);
    void init();
    void update();
    bool isPressed(byte buttonID);
    bool isHeld(byte buttonID);
    bool isTapped(byte buttonID);
    byte getRAWState();
    void overrideButtonState(byte buttonID, bool newState);
    
};
#endif
