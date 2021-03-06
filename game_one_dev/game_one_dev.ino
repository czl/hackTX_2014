#include <Wire.h>
#include "/hackTX14/rgb_lcd.h"
#include "/hackTX14/rgb_lcd.cpp"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

rgb_lcd lcd;

int colorR = 100;
int colorG = 150;
int colorB = 200;

const int button_pin = 3; //
const int led_pin = 13;
const int touch_pin = 4;
const int buzz_pin = 8;
const char blank_writer[17] = "                ";
int level;
const int max_level = 0; //index count
const int rxn_time[6] = {30, 29, 27, 24, 19, 13};//time to react, decreases as levels go up

int button_state = 0;
int touch_state = 0;
/*
boolean toggle = false;
int button_count = 0;
int hold_time = 0;
*/

const unsigned int map_0[] = {0,0,1,0,0,1,0,1,1,1,0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const unsigned int map_1[] = {1,0,0,1,1,0,1,1,1,0,1,1,0,1,0,1,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned int ptr0, ptr1;
unsigned int ptr_max = 21; //when pointer gets to index 15 we know that is the last part of map

int val0, val1, user0, user1, loop_count;
boolean good, start_delay;
boolean need_setup = true;
boolean cold_start = true;

boolean correct_press();//checks to see if the user pressed the correct buttons
void map_display();//const unsigned int& map0, const unsigned int& map1, unsigned int& p0, unsigned int& p1);
void clear_screen();
void our_setup();

//C io stuff
FILE *f;
f = fopen("/home/root/score.txt", "w+");
/fprintf(f, "score: %d", 8);

system("echo 8 > /home/root/score.txt");


void clear_screen()
{
    lcd.setCursor(0,0);
    lcd.print(blank_writer);
    lcd.setCursor(0,1);
    lcd.print(blank_writer);
}

void print_intro()
{
    lcd.setCursor(0,0);
    lcd.print("BINARY HERO");
    lcd.setCursor(0,1);
    lcd.print("Hit the X's...!!");
    delay(2000);
    clear_screen();
    lcd.setCursor(0,0);
    lcd.print("by Clemens Lee");
    lcd.setCursor(0,1);
    lcd.print(" & Brandon Dang");
    delay(2000);
    clear_screen();
}

void setup() 
{    
}

boolean temp_check()
{
   lcd.setRGB(0,40, 180);
   int a;
   float temp = 0;
   int B = 3975;
   float resistance;
   lcd.setCursor(0,0);
   lcd.print("Temp Check");
   lcd.setCursor(0,1);
   lcd.print("un-human temp");
   delay(800);
   while(temp<29)
   {
     a = analogRead(0);
     resistance=(float)(1023-a)*10000/a; //get the resistance of the sensor;
     temp=1/(log(resistance/10000)/B+1/298.15)-273.15;//convert to temperature via datasheet ;
     delay(500);
     clear_screen();
     lcd.setCursor(0,0);
     lcd.print("too cold...");
     lcd.setCursor(0,1);
     lcd.print("you aren't human");
     delay(300);
     if(temp>=29)
       break;
   }
   clear_screen();
   lcd.setCursor(0,0);
   lcd.print("you seem human");
   delay(750);
}

//our_setup
void our_setup()
{
    pinMode(button_pin, INPUT);
    pinMode(touch_pin, INPUT);
    pinMode(buzz_pin, OUTPUT);
    pinMode(led_pin, OUTPUT);
    lcd.begin(16,2);
    
    if(cold_start)
    {
      temp_check();
      cold_start = false;
    }

    lcd.setRGB(100,100,100);    
    val0 = 0;
    val1 = 0;
    user0 = 0;
    user1 = 0;
    good = false;
    loop_count = 0;
    start_delay = true;
    ptr0 = 0;
    ptr1 = 0;
    level = 0;
    
    //print_intro();
    clear_screen();
    lcd.setCursor(0,0);
    lcd.print("BEGIN GAME 0");
    delay(2000);
    lcd.setCursor(0,0);
    lcd.print("start!          ");
    delay(100);
//    lcd.setCursor(0,0);
//    lcd.print("count:      ");
    clear_screen();
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

void delay_beep(int delay_time, int tone1, int tone2)
{
   if(start_delay)
   {
      for(int i = 0; i < 3; i++)
      {
        delay(delay_time);
        tone(buzz_pin, tone1, 200);
      }
      delay(delay_time);
      tone(buzz_pin, tone2, 200);
    }
}

void win_lvl()
{
  tone(buzz_pin, 1000, 300);
  tone(buzz_pin, 1500, 300);
  tone(buzz_pin, 3000, 400);
}

void win_all()
{
  tone(buzz_pin, 1500, 200);
  tone(buzz_pin, 1500, 200);
  tone(buzz_pin, 1500, 200);
  tone(buzz_pin, 1500, 200);
  tone(buzz_pin, 3000, 500);
}

void lose()
{
  tone(buzz_pin, 1500, 200);
  tone(buzz_pin, 1000, 200);
  tone(buzz_pin, 700, 200);
  tone(buzz_pin, 500, 400);
}

void our_loop()
{
  lcd.setRGB(100,100,100);
  delay_beep(400, 1900, 3000);
  ++loop_count;
  button_state = digitalRead(button_pin);
  touch_state = digitalRead(touch_pin);
  if(button_state == HIGH)
    user1 = 1;
  else
    user1 = 0;
  if(touch_state == HIGH)
    user0 = 1;
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
    delay(10);
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
  
  if(ptr0>=ptr_max)//won this level!
  {
    if(level <= max_level)
    {
      win_lvl();
      lcd.setRGB(0,0,255);
    }
    clear_screen();
    lcd.setCursor(0,0);
    lcd.print("DONE! YOU WON!!");
    lcd.setCursor(0,1);
    lcd.print(blank_writer);
    ++level;//increase level
    
    //set pointers back to 0
    ptr0 = 0;
    ptr1 = 0;
    
    if(level > max_level)//you beat the game
    {
       level = 0;
       win_all();
       while(true)
       {
         clear_screen();
         lcd.setRGB(0,255,0);
         lcd.setCursor(0,0);
         lcd.print("GAME CHAMP!"); 
         if(digitalRead(button_pin) || digitalRead(touch_pin))
         {
           need_setup = true;
           break;
         }
         delay(1000);
         clear_screen();
         lcd.setCursor(0,0);
         lcd.print("hold button");
         lcd.setCursor(0,1);
         lcd.print("to play again");
         if(digitalRead(button_pin) || digitalRead(touch_pin))
         {
           need_setup = true;
           break;
         }
         delay(500);
         if(digitalRead(button_pin) || digitalRead(touch_pin))
         {
           need_setup = true;
           break;
         }
         delay(500);
         if(digitalRead(button_pin) || digitalRead(touch_pin))
         {
           need_setup = true;
           break;
         }
       }
    }
    delay(1000);
    clear_screen();
    lcd.setCursor(0,0);
    lcd.print("Starting Lvl: ");
    lcd.print(level);
    delay(500);
    start_delay = true;
  }
  else if(loop_count>=rxn_time[level] && !good)//lost
  {
    lcd.setRGB(200,0,0);//red bg
    lose();
    while(true)
    {
      clear_screen();
      lcd.setCursor(0,0);
      lcd.print("GAME OVER");
      delay(1000);
      clear_screen();
      lcd.setCursor(0,0);
      lcd.print("hold button");
      lcd.setCursor(0,1);
      lcd.print("to play again");
      if(digitalRead(button_pin) || digitalRead(touch_pin))
      {
        need_setup = true;
        break;
      }
      delay(500);
      if(digitalRead(button_pin) || digitalRead(touch_pin))
      {
        need_setup = true;
        break;
      }
      delay(500);
    }
    //lcd.print(ptr0); //used to figure out which index it failed at
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

void loop() 
{
  if(need_setup)
  {
    our_setup();
    need_setup = false;
  }
  our_loop();
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
