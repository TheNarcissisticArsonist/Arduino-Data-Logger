#include <SPI.h>
#include <SD.h>

char file[] = ""; //Put the file's name here

void setup() {
  Serial.begin(9600);
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println(SD.begin(4));
  SD.remove(file);
  Serial.println("Done.");
}
void loop() {}

//Now run listfiles to see if it worked. It probably did.
