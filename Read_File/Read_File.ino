#include <SD.h>

File myFile;

void setup() {
  Serial.begin(9600);
  SD.begin(4);
  myFile = SD.open("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  while(myFile.available()) {
    Serial.write(myFile.read());
  }
}
void loop() {}
