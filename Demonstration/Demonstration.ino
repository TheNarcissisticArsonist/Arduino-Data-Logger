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

/*
-----THESE VALUES MAY NEED TO BE CHANGED ON EACH ARDUINO-----
*/
const int systemId = 12345; //This is the system ID. It's used a lot on the server, and is unique to each sensor package
const String key = "INSERT KEY HERE"; //This is the key used to access the database. Shhhh! It's a secret!

char ssid[] = "RedSox2";    //This is my WiFi network name and password.
char pass[] = "fenway1999"; //These will need to be changed by location.
//char key[] = "";
//int keyIndex = 0;
/* --WiFi Instructions
 * Open network: Only SSID is needed. Set const int wifiType to 0
 * WPA or WPA2 personal network: SSID and password are needed. Set const int wifiType to 1
 * WEP networks: SSID, key, and key index are needed. Set const int wifiType to 2
 * All other networks are incompatible with Arduino.
 */
const int wifiType = 1;

//The following are given in seconds
const int measureTime = 5; //This is how long the system averages its measurements.
const int systemPurge = 5; //This is the amount of time needed after a valve
                           //configuration is changed before taking measurements.

const int sensor = 0; //The analog pin (has an A in front of it) that the sensor is plugged in to

const int sensorMinValue = 0;     //Minimum and maximum expected analog values for the sensor
const int sensorMaxValue = 1023;  //These vary by the individual sensor

IPAddress dataServer(67, 194, 11, 243); //This must be the numeric IP of the data server
char timeServer[] = "time.nist.gov";    //This is the hub server for the nist time servers

const double sensorSlope = 1529.9; //The slope of the VOC sensor used
/*
-----END UNIQUE VALUES-----
*/

const int valve1 = 1; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve2 = 2; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve3 = 3; //The pin for the valve that switches between _____ (on) and _____ (off)

int status = WL_IDLE_STATUS; //This variable is used to get the status of the Arduino's WiFi connection

double zeroOffset = 0;
/*
 * Craig gave me a great explanation for this. Basically, there's a slope that compares
 * some voltage value to a value on the sensor (in this case, ppb). However, no two
 * sensors are the same, and this leads to some problems. Due to their idiosyncrasies,
 * each sensor is slightly offset from that comparison slope by a changing number. This
 * value is grabbed from über-filtered air guarunteed to have none of whatever is being
 * sensed (VOCs), and then subtracted from further readings to remove the offset and
 * facilitate converting a 10 bit integer to a useable value.
 *
 * TLDR: We need to find what the actual zero value on the sensor is and base our
 * conversions off of that.
 */

File dataFile; //An instance of the file class, used to write the data to the SD card.

WiFiClient client; //An instance of the WiFiClient class, used to connect to the serverside file
String path = "/~Tommy/receive.php"; //The path to the file on the server.
