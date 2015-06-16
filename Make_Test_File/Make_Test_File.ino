#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  Serial.println("Started serial.");
  Serial.println("Starting SD card.");
  SD.begin(4);
  Serial.println("Opening/creating file.");
  myFile = SD.open("test.txt", FILE_WRITE);
  Serial.println("Writing to file.");
  myFile.println("Hello there!");
  Serial.println("Closing file.");
  myFile.close();
  Serial.println("Done.");
}

void loop() {}
