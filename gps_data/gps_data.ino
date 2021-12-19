/*
Reads NMEA messages from Parallax GPS module and displays information to 16x2 LCD.
Raw NMEA messages also passed over USB serial.

LCD display layout:
 --------------------------------------------
 |UTC time            valid  num SV messages|
 |number of sattelites      CNo of top 3 SVs|
 --------------------------------------------

*/
 
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

#include "config.h"

char inByte;
String sentence = "";

SoftwareSerial mySerial(GPS_RX, GPS_TX); // GPS module one-line serial interface
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);


void parse_gpgga(String gpgga){  
  int c1 = gpgga.indexOf(','); // time
  int c2 = gpgga.indexOf(',', c1+1); // lat
  int c3 = gpgga.indexOf(',', c2+1); // lat dir
  int c4 = gpgga.indexOf(',', c3+1); // long
  int c5 = gpgga.indexOf(',', c4+1); // long dir
  int c6 = gpgga.indexOf(',', c5+1); // fix quality
  int c7 = gpgga.indexOf(',', c6+1); // number of satelites
  int c8 = gpgga.indexOf(',', c7+1); // HDOP
  int c9 = gpgga.indexOf(',', c8+1); // altitude
  int c10 = gpgga.indexOf(',', c9+1); // alt unit
  int c11 = gpgga.indexOf(',', c10+1); // height
  int c12 = gpgga.indexOf(',', c11+1); // height unit
  int c13 = gpgga.indexOf(',', c12+1); // time since last DGPS update
  int c14 = gpgga.indexOf(',', c13+1); // DGPS ref station id
  int c15 = gpgga.indexOf(',', c14+1); // checksum
  int c16 = gpgga.indexOf(',', c15+1); // checksum
  int c17 = gpgga.indexOf(',', c16+1); // checksum
  
  String Time = gpgga.substring(0, c1);
  String nSat = gpgga.substring(c6+1, c7);
  
  lcd.setCursor(0,0);
  lcd.print("UTC: " + Time);
  lcd.setCursor(0,1);
  lcd.print("nSat:" + nSat);
}

void parse_gpgsv(String gpgsv){
  int c1 = gpgsv.indexOf(','); // num messages
  int c2 = gpgsv.indexOf(',', c1+1); // message number
  int c3 = gpgsv.indexOf(',', c2+1); // num SVs in view
  int c4 = gpgsv.indexOf(',', c3+1); // SV1 PRN
  int c5 = gpgsv.indexOf(',', c4+1); // SV1 elevation in degrees
  int c6 = gpgsv.indexOf(',', c5+1); // SV1 azimuth in degrees
  int c7 = gpgsv.indexOf(',', c6+1); // SV1 CNo
  int c8 = gpgsv.indexOf(',', c7+1); // SV2 PRN
  int c9 = gpgsv.indexOf(',', c8+1); // SV2 elevation in degrees
  int c10 = gpgsv.indexOf(',', c9+1); // SV2 azimuth in degrees
  int c11 = gpgsv.indexOf(',', c10+1); // SV2 CNo
  int c12 = gpgsv.indexOf(',', c11+1); // SV3 PRN
  int c13 = gpgsv.indexOf(',', c12+1); // SV3 elevation in degrees
  int c14 = gpgsv.indexOf(',', c13+1); // SV3 azimuth in degrees
  int c15 = gpgsv.indexOf(',', c14+1); // SV3 CNo
  int c16 = gpgsv.indexOf(',', c15+1); // SV4 PRN
  int c17 = gpgsv.indexOf(',', c16+1); // SV4 elevation in degrees
  int c18 = gpgsv.indexOf(',', c17+1); // SV4 azimuth in degrees
  int c19 = gpgsv.indexOf(',', c18+1); // SV4 CNo

  int nMess = gpgsv.substring(0, c1).toInt();

  int CNo1 = gpgsv.substring(c6+1, c7).toInt();
  int CNo2 = gpgsv.substring(c10+1, c11).toInt();
  int CNo3 = gpgsv.substring(c14+1, c15).toInt();

  lcd.setCursor(14,0);
  lcd.print(nMess);
  
  if (nMess == 1) {
    lcd.setCursor(8,1);
    lcd.print(CNo1);
    lcd.setCursor(11,1);
    lcd.print("--");
    lcd.setCursor(14,1);
    lcd.print("--");
  }
  if (nMess == 2) {
    lcd.setCursor(8,1);
    lcd.print(CNo1);
    lcd.setCursor(11,1);
    lcd.print(CNo2);
    lcd.setCursor(14,1);
    lcd.print("--");
  }
  if (nMess >= 3) {
    lcd.setCursor(8,1);
    lcd.print(CNo1);
    lcd.setCursor(11,1);
    lcd.print(CNo2);
    lcd.setCursor(14,1);
    lcd.print(CNo3);
  }
}

void parse_gprmc(String gprmc){  
  int c1 = gprmc.indexOf(','); // time
  int c2 = gprmc.indexOf(',', c1+1); // valid
  int c3 = gprmc.indexOf(',', c2+1); // lat 
  int c4 = gprmc.indexOf(',', c3+1); // N/S
  int c5 = gprmc.indexOf(',', c4+1); // long 
  int c6 = gprmc.indexOf(',', c5+1); // E/W
  int c7 = gprmc.indexOf(',', c6+1); // speed in knots
  int c8 = gprmc.indexOf(',', c7+1); // course
  int c9 = gprmc.indexOf(',', c8+1); // date
  int c10 = gprmc.indexOf(',', c9+1); // variation
  int c11 = gprmc.indexOf(',', c10+1); // E/W
  int c12 = gprmc.indexOf(',', c11+1); // checksum
  
  String Valid = gprmc.substring(c1+1, c2);
  
  lcd.setCursor(12,0);
  lcd.print(Valid);
}
  

void parse_sentence(String sent) {
  if (sent.substring(0, 6) == "$GPGGA"){
    parse_gpgga(sent.substring(7));
  }
  else if (sent.substring(0, 6) == "$GPGSV"){
    parse_gpgsv(sent.substring(7));
  }
  else if (sent.substring(0, 6) == "$GPRMC"){
    parse_gprmc(sent.substring(7));
  }
}

void setup() {
  lcd.begin(LCD_COLS, LCD_ROWS);
  Serial.begin(BAUD);
  while (!Serial) {
    ;
  }
  mySerial.begin(BAUD);
}

void loop() { // run over and over
  if (mySerial.available()) { // read GPS
    inByte = mySerial.read();
    
    Serial.write(inByte);

    if (inByte == 0x24){ // $ start of message
      parse_sentence(sentence); // handle last sentence
      sentence = inByte;
    }
    else {
      sentence.concat(inByte);
    }
  }
}
