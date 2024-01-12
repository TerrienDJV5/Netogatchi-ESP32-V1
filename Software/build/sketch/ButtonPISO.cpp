#line 1 "/home/hacktopubuntu/Documents/GitHub/Netogatchi-ESP32-V1/Software/tamagotchi_V0.0.0_for_Hardware_V1/ButtonPISO.cpp"
//By Terrien-DJV5
//ButtonPISO.cpp
//Date Created: april 20 2023 4:28PM

#include "ButtonPISO.h"
ButtonPISO::ButtonPISO(byte dataInPin, byte clockInPin, byte loadPin)
{
  this->dataInPin = dataInPin;
  this->clockInPin = clockInPin;
  this->loadPin = loadPin;
  init();
}

void ButtonPISO::init()
{
  // Setup 74HC165 connections
  pinMode(loadPin, OUTPUT);
  digitalWrite(loadPin, HIGH);
  pinMode(clockInPin, OUTPUT);
  pinMode(dataInPin, INPUT);
  update();
}

void ButtonPISO::update()
{
  buttonLastState = buttonState;
  // Write pulse to load pin
  digitalWrite(clockInPin, LOW);
  digitalWrite(loadPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(loadPin, LOW);
  delayMicroseconds(10);
  digitalWrite(loadPin, HIGH);
  delayMicroseconds(10);
  
  // Get data from 74HC165
  //digitalWrite(clockEnablePin, LOW);
  buttonState = shiftIn(dataInPin, clockInPin, MSBFIRST);
  //digitalWrite(clockEnablePin, HIGH);
}

//Private
byte ButtonPISO::getState(byte buttonID)
{
  return bitRead(buttonState, buttonID);
}


bool ButtonPISO::isPressed(byte buttonID)
{
  return (getState(buttonID) == HIGH);
}

bool ButtonPISO::isHeld(byte buttonID)
{
  return ((bitRead(buttonState, buttonID) == HIGH) and (bitRead(buttonLastState, buttonID)==HIGH));
}

bool ButtonPISO::isTapped(byte buttonID)
{
  return ((bitRead(buttonState, buttonID) == HIGH) and (bitRead(buttonLastState, buttonID)==LOW));
}

byte ButtonPISO::getRAWState()
{
  return buttonState;
}

void ButtonPISO::overrideButtonState(byte buttonID, bool newState)
{
  bitWrite(buttonState, buttonID, newState);
}
