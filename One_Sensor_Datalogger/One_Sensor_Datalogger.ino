/*
 * Code written by Thomas Cohn.
 * Insert more personalized comments and such here...
 */

/*
 * Do not worry!
 * Comments are removed during compiling, and
 * therefore have no effect on memory usage.
 */

//Include statements
#include <SD.h>       //Used for the SD card
#include <SPI.h>      //Used by WiFi and the SD card -- they both use SPI to communicate
#include <WiFi.h>     //Used for WiFi connectivity
#include <WiFiUdp.h>  //Used for UDP -- the protocal to get date and time

const int systemId = 12345; //This is the system ID. It's used a lot on the server
const String key = "INSERT KEY HERE"; //This is the key used to access the database. Shhhh! It's a secret!

const int valve1 = 1; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve2 = 2; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve3 = 3; //The pin for the valve that switches between _____ (on) and _____ (off)

void setup() { //Run once at the beginning
  Serial.begin(9600); //The serial statement is used primarily for debugging.
                      //Throughout the code, you may see statements such as Serial.print and Serial.println
                      //These are simply messages being printed out to the computer connected to the Arduino

  pinMode(valve1, OUTPUT); //This sets the pins controlling
  pinMode(valve2, OUTPUT); //the various valves defined as
  pinMode(valve3, OUTPUT); //constants above.
  /*
   * Incidentally, these aren't actually directly
   * controlling the valves, but rather controlling
   * relays that control the valves. This is because
   * the valves require a higher voltage and current
   * than the arduino can supply.
   */


}
void loop() { //Loop for all eternity

}
