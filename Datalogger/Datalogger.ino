#include <SD.h>
#include <SPI.h>

const int valve1 = 1; //Valve one is the valve closest to the sensor array, inside of valves two and three
const int valve2 = 2; //Valve two is the valve that chooses between "Room Air" and "Zero Air"
const int valve3 = 3; //Valve three is the valve that chooses between "Inter-System Air" and "Outlet Air"

void setup() {
  Serial.begin(9600);
  Serial.println("Serial connected");
  
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  Serial.println("Set pins controlling valves to output");
  
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
boolean setValves(String setting) {
  //setting is used to determine the configuration
  //Valid options are shown in the conditional statements below
  if(setting == "zero") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for zero air");
  }
  if(setting == "post finish filter") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for post finish filter air");
  }
  if(setting == "inner") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for inner air");
  }
  if(setting == "room") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for room air");
  }
}
