// comments starting with *** are questions in the code

#include <SD.h>
#include <SPI.h>

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

void setup() {
  Serial.begin(9600);
  Serial.println("Serial connected.");

  pinMode(valve1, OUTPUT);
  pinMode(valve2, OUTPUT);
  pinMode(valve3, OUTPUT);
  Serial.println("Set up pins controlling valves.");

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
  Serial.println("Completed void setup().");
  Serial.println("");
  Serial.println("");
  Serial.println("");
}
void loop() {
  String dataString = ""; //The data is formatted and dumped into this string, and then dumped into the log file
  /*
  The formatting for dataString will be for a .csv file:
  zeroAirCO, zeroAirVOC, zeroAirHumidity, zeroAirTemperature...
  Followed by post finish filter air
  Followed by inner air
  Followed by room air
  Followed lastly, by date and time information
  */
  dataString += takeMeasurement("zero");

  zeroValueSensor1 = averageValue1; //And this sets the zero value to the average value we just found (this is zero air)
  zeroValueSensor2 = averageValue2;
  zeroValueSensor3 = averageValue3;
  zeroValueSensor4 = averageValue4;

  //***Calibrate?

  dataString += takeMeasurement("post finish filter");
  dataString += takeMeasurement("inner");
  dataString += takeMeasurement("room");

  Serial.println("");
  Serial.println("Final dataString:");
  Serial.println(dataString);
  Serial.println("");
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
  if(setting == "post finish filter") {
  Serial.println("Setting valves for post finish filter air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
  if(setting == "inner") {
  Serial.println("Setting valves for inner air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
  if(setting == "room") {
  Serial.println("Setting valves for room air.");
    /*digitalWrite(valve1, );
    digitalWrite(valve2, );
    digitalWrite(valve3, );*/
  }
}
int takeSample1() { //CO

}
int takeSample2() { //VOC

}
int takeSample3() { //Humidity

}
int takeSample4() { //Temperature

}
String takeMeasurement(String setting) {
  String data = "";
  Serial.print("Measuring the ");
  Serial.print(setting);
  Serial.println(" air.");
  setValves(setting);
  Serial.print("Set valves for ");
  Serial.print(setting);
  Serial.println(" air.");
  Serial.println("");
  Serial.println("Please wait the number of seconds specified by delayWhileChangingAir.");
  //***Turn on vacuum pump?
  delay(1000 * delayWhileChangingAir); //1000 milliseconds * x seconds
  //***Turn off vacuum pump?
  Serial.println("Air changed, starting to take measurements.");
  sumValuesSensor1 = 0; //CO sensor
  sumValuesSensor2 = 0; //VOC sensor
  sumValuesSensor3 = 0; //Humidity sensor
  sumValuesSensor3 = 0; //Temperature sensor
  Serial.println("");
  for(int i=0; i<measurementTime; i++) {
    sumValuesSensor1 += takeSample1(); //These get the total value
    sumValuesSensor2 += takeSample2();
    sumValuesSensor3 += takeSample3();
    sumValuesSensor4 += takeSample4();
    Serial.print("Took measurement #");
    Serial.println(i+1);
    delay(1000);
  }
  Serial.println("");
  Serial.println("Finished getting values. Averaging...");
  averageValue1 = ((double)sumValuesSensor1)/measurementTime; //And this divides it by 30 (it takes one sample per second, for 30 seconds)
  averageValue2 = ((double)sumValuesSensor2)/measurementTime;
  averageValue3 = ((double)sumValuesSensor3)/measurementTime;
  averageValue4 = ((double)sumValuesSensor4)/measurementTime;
  Serial.println("Putting data into dataString");
  data += "averageValue1";
  data += ",";
  data += "averageValue2";
  data += ",";
  data += "averageValue3";
  data += ",";
  data += "averageValue4";
  data += ",";
  Serial.println("Finished putting data into dataString.");
  Serial.println(data);
  Serial.println("");
  return data;
}
