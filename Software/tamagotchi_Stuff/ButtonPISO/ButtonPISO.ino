//By Terrien-DJV5
//ButtonPISO
//Date Created: april 20 2023 4:28PM


#include "ButtonPISO.h"
//helpful:
//https://roboticsbackend.com/arduino-object-oriented-programming-oop/
//https://dronebotworkshop.com/shift-registers/



// Define Connections to 74HC165

// PL pin 1
#define BUTTONPISO_PL 7//32//7
// Q7 pin 7
#define BUTTONPISO_DataIn 5//35//5
// CP pin 2
#define BUTTONPISO_Clk 6//33//6






ButtonPISO buttonpiso1(BUTTONPISO_DataIn, BUTTONPISO_Clk, BUTTONPISO_PL);


void setup() {
  // put your setup code here, to run once:
  // Setup Serial Monitor
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  buttonpiso1.update();
  delay(100);
  if (buttonpiso1.isPressed(0)==true){
    Serial.println("Button 0 was Pressed!");
  };
  if (buttonpiso1.isPressed(1)==true){
    Serial.println("Button 1 was Pressed!");
  };
  if (buttonpiso1.isPressed(2)==true){
    Serial.println("Button 2 was Pressed!");
  };
  if (buttonpiso1.isPressed(3)==true){
    Serial.println("Button 3 was Pressed!");
  };
  if (buttonpiso1.isPressed(4)==true){
    Serial.println("Button 4 was Pressed!");
  };
  if (buttonpiso1.isPressed(5)==true){
    Serial.println("Button 5 was Pressed!");
  };
  if (buttonpiso1.isPressed(6)==true){
    Serial.println("Button 6 was Pressed!");
  };
  if (buttonpiso1.isPressed(7)==true){
    Serial.println("Button 7 was Pressed!");
  };
}



  
