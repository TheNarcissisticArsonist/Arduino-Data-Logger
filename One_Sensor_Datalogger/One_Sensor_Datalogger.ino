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

  Serial.println(); //The serial monitor often contains some garbled messages
  Serial.println(); //upon starting up. This simply prints a few lines so the
  Serial.println(); //programmer knows when the actual information is coming.

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

   if(SD.begin(4)) { //This starts up Arduino communication with the SD card on pin 4.
                     //Pin 4 is standard for the Arduino Uno Rev 3
                     //If successful, it returns true, allowing this conditional to work
     Serial.println("SD card connected through pin 4.");
   }
   else {
     Serial.println("ERROR: Could not connect to SD card.");
     while(true); //This effectively stops the Arduino
     /*
      * This is an unusual piece of code to find in most languages.
      * However, an Arduino can never truly "stop" -- it must always.
      * be doing something. Stopping a program is important if there's
      * a problem, as this prevents anything further from breaking and
      * gives the programmer a chance to fix the problem.
      *
      * This is the closest way you can come to stopping an Arduino
      * without literally unplugging it.
      */
   }
}
void loop() { //Loop for all eternity

}
