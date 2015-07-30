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
#include <Adafruit_CC3000.h>    //Used for WiFi connectivity
#include <ccspi.h>              // ^^^^^

//Pins used by the WiFi shield.
//It also uses 11, 12 and 13.
//And possibly maybe 4 and 7.
#define ADAFRUIT_CC3000_IRQ 3
#define ADAFRUIT_CC3000_VBAT 5
#define ADAFRUIT_CC3000_CS 10

//Create the instance of the Adafruit_CC3000 class to do all the web stuff
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, SPI_CLOCK_DIVIDER);

const char ssid[] = "RedSox2";              //This is the network name
const char pass[] = "fenway1999";           //and its password
#define WLAN_Security WLAN_SEC_WPA2         //and its security type

const char timeServer[] = "time.nist.gov";              //This is the hub server, so this doesn't need to be changed
const int timePort = 13;                                //Port 13 is used for the time server
const unsigned long dataServer = dotToNum(10, 0, 1, 8); //This is the numerical ip of the data server
const int dataPort = 80;                                //Port 80 is used for the data server

const int systemId = 12345;                 //The ID tag of the system
const String key = "super secret password"; //The password to get into the server

const int measureTime = 5;  //How long to average readings over
const int systemPurge = 5;  //How long to clear air for next reading

const int sensorPort = 0;   //The analog port the sensor is plugged in to

const int sensorMinValue = 0; //Given in volts
const int sensorMaxValue = 5; //These values are currently wrong

const double sensorSlope = 1529.9; //The slope of the current VOC sensor

const int valve1 = 1; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve2 = 2; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve3 = 4; //The pin for the valve that switches between _____ (on) and _____ (off)

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
    sum += analogRead(sensorPort);
    delay(1000);
  }
  return ((double)sum)/measureTime;
}
unsigned long dotToNum(unsigned long oct1, unsigned long oct2, unsigned long oct3, unsigned long oct4) {
  /*
   * Each of the four "octals" I have above are one of the four numbers in an IP address
   * I'm converting them to a single unsigned long variable
   */
  unsigned long sum = 0;
  sum += oct1 * 256 * 256 * 256;
  sum += oct2 * 256 * 256;
  sum += oct3 * 256;
  sum += oct4;
  return sum;
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

   /*
   ----- Set up the SD Card -----
   */
   if(!SD.begin(4)) { //If the SD card can't start on the Uno-standard pin 4...
     Serial.println("Error! Could not connect to SD card!");
     while(true);
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
  Serial.println("Connected to SD card.");

  /*
  ----- Set up WiFi -----
  */

  cc3000.begin(); //Start the module

  if(!cc3000.connectToAP(ssid, pass, WLAN_Security)) {
    //connectToAP will keep trying until it's connected
    Serial.println("Unable to connect to WiFi!");
    while(true);
  }
  Serial.println("Connected to WiFi!");

  while(!cc3000.checkDHCP()) {
    delay(100);
  }
  Serial.println("Set up DHCP.");
  Serial.println("Setup complete!");
  Serial.println();
  Serial.println();
}
void loop() { //Loop for all eternity
  /*
  ----- Take the Measurements -----
  */
  double data[4] = {9001, 9001, 9001, 9001}; //9001 is an outlandish value, so if there's an error, it's obvious
  for(int i=1; i<=4; ++i) { //1 through 4 gets zero, finished, inner, and room
    configureValves(i);
    delay(1000 * systemPurge); //Clear the air out of the pipes and bring new air in to test
    data[i-1] = takeMeasurement(); //Take the measurement and put it into the data array
    Serial.println(data[i-1]); //i-1 is used because arrays are 0-indexed and we're starting at 1
  }

  /*
  ----- Test for Sensor Error -----
  */
  boolean sensorError = false;
  for(int i=0; i<4; ++i) {
    if(data[i] > sensorMaxValue || data[i] < sensorMinValue) {
      sensorError = true; //If this data entry is larger than the max value or
                          //smaller than the minimum value, flag an error
    }
  }

  /*
  ----- Refine the Data -----
  */
  double nicerData[3] = {9001, 9001, 9001}; //It's over 9000!!!!!
  Serial.println("Refining data.");
  for(int i=0; i<3; ++i) {
    nicerData[i] = data[i+1]-data[0]; //Subtract the zero offset from finished, inner, and room air
    nicerData[i] = nicerData[i] * 5 / 1023; //Convert to volts
    nicerData[i] = nicerData[i] * sensorSlope; //Convert to ppb
    Serial.println(nicerData[i]);
  }

  /*
  ----- Get Timestamp -----
  */
  Serial.println("Getting timestamp.");
  unsigned long ip = 0; //This is how an IP truly is -- an unsigned long
  while(ip == 0) {
    cc3000.getHostByName(timeServer, &ip); //The & means by reference -- it'll update the ip variable
    delay(500); //Wait before trying again
  }
  cc3000.printIPdotsRev(ip); //Print out the IP in dot form
  Serial.println();
  Adafruit_CC3000_Client www = cc3000.connectTCP(ip, timePort); //This connects using TCP, the IP we found earlier, and port 13
  char timeStamp[50];
  int i = 0;
  while(www.connected()) {
    while(www.available()) {
      char c = www.read();
      Serial.print(c);
      timeStamp[i] = c;
      ++i;
    }
  }
  Serial.println();
  Serial.println(timeStamp);
  www.close();

  /*
  ----- Write Data, Timestamp, Sensor Error to File -----
  */
  dataFile = SD.open("DATA.csv", FILE_WRITE); //Open DATA.csv, and get ready to write to the end of it
  for(int i=0; i<3; ++i) {        //Write all 3 items in nicerData[]
    dataFile.print(nicerData[i]); //println isn't used because this is just one line of data.
    dataFile.print(",");          //.csv stands for comma-separated-values.
    dataFile.flush();             //This saves the file

    Serial.print(nicerData[i]);
    Serial.print(",");
  }
  dataFile.print(timeStamp);      //Write the timestamp
  dataFile.print(",");
  dataFile.print(sensorError);    //Write the sensor error
  dataFile.print(",");
  dataFile.flush();
}
