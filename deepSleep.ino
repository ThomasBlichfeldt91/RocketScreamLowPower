#include <RTCZero.h>
#include <LowPower.h>
#include <SPI.h>
#include <RH_RF95.h>
#define RF95_CS 5
#define RF95_RST 2
RH_RF95 rf95(RF95_CS, RF95_RST); 
#include <SerialFlash.h>
#define FLASH_CS 4

RTCZero rtc;

int ledPin = 13;

unsigned char count = 10;

const byte seconds = 0;
const byte minutes = 00;
const byte hours = 17;

const byte day = 17;
const byte month = 11;
const byte year = 15;

void setup()
{
  
// Turn off built in LED

pinMode(ledPin, OUTPUT);
digitalWrite(ledPin, LOW);

// Create timer. Timer is set to activate once every minute

rtc.begin();

rtc.setTime(hours, minutes, seconds);
rtc.setDate(day, month, year);

rtc.setAlarmTime(17, 00, 30);
rtc.enableAlarm(rtc.MATCH_SS);

rtc.attachInterrupt(alarmMatch);

// Countdown to 10 to prevent sleep lock

SerialUSB.println("Entering standby mode in:");

for (count; count > 0; count--)
{
SerialUSB.print(count);
SerialUSB.println(" s");
delay(1000);
}

// initialise radio and Serial Flash

rf95.init();
pinMode(RF95_CS, INPUT_PULLUP);
SerialFlash.begin(FLASH_CS);
}

void loop()
{
  
SerialUSB.println("Entering standby mode.");
SerialUSB.println("Alarm will wake up the device.");
SerialUSB.println("Zzzz…");

// Enter standby mode
sleepNow();
// Wake up
wakeUp();
// Wait for serial USB port to open
while(!SerialUSB);
// Serial USB is blazing fast, you might miss the messages
delay(1000);
SerialUSB.println("Awake!");
SerialUSB.println("Send any character to enter standby mode again");
// Wait for user response
while(!SerialUSB.available());
while(SerialUSB.available() > 0)
{
SerialUSB.read();
}
}

void alarmMatch()
{
  SerialUSB.println("ALARM!");
}

void sleepNow(){
  rf95.sleep();
  SerialFlash.sleep();
  LowPower.standby();
  }

void wakeUp(){
  rf95.available();
  SerialFlash.wakeup();
  }  
