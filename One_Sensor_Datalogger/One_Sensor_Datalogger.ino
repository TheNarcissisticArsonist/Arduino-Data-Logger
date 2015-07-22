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
-----THESE VALUES MAY NEED TO BE CHANGED ON EACH ARDUINO-----
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

//The following are given in seconds
const int measureTime = 5; //This is how long the system averages its measurements.
const int systemPurge = 5; //This is the amount of time needed after a valve
                           //configuration is changed before taking measurements.

const int sensor = 0; //The analog pin (has an A in front of it) that the sensor is plugged in to

const int sensorMinValue = 0;     //Minimum and maximum expected analog values for the sensor
const int sensorMaxValue = 1023;  //These vary by the individual sensor
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
   Serial.println("Wifi is connected.");

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
  /*
  -----Take the Measurements-----
  */
  Serial.println("Taking measurements.");
  double data[4] = {9001, 9001, 9001, 9001}; //9001 is an outlandish value, so if there's an error, it's obvious
  for(int i=1; i<=4; ++i) { //1 through 4 gets zero, finished, inner, and room
    configureValves(i);
    delay(1000 * systemPurge); //Clear the air out of the pipes and bring new air in to test
    data[i-1] = takeMeasurement(); //Take the measurement and put it into the data array
    Serial.println(data[i-1]); //i-1 is used because arrays are 0-indexed and we're starting at 1
  }

  /*
  -----Test for Sensor Error
  */
  boolean sensorError = false;
  for(int i=0; i<4; ++i) {
    if(data[i] > sensorMaxValue || data[i] < sensorMinValue) {
      sensorError = true; //If this data entry is larger than the max value or
                          //smaller than the minimum value, flag an error
    }
  }

  /*
  -----Refine the Data-----
  */
  double nicerData[3] = {9001, 9001, 9001}; //It's over 9000!!!!!
  Serial.println("Refining data.");
  for(int i=0; i<3; ++i) {
    nicerData[i] = data[i+1]-data[0]; //Subtract the zero offset from finished, inner, and room air
    Serial.println(nicerData[i]);
  }

  /*
  -----Get Timestamp-----
  */
  unsigned long timeStamp;
  Serial.println("Getting timestamp.");
  sendNTPpacket(timeServer);  //The function is located below the main loop.
                              //I don't fully understand how it works, so it's
                              //not commented. Just know that it grabs the time
                              //from a government server.
  delay(1000);
  if(Udp.parsePacket()) {
    Serial.println("Received packet.");
    Udp.read(packetBuffer, NTP_PACKET_SIZE); //This grabs the UDP data
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]); //These move the data into a
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  //single unsigned long.
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.print("Seconds since 1900: ");
    Serial.println(secsSince1900);
    unsigned long unixTime = secsSince1900 - 2208988800UL;
    Serial.print("Seconds since 1970 (unix time): ");
    Serial.println(unixTime);
    timeStamp = unixTime;
  }
  else {
    Serial.println("ERROR: Problem with UDP.");
    timeStamp = 0;
  }

  /*
  -----Write Data, Timestamp, and Sensor Error to File-----
  */
  dataFile = SD.open("DATA.csv", FILE_WRITE); //Open DATA.csv, and get ready to write to the end of it
  for(int i=0; i<3; ++i) {        //Write all 3 items in nicerData[]
    dataFile.print(nicerData[i]); //println isn't used because this is just one line of data.
    dataFile.print(",");          //.csv stands for comma-separated-values.
    dataFile.flush();             //This saves the file
  }
  dataFile.print(timeStamp);
  dataFile.print(",");
  dataFile.print(sensorError);
  dataFile.flush();
}
unsigned long sendNTPpacket(IPAddress& address) {
  //Serial.println("1");
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  //Serial.println("2");
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  //Serial.println("3");

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  //Serial.println("4");
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  //Serial.println("5");
  Udp.endPacket();
  //Serial.println("6");
}
