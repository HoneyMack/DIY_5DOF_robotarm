#include <Arduino.h>

#ifndef PUTHANDLER_H
#define PUTHANDLER_H

class PutHandler{
private:
  int value;
  int pin;
  bool isDigital;
  int mode;
public:
  PutHandler(int _pin,int _mode = INPUT,bool _isDigital = false){
    pin = _pin;
    mode = _mode;
    isDigital = _isDigital;
    value = 0;
    
    if(isDigital){
      pinMode(pin,mode);
    }
  }
  int GetValue(){
    if(mode == INPUT){
      if(isDigital)
        value = digitalRead(pin);
      else
        value = analogRead(pin);
    }
    return value;
  }
  void SetValue(int i){
    if(mode == OUTPUT){
      if(isDigital)
        digitalWrite(pin,value);
      else
        analogWrite(pin,value);
    }
  }
};

#endif
