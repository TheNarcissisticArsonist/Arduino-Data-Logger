// comments starting with *** are questions in the code

#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const int valve1 = 1; //Valve one is the valve closest to the sensor array, inside of valves two and three
const int valve2 = 2; //Valve two is the valve that chooses between "Room Air" and "Zero Air"
const int valve3 = 3; //Valve three is the valve that chooses between "Inter-System Air" and "Outlet Air"

const int delayWhileChangingAir = 5; //This is in seconds

//***Should sensors have double values, or will it be something else?
double zeroValueSensor1; //CO
double zeroValueSensor2; //VOC
double zeroValueSensor3; //Humidity
double zeroValueSensor4; //Temperature

//These are used to find the average, by adding all the measurements and then dividing by 30.
int sumValuesSensor1;
int sumValuesSensor2;
int sumValuesSensor3;
int sumValuesSensor4;
double averageValue1;
double averageValue2;
double averageValue3;
double averageValue4;

//How long the system measures the sensors (at 1 Hz) before averaging
int measurementTime = 5;

//Analog ports and their respective sensors
int sensorCO = 1;
int sensorVOC = 2;
int sensorHumidity = 3;
int sensorTemperature = 4;

//Creating an instance of the file class, to be used later
File dataFile;

//Used later.
//See void loop() for more info
String dataString;

//Stuff used for the wifi connection
char ssid[] = "RedSox2";
char pass[] = "fenway1999";
int status = WL_IDLE_STATUS;
unsigned int localPort = 2390;
IPAddress timeServer(129, 6, 15, 30);
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[NTP_PACKET_SIZE];
WiFiUDP Udp;

void setup() {
  Serial.begin(9600);

  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);

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
  Serial.println("Attempting to connect to wifi.");
  while(status != WL_CONNECTED) {
    Serial.println("Not yet connected.");
    Serial.println(status);
    status = WiFi.begin(ssid, pass);
    delay(3000);
    Serial.println(status);
  }
  Serial.println("Connecting UDP.");
  Udp.begin(localPort);
  Serial.println("Completed void setup().");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}
void loop() {
  dataFile = SD.open("DATA.csv", FILE_WRITE);
  dataFile.print(takeMeasurement("zero"));
  dataFile.flush();
  dataFile.print(takeMeasurement("post finish filter"));
  dataFile.flush();
  dataFile.print(takeMeasurement("inner"));
  dataFile.flush();
  dataFile.print(takeMeasurement("room"));
  dataFile.flush();

  sendNTPpacket(timeServer);
  delay(1000);
  Serial.println(UDP.parsePacket());
  if(UDP.parsePacket()) {
    Serial.println("Packet received!");
    Udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    Serial.println(secsSince1900);
  }

  /*dataFile.print(dateString);
  dataFile.flush();*/
  dataFile.println("");
  dataFile.flush();
  dataFile.close();
}
boolean setValves(String setting) {
  //setting is used to determine the configuration
  //Valid options are shown in the conditional statements below
  if(setting == "zero") {
    Serial.println("Setting valves for zero air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
  else if(setting == "post finish filter") {
    Serial.println("Setting valves for post finish filter air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
  else if(setting == "inner") {
    Serial.println("Setting valves for inner air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
  else if(setting == "room") {
    Serial.println("Setting valves for room air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
  else {
    Serial.print("ERROR: ");
    Serial.print(setting);
    Serial.println(" is not a known valve configuration!");
  }
}
int takeSample(int sensor) {
  switch(sensor) {
    case 1: //CO
      return analogRead(sensorCO);
      break;
    case 2: //VOC
      return analogRead(sensorVOC);
      break;
    case 3: //Humidity
    return analogRead(sensorHumidity);
      break;
    case 4: //Temperature
    return analogRead(sensorTemperature);
      break;
    default:
      Serial.println("No sensor for that number!");
  }
}
String takeMeasurement(String setting) {
  String data = "";
  setValves(setting);
  //***Turn on vacuum pump?
  delay(1000 * delayWhileChangingAir); //1000 milliseconds * x seconds
  //***Turn off vacuum pump?
  sumValuesSensor1 = 0; //CO sensor
  sumValuesSensor2 = 0; //VOC sensor
  sumValuesSensor3 = 0; //Humidity sensor
  sumValuesSensor4 = 0; //Temperature sensor
  for(int i=0; i<measurementTime; i++) {
    sumValuesSensor1 += takeSample(1); //These get the total value
    sumValuesSensor2 += takeSample(2);
    sumValuesSensor3 += takeSample(3);
    sumValuesSensor4 += takeSample(4);
    delay(1000);
  }
  averageValue1 = ((double)sumValuesSensor1)/measurementTime; //And this divides it by 30 (it takes one sample per second, for 30 seconds)
  averageValue2 = ((double)sumValuesSensor2)/measurementTime;
  averageValue3 = ((double)sumValuesSensor3)/measurementTime;
  averageValue4 = ((double)sumValuesSensor4)/measurementTime;
  data += String(averageValue1);
  data += ",";
  data += String(averageValue2);
  data += ",";
  data += String(averageValue3);
  data += ",";
  data += String(averageValue4);
  data += ",";
  Serial.println(data);
  return data;
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
