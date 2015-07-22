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

/*
-----THESE VALUES MUST BE CHANGED FOR EACH ARDUINO-----
*/
const int systemId = 12345; //This is the system ID. It's used a lot on the server
const String key = "INSERT KEY HERE"; //This is the key used to access the database. Shhhh! It's a secret!

char ssid[] = "RedSox2";    //This is my WiFi network name and password.
char pass[] = "fenway1999"; //These will need to be changed by location.
//char key[] = "";
//int keyIndex = 0;
/* --WiFi Instructions
 * Open network: Only SSID is needed. Set const int wifiType to 0
 * WPA2 personal network: SSID and password are needed. Set const int wifiType to 1
 * WEP networks: SSID, key, and key index are needed. Set const int wifiType to 2
 * All other networks are incompatible with Arduino.
 */
const int wifiType = 1;

IPAddress timeServer(64, 113, 32, 5);
//This is a US Government time server.
//It's best to vary this. I'd pick one close by.
//List of servers: http://tf.nist.gov/tf-cgi/servers.cgi
/*
-----END UNIQUE VALUES-----
*/

const int valve1 = 1; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve2 = 2; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve3 = 3; //The pin for the valve that switches between _____ (on) and _____ (off)

int status = WL_IDLE_STATUS; //This variable is used to get the status of the Arduino's WiFi connection

WiFiUDP Udp; //This initializes a UDP client. It's used to get time from a US Government server
const int NTP_PACKET_SIZE = 48; //This is the size of a single NTP packet
const int localPort = 2390; //This is used by UDP for NTP.
byte packetBuffer[NTP_PACKET_SIZE]; //Another thing used by UDP for NTP.

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

   /*
   -----SD CARD-----
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

   /*
   -----WIFI-----
   */
   Serial.println("Attempting to connect to WiFi");
   Serial.print("Network name: ");
   Serial.println(ssid);
   Serial.print("Password: ");
   Serial.println(pass);
   Serial.print("Network type: ");
   Serial.println(wifiType);
   while(status != WL_CONNECTED) { //WL_CONNECTED is the status given when the WiFi is connected and all is good.
                                   //The green LED on the WiFi shield lights up when it's connected.
     Serial.print("Not yet connected. Status: ");
     Serial.println(status);

     switch(wifiType) {
       case 0: //Open network
        status = WiFi.begin(ssid);
        break;
      case 1: //WPA2 personal network
        status = WiFi.begin(ssid, pass);
        break;
      /*case 2: //WEP network
        status = WiFi.begin(ssid, keyIndex, key);
        break;*/
      //Keep that last one commented unless you're using WEP.
      //Compile errors... BEWARE
     }
     delay(3000); //Wait 3 seconds before trying again
   }

   /*
   -----UDP (Time)-----
   */
   Serial.println("Connecting UDP.");
   Udp.begin(localPort);
   Serial.println("Completed the setup function.");
   Serial.println();
   Serial.println();
}
void loop() { //Loop for all eternity

}
