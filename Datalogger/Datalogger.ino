#include <SD.h>
#include <SPI.h>

const int valve1 = 1; //Valve one is the valve closest to the sensor array, inside of valves two and three
const int valve2 = 2; //Valve two is the valve that chooses between "Room Air" and "Zero Air"
const int valve3 = 3; //Valve three is the valve that chooses between "Inter-System Air" and "Outlet Air"

const int delayBetweenMeasurements = 120; //This is in seconds

double zeroValueSensor1; //CO
double zeroValueSensor2; //VOC
double zeroValueSensor3; //Humidity
double zeroValueSensor4; //Temperature

void setup() {
  Serial.begin(9600);
  Serial.println("Serial connected");
  
  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  Serial.println("Set pins controlling valves to output");
  
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
}
void loop() {
  String dataString = ""; //The data is formatted and dumped into this string, and then dumped into the log file
  setValves("zero");
  delay(1000 * delayBetweenMeasurements); //1000 milliseconds * x seconds
  double sumValuesSensor1 = 0; //CO sensor
  double sumValuesSensor2 = 0; //VOC sensor
  double sumValuesSensor3 = 0; //Humidity sensor
  double sumValuesSensor3 = 0; //Temperature sensor
  for(int i=0; i<30; i++) {
    sumValuesSensor1 += takeSample1(); //These get the total value
    sumValuesSensor2 += takeSample2();
    sumValuesSensor3 += takeSample3();
    sumValuesSensor4 += takeSample4();
    delay(1000);
  }
  double averageValue1 = sumValuesSensor1/30; //And this divides it by 30 (it takes one sample per second, for 30 seconds)
  double averageValue2 = sumValuesSensor2/30;
  double averageValue3 = sumValuesSensor3/30;
  double averageValue4 = sumValuesSensor4/30;
  zeroValueSensor1 = averageValue1; //And this sets the zero value to the average value we just found (this is zero air)
  zeroValueSensor2 = averageValue2;
  zeroValueSensor3 = averageValue3;
  zeroValueSensor4 = averageValue4;
}
boolean setValves(String setting) {
  //setting is used to determine the configuration
  //Valid options are shown in the conditional statements below
  if(setting == "zero") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for zero air");
  }
  if(setting == "post finish filter") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for post finish filter air");
  }
  if(setting == "inner") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for inner air");
  }
  if(setting == "room") {
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
    Serial.println("Set valves for room air");
  }
}
double takeSample1() { //CO
  
}
double takeSample2() { //VOC
  
}
double takeSample3() { //Humidity
  
}
double takeSample4() { //Temperature
  
}
