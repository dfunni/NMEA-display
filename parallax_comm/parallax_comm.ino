/*
Connects to Parallax GPS module to read raw data.
Ensure raw pin is pulled to ground.
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 10); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(4800);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  // set the data rate for the SoftwareSerial port
  mySerial.begin(4800);
}

void loop() { // run over and over
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
