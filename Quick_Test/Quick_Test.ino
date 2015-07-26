void setup() {
  Serial.begin(9600);
}
void loop() {
  int temp = analogRead(0);
  double temp2 = (double)temp * 5 / 1023;
  temp2 = temp2 * 1529.9;
  Serial.println(temp);
  delay(1000);
}
