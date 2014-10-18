#include <Wire.h>
#include "/hackTX14/rgb_lcd.h"
#include "/hackTX14/rgb_lcd.cpp"

rgb_lcd lcd;

int colorR = 100;
int colorG = 150;
int colorB = 200;

const int button_pin = 3; //
const int led_pin = 13;
const int touch_pin = 4;
const char blank_writer[16] = "               ";

int button_state = 0;
int touch_state = 0;
boolean toggle = false;
int button_count = 0;
int hold_time = 0;

void setup() 
{    
    pinMode(button_pin, INPUT);
    pinMode(touch_pin, INPUT);
    pinMode(led_pin, OUTPUT);
    lcd.begin(16,2);
    lcd.setRGB(100,100,100);
    lcd.print("test button");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("start!      ");
    delay(100);
    lcd.setCursor(0,0);
    lcd.print("count:      ");
    lcd.setCursor(0,1);
    lcd.print(blank_writer);//write all blank
    lcd.setCursor(9,0);
    lcd.print("touch");
}

void loop() 
{
    //delay(150);
    
    lcd.setCursor(0,1);
    lcd.print(button_count);
    button_state = digitalRead(button_pin);
    touch_state = digitalRead(touch_pin);
    
    if(touch_state == HIGH){
      lcd.setCursor(9,0);
      lcd.print("on   ");
    }
    else{
     lcd.setCursor(9,0);
     lcd.print("off   ");  
    }
    
//    if(button_state == LOW){
//      while(digitalRead(button_pin) == LOW){}       
//    }
    if(button_state == HIGH){
      toggle = !toggle;
      ++button_count;
      hold_time = 0;
//      while(digitalRead(button_pin) == HIGH){ //sample is in microseconds, b/c 1MHz processor
//        ++hold_time;//counts how long we have held it in ms
//      };
    }  
    lcd.setCursor(6,1);
    lcd.print("          ");
    lcd.setCursor(6,1);
    lcd.print(hold_time);  
    if(toggle)
      digitalWrite(led_pin, HIGH);
    else
      digitalWrite(led_pin, LOW);
    delay(10); //delay in MS
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
