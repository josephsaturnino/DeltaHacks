//Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SD.h>
String SSIDf;
String PASSf;
File myFile;
RF24 radio(9, 8); 

const byte address[6] = "00001";

void setup()
{
  Serial.begin(9600);
while (!Serial) {
;  
}
Serial.print("Initializing SD card...");
if (!SD.begin(10)) {
Serial.println("initialization failed!");
while (1);
}
Serial.println("initialization done.");
myFile = SD.open("SSID.txt");
if (myFile) {
while (myFile.available()) {
SSIDf = myFile.readStringUntil('\r');
}
myFile.close();
} else {
Serial.println("error opening time.txt");
}

myFile = SD.open("PASS.txt");
if (myFile) {
while (myFile.available()) {
PASSf = myFile.readStringUntil('\r');
}
myFile.close();
} else {
Serial.println("error opening num.txt");
}

delay(500);
Serial.println(SSIDf);
Serial.println(PASSf);

  
  radio.begin();
  

  radio.openReadingPipe(0, address);
 
  radio.startListening();
}

void loop()
{

  if (radio.available())
  {
    char text[32] = {0};
    radio.read(&text, sizeof(text));
    Serial.println(text);
    myFile = SD.open("data.txt", FILE_WRITE);
    if (myFile) {
    Serial.print("Writing to data.txt...");
    myFile.println(text);
    myFile.close();
    } 
    else {
    Serial.println("data.txt");
}
  }
}
