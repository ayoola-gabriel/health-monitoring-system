#include <Adafruit_FONA.h>
#include <HardwareSerial.h>

#define FONA_RST 12
#define RXD2 32
#define TXD2 4

HardwareSerial SIM800ss(2);
Adafruit_FONA SIM800 = Adafruit_FONA(FONA_RST);

void beginSIM(){
  Serial.println("Setting up SIM module");
  SIM800ss.begin(4800, SERIAL_8N1, RXD2, TXD2);
  if (!SIM800.begin(SIM800ss)) {
    Serial.println("Couldn't find SIM800L");
    while (1)
      ;
  }
}

uint8_t getNetworkStatus(){
    uint8_t n = SIM800.getNetworkStatus();
    Serial.print(F("Network status "));
    Serial.print(n);
    Serial.print(F(": "));
    if (n == 0) Serial.println(F("Not registered"));
    if (n == 1) Serial.println(F("Registered (home)"));
    if (n == 2) Serial.println(F("Not registered (searching)"));
    if (n == 3) Serial.println(F("Denied"));
    if (n == 4) Serial.println(F("Unknown"));
    if (n == 5) Serial.println(F("Registered roaming"));

    return n;
}

bool enableNetworkTime(bool s){
  return (SIM800.enableNetworkTimeSync(s));
}

String readTime(){
  bool b = SIM800.enableNetworkTimeSync(true);
  char buffer[23];
  if(b){
  SIM800.getTime(buffer, 23);  // make sure replybuffer is at least 23 bytes!
  Serial.print(F("Time = ")); Serial.println(buffer);
  return String(buffer);
  } else{
    Serial.println("Time not Sync");
  }
}

uint16_t vbat;
uint16_t getBatt() {
  if (!SIM800.getBattPercent(&vbat)) {
    Serial.println(F("Failed to read Batt"));
  } else {
    Serial.print(F("VPct = "));
    Serial.print(vbat);
    Serial.println(F("%"));
  }
  return vbat;
}

uint16_t batLevel;
uint16_t getBattLevel(){
  if (!SIM800.getBattVoltage(&batLevel)) {
    Serial.println(F("Failed to read Batt Level"));
  } else {
    Serial.print(F("Battery Voltage = "));
    Serial.print(batLevel);
    Serial.println(F("V"));
  }
  return batLevel;
}

uint8_t getRSSI() {
  // read the RSSI
  uint8_t n = SIM800.getRSSI();
  uint8_t rssi;

  Serial.print(F("RSSI = "));
  Serial.print(n);
  Serial.print(": ");
  if (n == 0) rssi = -115;
  if (n == 1) rssi = -111;
  if (n == 31) rssi = -52;
  if ((n >= 2) && (n <= 30)) {
    rssi = map(n, 2, 30, -110, -54);
  }
  Serial.print(rssi);
  Serial.println(F(" dBm"));

  return rssi;
}

void sendMessage(char *phoneNumber, char *msg) {

  if (!SIM800.sendSMS(phoneNumber, msg)) {
          Serial.println(F("Failed"));
        } else {
          Serial.println(F("Sent!"));
        }
    // SIM800ss.println("AT+CMGF=1");
    // myDelay(500);
    // String str = "AT+CMGS=\"";
    // str += phoneNumber;
    // str += "\"";

    // Serial.println(str);
    // SIM800ss.println(str);
    // myDelay(500);
    // SIM800ss.print(msg);
    // myDelay(500);
    // SIM800ss.write(26);
    // myDelay(2000);
}

char url_string[] = "https://api.thingspeak.com/update?api_key=HYH2N80HOWTI5PF2";

void simToThingspeak(char *apn, char *req) {
  char buf1[50];
  char buf2[150];
  
  SIM800ss.println("AT+CGATT?");
  myDelay(500);
  SIM800ss.println("AT+CIPSHUT");
  myDelay(500);
  SIM800ss.println("AT+CIPSTATUS");
  myDelay(1000);
  SIM800ss.println("AT+CIPMUX=0");
  myDelay(1000);
  sprintf(buf1, "AT+CSTT=%s", apn);
  
  Serial.println(buf1);
  SIM800ss.println(buf1);

  myDelay(1000);
  SIM800ss.println("AT+CIICR");
  myDelay(3000);
  SIM800ss.println("AT+CIFSR");
  myDelay(2000);
  SIM800ss.println("AT+CIPSTART=0");
  myDelay(3000);
  SIM800ss.println("AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",\"80\"");
  myDelay(6000);
  SIM800ss.println("AT+CIPSEND");
  myDelay(4000);
  sprintf(buf2, "GET %s", req);
  
  Serial.println(buf2);
  SIM800ss.println(buf2);
  myDelay(4000);

  SIM800ss.println((char)26);
  myDelay(5000);

  SIM800ss.println("AT+CIPSHUT");
}


void simForceSleep(){
  bool n = SIM800.minimumPower();
  // return n;
}

void wakeUpSim(){
  bool n = SIM800.restorePower();
  // return n;
}


