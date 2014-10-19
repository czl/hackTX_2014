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
const int buzz_pin = 8;
const char blank_writer[16] = "               ";

int button_state = 0;
int touch_state = 0;
boolean toggle = false;
int button_count = 0;
int hold_time = 0;

const unsigned int map_0[] = {0,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned int map_1[] = {1,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int ptr0 = 0;
unsigned int ptr1 = 0;

void map_display();//const unsigned int& map0, const unsigned int& map1, unsigned int& p0, unsigned int& p1);

void setup() 
{    
    pinMode(button_pin, INPUT);
    pinMode(touch_pin, INPUT);
    pinMode(buzz_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    lcd.begin(16,2);
    lcd.setRGB(100,100,100);
    lcd.print("BEGIN GAME 1");
    delay(1000);
    lcd.setCursor(0,0);
    lcd.print("start!          ");
    delay(100);
    lcd.setCursor(0,0);
//    lcd.print("count:      ");
    lcd.setCursor(0,0);
    lcd.print(blank_writer);//write all blank
    lcd.setCursor(0,1);
    lcd.print(blank_writer);
    delay(200);
    map_display();
}

//map display function
void map_display()//const unsigned int& map0, const unsigned int& map1, unsigned int& p0, unsigned int& p1)
{
  lcd.setCursor(0,0);
  int i = 0;
  for(;i<16;++i)
  {
    lcd.setCursor(i,0);
    if(map_0[i])
      lcd.print("X"); 
    else
      lcd.print(" ");
    lcd.setCursor(i,1);
    if(map_1[i])
      lcd.print("X");
    else
      lcd.print(" ");
  } 
}

void loop() 
{
    
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
