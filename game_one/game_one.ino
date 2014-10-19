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
const char blank_writer[17] = "                ";
int level = 0;
const int rxn_time[6] = {30, 29, 27, 24, 19};//time to react, decreases as levels go up

int button_state = 0;
int touch_state = 0;
boolean toggle = false;
int button_count = 0;
int hold_time = 0;

const unsigned int map_0[] = {0,0,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned int map_1[] = {1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int ptr0 = 0;
unsigned int ptr1 = 0;
unsigned int ptr_max = 16; //when pointer gets to index 15 we know that is the last part of map

int val0 = 0;
int val1 = 0;
int user0 = 0;
int user1 = 0;
boolean good = false;
int loop_count = 0;
boolean start_delay = true;

boolean correct_press();//checks to see if the user pressed the correct buttons
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
    delay(30);
}

//map display function
void map_display()//const unsigned int& map0, const unsigned int& map1, unsigned int& p0, unsigned int& p1)
{
  lcd.setCursor(0,0);
  int i = 0;
  //setting val0 and val1
  val0 = map_0[ptr0];
  val1 = map_1[ptr1];
  //displaying map
  for(;i<16;++i)
  {
    lcd.setCursor(i,0);
    if(map_0[i+ptr0])
      lcd.print("X"); 
    else
      lcd.print(" ");
    lcd.setCursor(i,1);
    if(map_1[i+ptr1])
      lcd.print("X");
    else
      lcd.print(" ");
  } 
  //delay(700);
}

boolean correct_press()
{
//   delay(80);
   return (user0==val0 && user1==val1);
}

void loop() 
{
  if(start_delay)
  {
    delay(400);
    tone(buzz_pin, 1900, 200); 
    delay(400);
    tone(buzz_pin, 1900, 200);
    delay(400);
    tone(buzz_pin, 1900, 200);
    delay(400);
    tone(buzz_pin, 3000, 200);
  }
  ++loop_count;
  button_state = digitalRead(button_pin);
  touch_state = digitalRead(touch_pin);
  if(button_state == HIGH)
  {
    user1 = 1;
  }
  else
    user1 = 0;
  if(touch_state == HIGH)
  {
    user0 = 1;
  }
  else
    user0 = 0;
//  lcd.setCursor(0,0);
//  lcd.print("user0:");
//  lcd.print(user0);
//  lcd.setCursor(0,1);
//  lcd.print("user1:");
//  lcd.print(user1);
//  delay(100);
  if(start_delay)
  {
    delay(300);
    start_delay = false;
  }
  if(correct_press())
  {
    //++ptr0;
    //++ptr1; 
    good = true;
  }
  if(ptr0<ptr_max)
  { 
   //delay(100);
    map_display();
  }
  
  if(ptr0>=ptr_max)
  {
    lcd.setCursor(0,0);
    lcd.print(blank_writer);
    lcd.print(blank_writer);
    lcd.setCursor(0,0);
    lcd.print("DONE! YOU WON!!");
    lcd.setCursor(0,1);
    lcd.print(blank_writer);
    while(true){}
  }
  if(loop_count>=rxn_time[level] && !good)//lost
  {
    lcd.setCursor(0,0);
    lcd.print(blank_writer);
    lcd.setCursor(0,1);
    lcd.print(blank_writer);
    lcd.setCursor(0,0);
    lcd.print("GAME OVER");
    //lcd.print(ptr0); //used to figure out which index it failed at
    lcd.setCursor(0,1);
    lcd.print("TRY AGAIN");
    while(true){}
  }
  else if(loop_count>=rxn_time[level] && good)//continue
  {
    ++ptr0;
    ++ptr1; 
    loop_count = 0;
    good = false;
  }
  delay(10);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
