//Include statements
#include <SD.h>       //Used for the SD card
#include <SPI.h>      //Used by WiFi and the SD card -- they both use SPI to communicate
#include <WiFi.h>     //Used for WiFi connectivity
#include <WiFiUdp.h>  //Used for UDP -- the protocal to get date and time

const int systemId = 12345; //This is the system ID. It's used a lot on the server
const String key = "INSERT KEY HERE"; //This is the key used to access the database. Shhhh! It's a secret!

const int valve1 = 1; //The valve that switches between _____ (on) and _____ (off)
const int valve2 = 2; //The valve that switches between _____ (on) and _____ (off)
const int valve3 = 3; //The valve that switches between _____ (on) and _____ (off)

void setup() { //Run once at the beginning
  Serial.begin(9600); //The serial statement is used primarily for debugging.
                      //Throughout the code, you may see statements such as Serial.print and Serial.println
                      //These are simply messages being printed out to the computer connected to the Arduino
  
}
void loop() { //Loop for all eternity

}
