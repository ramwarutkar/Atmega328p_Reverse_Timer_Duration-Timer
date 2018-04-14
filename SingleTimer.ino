/*
  EEPROM:-
  0 - Second
  1 - Minute
  2 - Hours
  3 - Page_no
  4 - Integer i
  5 - Enable Button ON led
  6 - Enable Button OFF led
  7 - Date
*/



#include "Nokia_5110.h"
#include <DS3231.h>
#include <Wire.h>
#include <EEPROM.h>
DS3231 rtc(SDA, SCL);
Time  t;
int Hor;
int Min;
int Sec;
int Date;

#define RST 2
#define CE 3
#define DC 4
#define DIN 5
#define CLK 6
Nokia_5110 lcd = Nokia_5110(RST, CE, DC, DIN, CLK);

const int up = 8;       //  BUTTONS AND LED's
const int down = 9;
const int set_but = 10;
const int enable_but = 11;
const int pushbut_led = 12;
const int enable_on_led = 13;
const int enable_off_led = 7;

int tot_button_state = 0;  // TOTAL BUTTON STATE

static unsigned int duration_sec = 0;
static unsigned int duration_min = 0;
static unsigned int duration_hour = 0;
static unsigned int duration_count = 1;
static unsigned int duration_print = 1;
int temp_hour = 1;


void intro(void);
void set_duration(void);
void timer(void);
void led_light(void);

void setup()
{
  pinMode(up, INPUT);
  pinMode(down, INPUT);
  pinMode(set_but, INPUT);
  pinMode(enable_but, INPUT);
  pinMode(pushbut_led, OUTPUT);
  pinMode(enable_on_led, OUTPUT);
  pinMode(enable_off_led, OUTPUT);
  rtc.begin();
  intro();
  //EEPROM.write(3, 1); // page no
  EEPROM.write(4, 1); // i in Enable
  EEPROM.write(5, 1);
  EEPROM.write(6, 1);
  EEPROM.write(7, 1);
}

void loop()
{
  if (EEPROM.read(3) == 1)
  {
    do
    {
      set_duration();
    } while (EEPROM.read(3) == 1);
  }
  else if (EEPROM.read(3) == 2)
  {
    do
    {
      timer();
    } while (EEPROM.read(3) == 2);
  }
}

void intro()
{
  lcd.clear();
  lcd.setCursor(28, 1);
  lcd.print("SAMOM");
  lcd.setCursor(9, 2);
  lcd.print("Technologies");
  delay(2000);
  lcd.clear();
}

void set_duration()                   // DURATION  TIME FUNCTION
{
  if (duration_print == 1)
  {
    lcd.setCursor(0, 0);
    lcd.print("SET DURATION");
    lcd.setCursor(15, 3);
    lcd.print("00 : 00 : 00");
  }

  if (duration_count == 1)             //   FOR SETTING SECONDS
  {
    do
    {
      lcd.setCursor(57, 4);
      lcd.print("^");
      tot_button_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(100);
      switch (tot_button_state)
      {
        case 1:
          {
            if (duration_sec <= 58)
            {
              duration_sec++;
              led_light();
              lcd.clear(3, 57, 70);
              lcd.print(duration_sec);
            }
            break;
          }
        case 2:
          {
            if (duration_sec != 0)
            {
              duration_sec--;
              led_light();
              lcd.clear(3, 57, 70);
              lcd.print(duration_sec);
            }
            break;
          }
        case 3:
          {
            led_light();
            EEPROM.write(0, duration_sec);
            duration_count = 2;
            lcd.clear(4, 57, 75);
            duration_print = 0;
            break;
          }

        default: break;
      }
    } while (duration_count == 1);
  }
  else if (duration_count == 2)         //   FOR SETTING MINUTES
  {
    do
    {
      lcd.setCursor(36, 4);
      lcd.print("^");
      tot_button_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(100);
      switch (tot_button_state)
      {
        case 1:
          {
            if (duration_min <= 58)
            {
              duration_min++;
              led_light();
              lcd.clear(3, 36, 46);
              lcd.print(duration_min);
            }
            break;
          }
        case 2:
          {
            if (duration_min != 0)
            {
              duration_min--;
              led_light();
              lcd.clear(3, 36, 46);
              lcd.print(duration_min);
            }
            break;
          }
        case 3:
          {
            led_light();
            EEPROM.write(1, duration_min);
            duration_count = 3;
            lcd.clear(4, 36, 46);
            duration_print = 0;
            break;
          }
        default: break;

      }
    } while (duration_count == 2);
  }
  else if (duration_count == 3)                  // FOR SETTING HOURS
  {
    do
    {
      lcd.setCursor(15, 4);
      lcd.print("^");

      tot_button_state = digitalRead(up) * 1 + digitalRead(down) * 2 + digitalRead(set_but) * 3;
      delay(100);
      switch (tot_button_state)
      {
        case 1:
          {
            if (duration_hour <= 29)
            {
              duration_hour++;
              led_light();
              lcd.clear(3, 15, 25);
              lcd.print(duration_hour);
            }
            break;
          }
        case 2:
          {
            if (duration_hour != 0)
            {
              duration_hour--;
              led_light();
              lcd.clear(3, 15, 25);
              lcd.print(duration_hour);
            }
            break;
          }
        case 3:
          {
            led_light();
            if (duration_hour >= 24)
            {
              temp_hour = duration_hour - 24;
              EEPROM.write(2, temp_hour);
              EEPROM.write(7, 02);
            }
            else
            {
              EEPROM.write(2, duration_hour);
              EEPROM.write(7, 01);
            }
            duration_count = 4;
            lcd.clear();
            duration_print = 0;
            EEPROM.write(3, 2);
            break;
          }
        default: break;
      }

    } while (duration_count == 3);
  }
}

void timer()
{
  t = rtc.getTime();
  Hor = t.hour;
  Min = t.min;
  Sec = t.sec;
  Date = t.date;

  lcd.setCursor(20, 0);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(0, 1);
  lcd.print("TIMER ACTIVATED");
  lcd.setCursor(20, 2);
  lcd.print(EEPROM.read(2));
  lcd.setCursor(33, 2);
  lcd.print(":");
  lcd.setCursor( 39, 2);
  lcd.print(EEPROM.read(1));
  lcd.setCursor(53, 2);
  lcd.print(":");
  lcd.setCursor(60, 2);
  lcd.print(EEPROM.read(0));
  lcd.setCursor(0, 5);
  lcd.print("SET->RESET");

  if (digitalRead(enable_but) == HIGH )       //   IF ENABLE BUTTON IS ON
  {
    lcd.clear(4, 0, 60);
    lcd.setCursor(0, 4);
    lcd.print("ENABLE ON");

    if (EEPROM.read(4) == 1)
    {
      rtc.setTime(5 , 59 , 3);
      rtc.setDate(1, 1, 2018 );
      EEPROM.write(4, 2);
    }
    else if (EEPROM.read(4) == 2)
    {
      if (Date == EEPROM.read(7) && Hor == EEPROM.read(2) && Min == EEPROM.read(1) && Sec == EEPROM.read(0))
      {
        EEPROM.write(5, 2);
        //digitalWrite(enable_on_led, HIGH);
        EEPROM.write(4, 3);
        delay(200);
      }
    }
    else if (EEPROM.read(4) == 3)
    {
      rtc.setTime(5 , 59 , 3);
      rtc.setDate(1, 1, 2108 );
      EEPROM.write(4, 4);
    }
    else if (EEPROM.read(4) == 4)
    {
      if (Date == EEPROM.read(7) && Hor == EEPROM.read(2) && Min == EEPROM.read(1) && Sec == EEPROM.read(0))
      {
        EEPROM.write(5, 3);
        //digitalWrite(enable_on_led, LOW);
        duration_count = 1;
        duration_print = 1;
        duration_sec = 0;
        duration_min = 0;
        duration_hour = 0;
        EEPROM.write(4, 1);
        EEPROM.write(3, 1);
        EEPROM.write(7, 1);
        lcd.clear();
        delay(200);
      }
    }
  }


  else if (digitalRead(enable_but) == LOW)                                             // IF ENABLE BUTTON IS OFF
  {
    lcd.clear(4, 0, 60);
    lcd.setCursor(0, 4);
    lcd.print("ENABLE OFF");

    if (EEPROM.read(4) == 1)
    {
      rtc.setTime(0 , 0 , 0);
      rtc.setDate(1, 1, 2018 );
      EEPROM.write(4, 2);
    }
    else if (EEPROM.read(4) == 2)
    {

      if (Date == EEPROM.read(7) && Hor == EEPROM.read(2) && Min == EEPROM.read(1) && Sec == EEPROM.read(0))
      {
        EEPROM.write(6, 2);
        //digitalWrite(enable_off_led, HIGH);
        duration_count = 1;
        duration_print = 1;
        duration_sec = 0;
        duration_min = 0;
        duration_hour = 0;
        EEPROM.write(4, 1);
        EEPROM.write(3, 1);
        EEPROM.write(7, 1);
        lcd.clear();
        delay(200);
      }
    }
  }

  if (digitalRead(set_but) == HIGH)
  {
    duration_count = 1;
    duration_print = 1;
    duration_sec = 0;
    duration_min = 0;
    duration_hour = 0;
    EEPROM.write(3, 1);
    EEPROM.write(4, 1);
    EEPROM.write(5, 1);
    EEPROM.write(6, 1);
    EEPROM.write(7, 1);
    digitalWrite(enable_on_led, LOW);
    digitalWrite(enable_off_led, LOW);
    lcd.clear();
    delay(200);
  }

  // For Leds
  if (EEPROM.read(7) == 2)
  {
    lcd.setCursor(0, 3);
    lcd.print("TOMORROW");
  }
  if (EEPROM.read(5) == 2)
  {
    digitalWrite(enable_on_led, HIGH);
  }
  if (EEPROM.read(5) == 3)
  {
    digitalWrite(enable_on_led, LOW);
    EEPROM.write(5, 1);
  }
  if (EEPROM.read(6) == 2)
  {
    digitalWrite(enable_off_led, HIGH);
  }
}

void led_light()
{
  digitalWrite(pushbut_led, HIGH);
  delay(100);
  digitalWrite(pushbut_led, LOW);
}


