#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  SD.begin(4);
  myFile = SD.open("data.csv");
  while(myFile.available()) {
    Serial.write(myFile.read());
  }
}
void loop() {}
