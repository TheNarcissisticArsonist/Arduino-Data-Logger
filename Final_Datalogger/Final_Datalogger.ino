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
/* WARNING: WRONG WIFI LIBRARY! */
/* Use this one instead: https://learn.adafruit.com/adafruit-cc3000-wifi/cc3000-library-software */

const int systemId = 12345;                 //The ID tag of the system
const String key = "super secret password"  //The password to get into the server

char ssid[] = "RedSox2";      //Network name
char pass[] = "fenway1999";   //Password
char key = "";                //Used for WEP
int keyIndex = 0;             //Used for WEP
const int wifiType = 1;
/*
 * Open Network: only ssid is needed. Set wifiType to 0.
 * WPA or WPA2 Personal: ssid and pass are needed. Set wifiType to 1.
 * WEP: ssid, key, and keyIndex are needed. Set wifiType to 2.
 * Other networks are incompatible.
 */

char timeServer[] = "time.nist.gov";  //The government time hub server
const int timePort = 13;              //Used to connect to time.nist.gov
IPAddress dataServer(64, 113, 32, 5); //The server to send data to
const int dataPort = 80;              //This is the normal port used

const int measureTime = 5;  //How long to average readings over
const int systemPurge = 5;  //How long to clear air for next reading

const int sensorPort = 0;   //The analog port the sensor is plugged in to

const int sensorMinValue = 0; //Given in volts
const int sensorMaxValue = 5; //These values are currently wrong

const double sensorSlope = 1529.9; //The slope of the current VOC sensor

const int valve1 = 1; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve2 = 2; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve3 = 4; //The pin for the valve that switches between _____ (on) and _____ (off)

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

void configureValves(int configuration) {
  //1 is zero air
  //2 is finished air
  //3 is inner air
  //4 is room air

  Serial.print("Setting valves to configuration ");
  Serial.println(configuration);

  /***** The wrong valves are being turned on and off here. *****/
  switch(configuration) { //Switch statements work similar to if/elseif/elseif/... statements
    case 1:
      digitalWrite(valve1, HIGH);
      digitalWrite(valve2, LOW);
      digitalWrite(valve3, HIGH);
      break;
    case 2:
      digitalWrite(valve1, HIGH);
      digitalWrite(valve2, LOW);
      digitalWrite(valve3, HIGH);
      break;
    case 3:
      digitalWrite(valve1, HIGH);
      digitalWrite(valve1, LOW);
      digitalWrite(valve1, HIGH);
      break;
    case 4:
      digitalWrite(valve1, HIGH);
      digitalWrite(valve1, LOW);
      digitalWrite(valve1, HIGH);
      break;
    default:
      Serial.print(configuration);
      Serial.println(" is not a valid configuration for valves!");
      while(true);
      break;
  }
}
double takeMeasurement() {
  int sum = 0;
  for(int i=0; i<measureTime; ++i) {
    sum += analogRead(sensor);
    delay(1000);
  }
  return ((double)sum)/measureTime;
}

void setup() {
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
}
