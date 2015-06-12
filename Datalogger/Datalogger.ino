#include <SD.h>
#include <SPI.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Serial connected");
  if(SD.begin(4)) {
    Serial.println("SD card connected through pin 4");
  }
  else {
    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("ERROR: SD card could not connect via pin 4.");
    while(true);
  }
}
void loop() {
  
}
