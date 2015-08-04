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

char timeServer[] = "time.nist.gov";    //This is the hub server for the nist time servers

const double sensorSlope = 1529.9; //The slope of the VOC sensor used
/*
-----END UNIQUE VALUES-----
*/

const int valve1 = 2; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve2 = 3; //The pin for the valve that switches between _____ (on) and _____ (off)
const int valve3 = 5; //The pin for the valve that switches between _____ (on) and _____ (off)

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

void configureValves(int configuration) {
  //1 is zero air
  //2 is finished air
  //3 is inner air
  //4 is room air

  Serial.print(F("Setting valves to configuration "));
  Serial.println(configuration);

  /***** The wrong valves are being turned on and off here. *****/
  switch(configuration) { //Switch statements work similar to if/elseif/elseif/... statements
    case 1:
      digitalWrite(valve1, HIGH);
      digitalWrite(valve2, HIGH);
      digitalWrite(valve3, HIGH);
      break;
    case 2:
      digitalWrite(valve1, LOW);
      digitalWrite(valve2, LOW);
      digitalWrite(valve3, LOW);
      break;
    case 3:
      digitalWrite(valve1, LOW);
      digitalWrite(valve2, HIGH);
      digitalWrite(valve3, LOW);
      break;
    case 4:
      digitalWrite(valve1, HIGH);
      digitalWrite(valve2, LOW);
      digitalWrite(valve3, HIGH);
      break;
    default:
      Serial.print(configuration);
      Serial.println(F(" is not a valid configuration for valves!"));
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
  digitalWrite(valve1, HIGH); //This turns off all the valves to start out
  digitalWrite(valve2, HIGH);
  digitalWrite(valve3, HIGH);
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
     Serial.println(F("SD card connected through pin 4."));
   }
   else {
     Serial.println(F("ERROR: Could not connect to SD card."));
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
   Serial.println(F("Attempting to connect to WiFi"));
   Serial.print(F("Network name: "));
   Serial.println(ssid);
   Serial.print(F("Password: "));
   Serial.println(pass);
   Serial.print(F("Network type: "));
   Serial.println(wifiType);
   while(status != WL_CONNECTED) { //WL_CONNECTED is the status given when the WiFi is connected and all is good.
                                   //The green LED on the WiFi shield lights up when it's connected.
     Serial.print(F("Not yet connected. Status: "));
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
   Serial.println(F("Wifi is connected."));

   Serial.println(F("Completed the setup function."));
   Serial.println();
   Serial.println();
}

void loop() { //Loop for all eternity
  /*
  -----Take the Measurements-----
  */
  Serial.println(F("Taking measurements."));
  double data[4] = {9001, 9001, 9001, 9001}; //9001 is an outlandish value, so if there's an error, it's obvious
  for(int i=1; i<=4; ++i) { //1 through 4 gets zero, finished, inner, and room
    configureValves(i);
    delay(1000 * systemPurge); //Clear the air out of the pipes and bring new air in to test
    data[i-1] = takeMeasurement(); //Take the measurement and put it into the data array
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
  Serial.println(F("Refining data."));
  for(int i=0; i<3; ++i) {
    nicerData[i] = data[i+1]-data[0]; //Subtract the zero offset from finished, inner, and room air
    nicerData[i] = nicerData[i] * 5 / 1023; //Convert to volts
    nicerData[i] = nicerData[i] * sensorSlope; //Convert to ppb
  }

  /*
  -----Get Timestamp-----
  */
  client.stop();                    //Reset the client so it can make a web request
  client.connect(timeServer, 13);   //Connect to time.nist.gov on port 13
  String timeStamp = "";
  while(timeStamp == "") {          //This ensures that the program waits until it's received a response before trying to record it
    while(client.available()) {     //While there's more text available,
      char c = client.read();       //Read it to char c
        if(c != 10) {               //If it's not a funny newline character
          Serial.write(c);          //Write it to the serial monitor
          timeStamp += c;           //And add it to the timeStamp string
        }
    }
  }
  Serial.println();
  Serial.println(timeStamp);

  /*
  -----Write Data, Timestamp, and Sensor Error to File-----
  */
  dataFile = SD.open("DATA.csv", FILE_WRITE); //Open DATA.csv, and get ready to write to the end of it
  for(int i=0; i<3; ++i) {        //Write all 3 items in nicerData[]
    dataFile.print(nicerData[i]); //println isn't used because this is just one line of data.
    dataFile.print(",");          //.csv stands for comma-separated-values.
    dataFile.flush();             //This saves the file

    Serial.print(nicerData[i]);
    Serial.print(F(","));
  }
  dataFile.print(timeStamp);      //Write the timestamp
  dataFile.print(",");
  dataFile.print(sensorError);    //Write the sensor error
  dataFile.flush();

  Serial.print(timeStamp);
  Serial.print(F(","));
  Serial.print(sensorError);

  dataFile.println(); //Finally finish the line of data
  dataFile.flush();
  dataFile.close();

  Serial.println(); //This really isn't needed, but it looks better on the serial monitor
}
