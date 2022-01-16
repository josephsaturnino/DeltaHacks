  

#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>     
#include"ServoTimer2.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//create an RF24 object
RF24 radio(9, 8);  
ServoTimer2 servo1;
const int trigger = 7;
const int PulseWire = 0;       
int Threshold = 550;
bool safe = false;     


const byte address[6] = "00001";
PulseSensorPlayground pulseSensor;

void setup()
{
  servo1.attach(4); 
  servo1.write(750);
  Serial.begin(9600);
  radio.begin();
  
  //set the address
  radio.openWritingPipe(address);
  
  //Set module as transmitter
  radio.stopListening();

   pinMode(trigger, INPUT_PULLUP);
    pulseSensor.analogInput(PulseWire);   
  pulseSensor.setThreshold(Threshold); 
  if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !"); 
  }
}
void loop()
{
  int myBPM = 0;
  String BPM = "";
  for(int f = 0; f < 10; f++)
  {
    myBPM = pulseSensor.getBeatsPerMinute();
    if (pulseSensor.sawStartOfBeat()) {            
    Serial.println("♥  A HeartBeat Happened ! "); 
    Serial.print("BPM: ");                        
    Serial.println(myBPM);                        
    }
    delay(20);
  }
  BPM = String(myBPM);
  radio.write(&BPM, sizeof(BPM));
  delay(200);
  if(myBPM <= 50)
  {
    Serial.print("Do you need help");
    for(int w = 1; w <= 10; w++)
    {
      if(digitalRead(trigger) == LOW)
      {
        safe = true;
      }
      delay(1000);
      Serial.println(w);
    }
    if(safe == false)
    {
      Serial.println("Sending distress");
    servo1.write(750);  
    delay(3000);
    servo1.write(1500);  
    const char text[] = "HELP";
    radio.write(&text, sizeof(text));
    delay(1000);
    }
    else
    {
      Serial.print("turn button off");
      while(digitalRead(trigger) == LOW)
      {
        delay(10);
      }
    }
   
  }
  
  if(digitalRead(trigger) == LOW)
  {
    Serial.println("Sending distress");
    servo1.write(750); 
    delay(3000);
    servo1.write(1500);  
    const char text[] = "Help";
    radio.write(&text, sizeof(text));
    delay(1000);
  }
    
}
