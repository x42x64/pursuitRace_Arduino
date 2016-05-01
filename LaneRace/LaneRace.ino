#include "TimerOne.h"

#define RED 0
#define GREEN 1
#define BLUE 2
#define CHANNELS_PER_LANE 3

#define NUM_LANES 6


String command; // String input from command prompt
String temp1, temp2; // temporary strings
char inByte; // Byte input from command prompt
char carray[6]; // character array for string to int // manipulation
int a, b, c; // temporary numbers

typedef struct {
  int pins[CHANNELS_PER_LANE];
  unsigned char intensity[CHANNELS_PER_LANE];
  int cnt1;
  int cnt2;

  unsigned int time2start; // in ms
  int cntBlink;
} tLane;


tLane lanes[NUM_LANES];
unsigned int timeSettings[NUM_LANES];

void updateColor(tLane* lane)
{
  int i;
  for (i = 0; i < CHANNELS_PER_LANE; i++)
  {
    digitalWrite(lane->pins[i], (lane->cnt1 < lane->intensity[i]));
  }
}

void setColor(tLane* lane, unsigned char red, unsigned char green, unsigned char blue)
{
  lane->intensity[RED] = red;
  lane->intensity[GREEN] = green;
  lane->intensity[BLUE] = blue;

  updateColor(lane);
}



void setup() {
  int i, j;

  // put your setup code here, to run once:
  lanes[0].pins[RED] =53;
  lanes[0].pins[GREEN] = 51;
  lanes[0].pins[BLUE] = 47;

  lanes[1].pins[RED] = 43;
  lanes[1].pins[GREEN] = 39;
  lanes[1].pins[BLUE] = 35;

  lanes[2].pins[RED] = 31;
  lanes[2].pins[GREEN] = 27;
  lanes[2].pins[BLUE] = 23;

  lanes[3].pins[RED] = 22;
  lanes[3].pins[GREEN] = 26;
  lanes[3].pins[BLUE] = 30;

  lanes[4].pins[RED] = 34;
  lanes[4].pins[GREEN] = 38;
  lanes[4].pins[BLUE] = 42;

  lanes[5].pins[RED] = 46;
  lanes[5].pins[GREEN] = 50;
  lanes[5].pins[BLUE] = 52;

//  lanes[6].pins[RED] = 28;
//  lanes[6].pins[GREEN] = 28;
//  lanes[6].pins[BLUE] = 28;
//
//  lanes[7].pins[RED] = 28;
//  lanes[7].pins[GREEN] = 28;
//  lanes[7].pins[BLUE] = 28;



  for (j = 0; j < NUM_LANES; j++)
  {
    for (i = 0; i < CHANNELS_PER_LANE; i++)
    {
      pinMode(lanes[j].pins[i], OUTPUT);
    }

    timeSettings[j] = 5000;
  }

  for (i = 0; i < NUM_LANES; i++)
  {
    lanes[i].cnt1 = 0;
    lanes[i].cnt2 = 0;

    setColor(&(lanes[i]), 255, 0, 0);
    lanes[i].time2start = timeSettings[i];

  }


  Serial.begin(9600);
  Serial.println("Hello, enter your name!");

  Timer1.attachInterrupt(isr_call);


}

void start()
{
  int i;
  for (i = 0; i < NUM_LANES; i++)
  {
    lanes[i].cnt1 = 0;
    lanes[i].cnt2 = 0;

    setColor(&(lanes[i]), 255, 0, 0);
    lanes[i].time2start = timeSettings[i];

  }
  Timer1.initialize(1000);
}

void isr_call()
{
  //  task_50us();
  task_1ms();
}

void task_1ms ()
{
  int j;

  for (j = 0; j < NUM_LANES; j++)
  {
    if (lanes[j].time2start > 0)
    {
      lanes[j].time2start--;
      setColor(&lanes[j], 255, 0, 0);
    }
    else
    {
      setColor(&lanes[j], 0, 255, 255);
    }
  }

}



void loop() {
  int currentLane = 9;
  // Input serial information:
  if (Serial.available() > 0)
  {
    inByte = Serial.read();
    // only input if a letter, number, =,?,+ are typed!
    if ((inByte >= 65 && inByte <= 90) || (inByte >= 97 && inByte <= 122) || (inByte >= 48 &&     inByte <= 57) || inByte == 43 || inByte == 61 || inByte == 63 || inByte == ':')
    {
      command.concat(inByte);
    }

    if (inByte == 10 || inByte == 13)
    {
      if (command.equals("go"))
      {
        Serial.println("Started...");
        start();
      }
      else if (command.length() > 4 && command.charAt(0) == 'L' && command.charAt(2) == ':')
      {
        if (command.charAt(1) >= '0' && command.charAt(1) <= '8')
        {
          currentLane = command.charAt(1) - '0';
          if (currentLane >= 0 && currentLane < NUM_LANES)
          {
            timeSettings[currentLane] = (unsigned int) command.substring(3).toInt();
            Serial.print("Set lane ");
            Serial.print(currentLane);
            Serial.print(" to start time ");
            Serial.print(timeSettings[currentLane]);
            Serial.print("ms \r\n");
          }
        }
      }
      else
      {
        Serial.println("Invalid command:");
        Serial.println(command);
      }
      command = "";
    }
  }
}
