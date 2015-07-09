#include <Wire.h>

byte address = 0x27;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  //Tell the device to take a measuremnet
  Wire.beginTransmission(address);
  Wire.write((address << 1) & byte(B11111110));
  Wire.endTransmission();

  delay(100);

  //Get the measurement
  Wire.beginTransmission(address);
  Wire.write((address << 1) | byte(B00000001));
  Wire.endTransmission();

  if(Wire.available() >= 2) {
    unsigned int data = Wire.read();
    data = data << 8;
    data |= Wire.read();
    data = data << 8;
    data |= Wire.read();
    data = data << 8;
    data |= Wire.read();

    unsigned short status = (data && B11000000000000000000000000000000) >> 30;
    unsigned short humidityData = (data && B00111111111111110000000000000000) >> 16;
    unsigned short temperatureData = (data && B00000000000000001111111111111100) >> 2;
    Serial.println(status);
    Serial.println("Humidity");
    Serial.println((humidityData * 100)/(16382));
    Serial.println("Temperature");
    Serial.println(((temperatureData * 165)/(16382))-40);
  }
}
void loop() {
}
