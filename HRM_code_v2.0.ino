#include "includes.h"
#include "max32664.h"
#include <ESP32Time.h>

#define MAX_SBP 160
#define MIN_SBP 90
#define MAX_DBP 100
#define MIN_DBP 50
#define MAX_HR 150
#define MIN_HR 40

uint16_t batteryVoltage;
bool msgFlag = false;
bool bpFlag = false;
bool onDisplay = false;

WiFiClient client;
max32664 MAX32664(RESET_PIN, MFIO_PIN, RAWDATA_BUFFLEN);

const char *host = "api.thingspeak.com";       // This should not be changed
const int httpPort = 80;                       // This should not be changed
const String channelID = "1636011";            // Change this to your channel ID
const String writeApiKey = "HYH2N80HOWTI5PF2"; // Change this to your Write API key
const String readApiKey = "7ZZUY4TB3V9DJ0QS";  // Change this to your Read API key

String footer = String(" HTTP/1.1\r\n") + "Host: " + String(host) + "\r\n" + "Connection: close\r\n\r\n";

ESP32Time rtc(3600);
bool sensorReady = false;
uint8_t interval;
uint32_t counter[8];
uint16_t initialLevel = 0;
unsigned long previousMillis = 0;
int sbp, dbp, hrate, temp, realTemp = 0;
bool wifiConnected = false;
bool simSleepFlag = false;
bool state_info = true;
String ssid, pass_w;

void mfioInterruptHndlr()
{
  // Serial.println("i");
}

void enableInterruptPin()
{

  // pinMode(mfioPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(MAX32664.mfioPin), mfioInterruptHndlr, FALLING);
}

bool buttonPressed = false;
void IRAM_ATTR buttonISR(){
  buttonPressed = true;
}

void setup()
{
  Serial.begin(115200);

  beginDisplay();

  beginSPIFFS(FORMAT_SPIFFS_IF_FAILED);

  bool enterSetUp = preBoot(true);
  if (enterSetUp)
  {
    displaySetUp();
    showScreen();
    runWebServer();
    while (true)
      ;
  }

  displayWelcome();
  showScreen();
  drawBaseLine(1);
  myDelay(100);
  drawBaseLine(10);

  beginSIM();
  myDelay(10000);
  drawBaseLine(30);

  String s = readFile(SPIFFS, intervalPath);
  if (s == "")
  {
    interval = 1;
  } else interval = s.toInt();

  Serial.printf("Measurement Interval: %d\n", interval);

  drawBaseLine(50);

  if (getNetworkStatus() != 1)
  {
    String t = readTime();
    Serial.print(t);
    // set ESP RTC Time
    rtc.setTime(30, 24, 15, 17, 1, 2021); // 17th Jan 2021 15:24:30
    simForceSleep();
    simSleepFlag = true;
  }
  else
  {
    simSleepFlag = false;
  }
  drawBaseLine(60);

  loadAlgomodeParameters();
  drawBaseLine(70);

  // sensorReady = setupSensor();
  drawBaseLine(110);
  ssid = readFile(SPIFFS, ssidPath);
  pass_w = readFile(SPIFFS, passPath);

  uint8_t num_samples = MAX32664.readSamples();
  drawBaseLine(120);
  
  drawBaseLine(128);
  attachInterrupt(digitalPinToInterrupt(buttonPin), buttonISR, FALLING);
  onDisplay = true;
  counter[5] = 0;
  clearScreen();
  drawInfo("HMS");
  drawBatteryLevel(batteryVoltage);
  drawWifi(wifiConnected, 110, 18);
}


void loop()
{
   // show screen displays and also try uploading...
  if(millis()-previousMillis>5000){
    previousMillis = millis();
    batteryVoltage = getBatt();
    drawBatteryLevel(batteryVoltage);
    drawWifi(wifiConnected, 110, 18);
    drawBP(sbp, dbp);
    drawHR(hrate, realTemp);


    if(batteryVoltage>90){
      data_to_upload();
    }
  }

  if(!state_info){
    drawInfo("HMS");
  }

//Do checkups every 10seconds
if((millis() - counter[7])>5000){
  counter[7] = millis();

//Check if SIM is connected to network
  if (!simSleepFlag)
  {
    if (getNetworkStatus() == 1)
    {
      String t = readTime();
      Serial.print(t);
      // set ESP RTC Time
      rtc.setTime(30, 24, 15, 17, 1, 2021); // 17th Jan 2021 15:24:30
      simForceSleep();
      simSleepFlag = true;
    }
    else
    {
      simSleepFlag = false;
    }
  }

//check if sensor is still connected
  if(!MAX32664.readSensorHubVersion()){
    sensorReady = false;
    bpFlag = false;
    state_info = drawInfo("NO SENSOR");
    counter[2] = interval + 1;
  }

//check for charging
  if(charging()){

  }

  state_info = false;
}

// take sensor Measurement
  if (sensorReady && takeMeasurement(interval))
  {
    drawHeart(true,110,39);
    while(sbp==0 && !bpFlag){
      uint8_t num_samples = MAX32664.readSamples();
    if (num_samples){
      sbp = MAX32664.max32664Output.sys;
    }
      Serial.println("Loading BP Values...");
      myDelay(2000);
    }
    bpFlag = true;
    uint8_t num_samples = MAX32664.readSamples();
    if (num_samples)
    {
      sbp = MAX32664.max32664Output.sys;
      dbp = MAX32664.max32664Output.dia;
      hrate = MAX32664.max32664Output.hr;
      temp = getTemp();
      (temp > 25 || temp < 45)? realTemp = temp : realTemp = realTemp;
      Serial.printf("Blood Pressure:%d/%dmmHg, HR:%dbpm, Temp:%.2f\n\n", sbp, dbp, hrate, realTemp);
      
    }

    if (sbp != 0 && dbp != 0)
    {

      // wifiConnected = connectWiFi();
      int dataIndex = readFile(SPIFFS, indexPath).toInt();

      String buff = makeReq() + "@";

      if (dataIndex == 0)
      {
        writeFile(SPIFFS, dataPath, buff.c_str());
      }
      else
      {
        appendFile(SPIFFS, dataPath, buff.c_str());
      }
      dataIndex++;
      char indexBuf[6];
      sprintf(indexBuf, "%d", dataIndex);
      Serial.printf("Writing Data at index: %s\r\n", indexBuf);
      // Serial.printf("Data: %s", buf);
      writeFile(SPIFFS, indexPath, indexBuf);
    } drawHeart(false,110,39);
  }
  else if (!sensorReady)
  {
    // try reintializing Sensor
    sensorReady = setupSensor();
    if (!sensorReady)
    {
      // send No Sensor Found to info
      state_info = drawInfo("NO SENSOR"); 
    }
  }

  // Send SMS if vitals are critical, sensor is ready,
  //message has not been sent before
  if (criticalState(sbp, dbp) && sensorReady && !msgFlag && counter[5]!=0)
  {
    char msg[] = "Blood Pressure or Heart rate is critical. Please confirm the user's state of heart";
    char phone_Number[15];
    readFile(SPIFFS, sos1Path).toCharArray(phone_Number, 15);
    Serial.println("Activate SMS routine");
    if (simSleepFlag)
    {
      beginSIM();
      myDelay(10000);
      int8_t timeout = 15;
      while (getNetworkStatus() != 1)
      {
        timeout--;
        if (timeout <= 0)
          break;
        myDelay(1000);
      }
    }
    sendMessage(phone_Number, msg);
    state_info = drawInfo("CRITICAL");
    msgFlag = true;
  }

  //clear msgFlag
  if(msgFlag) {
    counter[6]++;
    if(counter[6]>1000){
      counter[6] = 0;
      msgFlag = false;
    }
  }

   counter[5] = 1;

//Button press functions
  if(buttonPressed){
    uint32_t currentMillis = millis();

    onDisplay ? onDisplay = false: onDisplay = true;
    displayON(onDisplay);
    Serial.printf("Button Pressed. Display: %d\n", onDisplay);
    myDelay(2000);
    while(!digitalRead(buttonPin)){
      detachInterrupt(buttonPin);
      if(millis()-currentMillis>8000){
        //Reset Device Sequence Activated
        Serial.println("Reset Device Sequence Activated");
        displayON(true);
        myDelay(100);
        formatScreen();
        formatSPIFFS();
        ESP.restart();
      }
    } buttonPressed = false;
  }

}

bool getNetworkstate()
{
  bool networkStatus = getNetworkStatus();

  if (networkStatus)
  {
    bool n = enableNetworkTime(true);
    if (n)
    {
      String t = readTime();
      Serial.print(t);
      // set ESP RTC Time
      rtc.setTime(30, 24, 15, 17, 1, 2021); // 17th Jan 2021 15:24:30
    }
  }
  return networkStatus;
}

uint8_t getTemp()
{
  MAX32664.writeMax301xxReg(0x21, 0x01);
  unsigned long start_t = millis();
  double tmp = 0.00;
  int temp = 0;
  bool isValid = false;

  while (millis() - start_t < 100)
  {
    uint8_t response = MAX32664.readMax301xxReg(0x01);
    if ((response & 0x02) > 0)
      break;
  }
  int8_t tempInt = MAX32664.readMax301xxReg(0x1F);
  uint8_t tempFrac = MAX32664.readMax301xxReg(0x20);

  tmp = (double)tempInt + ((double)tempFrac * 0.0625);
  temp = int(tmp);

    Serial.print("Temperature: ");
    Serial.println(tmp);
    return temp;
}

bool preBoot(bool state)
{
  bool setup = false;
  pinMode(RESET_PIN, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  // pinMode(batteryPin, INPUT);

  myDelay(1000);
  String firstTimeBootCheck = readFile(SPIFFS, firstStart);
  if(firstTimeBootCheck == ""){
    return true;
  }

  if (state)
  {
    myDelay(2000);
    if (!digitalRead(buttonPin))
    {
      setup = true;
      Serial.println("Entering Setup Mode");
    }
  }

  if (!setup)
  {
    Serial.println("Device is powered on successfully");
    // sleep_en = false;
  }
  myDelay(1000);
  return setup;
}

void loadAlgomodeParameters()
{

  algomodeInitialiser algoParameters;
  /*  Replace the predefined values with the calibration values taken with a reference spo2 device in a controlled environt.
      Please have a look here for more information, https://pdfserv.maximintegrated.com/en/an/an6921-measuring-blood-pressure-MAX32664D.pdf
      https://github.com/Protocentral/protocentral-pulse-express/blob/master/docs/SpO2-Measurement-Maxim-MAX32664-Sensor-Hub.pdf
  */

  // recover values from SPIFFS

  int loadedSys = readFile(SPIFFS, sbpPath).toInt();
  int loadedDia = readFile(SPIFFS, dbpPath).toInt();

  algoParameters.calibValSys[0] = loadedSys + 1;
  algoParameters.calibValSys[1] = loadedSys;
  algoParameters.calibValSys[2] = loadedSys + 1;

  algoParameters.calibValDia[0] = loadedDia;
  algoParameters.calibValDia[1] = loadedDia;
  algoParameters.calibValDia[2] = loadedDia;

  algoParameters.spo2CalibCoefA = 1.5958422;
  algoParameters.spo2CalibCoefB = -34.659664;
  algoParameters.spo2CalibCoefC = 112.68987;

  MAX32664.loadAlgorithmParameters(&algoParameters);
}

bool setupSensor()
{
  bool ready = false;
  if (MAX32664.hubBegin() == CMD_SUCCESS)
  {
    Serial.println("Sensorhub begin!");
    bool ret = MAX32664.startBPTcalibration();
    while (!ret)
    {
      // myDelay(10000);
      Serial.println("Calibration Failed");
      ready = false;
      break;
      // ret = MAX32664.startBPTcalibration();
    }

    myDelay(1000);

    // Serial.println("start in estimation mode");
    if (ret == true)
    {
      ret = MAX32664.configAlgoInEstimationMode();
      int cnt = 0;
      while (!ret)
      {
        Serial.println("failed est mode");
        ret = MAX32664.configAlgoInEstimationMode();
        myDelay(10000);
        ready = false;
        cnt++;
        if (cnt > 2)
          break;
      }
    }

    if (ret)
    {
      ready = true;
      // MAX32664.enableInterruptPin();
      // Only if sensor is ready
      Serial.println("Device is ready..");
      myDelay(1000);
    }
  }
  else
  {
    Serial.println("Could not communicate with the sensor! please make proper connections");
    ready = false;
  }
  return ready;
}

bool takeMeasurement(int c)
{
  bool state = false;
  if (millis() - counter[1] > 60000)
  {
    counter[2] = counter[2] + 1;
    counter[1] = millis();
  }
  if (counter[2] >= c)
  {
    state = true;
    counter[2] = 0;
  }
  return state;
}

bool connectWiFi(String ssid_, String pass_)
{
  bool state = false;
  WiFi.mode(WIFI_STA);

  Serial.printf("\r\n[Wifi]: Connecting");
  WiFi.begin(ssid_.c_str(), pass_.c_str());
  int ii = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (ii == 50)
    {
      Serial.println("Unable to Connect to Network");
      break;
    }
    Serial.printf(".");
    delay(100);
    ii++;
    // digitalWrite(wifiLed,~digitalRead(wifiLed));
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
    state = true;
  }
  return state;
}

String makeReq()
{
  String str = "&field1="; // time
  str += sbp;
  str += "&field2="; // sbp
  str += dbp;
  str += "&field3="; // dbp
  str += hrate;
  str += "&field4="; // hr
  str += realTemp;
  str += "&field5="; // ppg
  str += rtc.getTime();

  return str;
}

bool uploadInfo(String n)
{
  if (!client.connect(host, httpPort))
  {
    Serial.println("Unable to Connect to Server");
    return false;
  }

  Serial.print("GET /update?api_key=" + writeApiKey + n);
  client.print("GET /update?api_key=" + writeApiKey + n + footer);
  readResponse(&client);

  return true;
}

void readResponse(WiFiClient *client)
{
  unsigned long timeout = millis();
  while (client->available() == 0)
  {
    if (millis() - timeout > 5000)
    {
      Serial.println(">>> Client Timeout !");
      client->stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client->available())
  {
    String line = client->readStringUntil('\r');
    // Serial.print(line);
  }

  Serial.print("Closing connection\n\n");
}

bool criticalState(int sys, int dia)
{
  bool state = false;

  if (sys == 0 || dia == 0)
  {
    return false;
  }
  else
  {

    (sys > MAX_SBP || sys < MIN_SBP) ? state = true : state = false;
    (dia > MAX_DBP || dia < MIN_DBP) ? state = true : state = false;
    // (rate > MAX_HR || rate < MIN_HR) ? state = true : state = false;
    return state;
  }
}

bool charging()
{
  bool state = false;
  if (counter[4] == 0)
  {
    initialLevel = getBattLevel();
  }
  counter[4]++;
  if (counter[4] >= 5)
  {
    counter[4] = 0;
    uint8_t finalLevel = getBattLevel();
    if (finalLevel > initialLevel)
    {
      state = true;
      drawInfo("CHARGING");
    }
  }
  if (batteryVoltage >= 97)
    {
      drawInfo("BATTERY FULL");
    }
  return state;
}

void data_to_upload(){
  wifiConnected = connectWiFi(ssid, pass_w);
  drawWifi(wifiConnected,110,18);
  if(wifiConnected){
    int dataIndex = readFile(SPIFFS, indexPath).toInt();
      if (dataIndex == 0)
      {
        Serial.println("No data to upload");
      }
      else {
        drawInfo("UPLOADING...");
          String data_ = readFile(SPIFFS, dataPath);
          for (int j = 0; j < dataIndex; j++)
          {
            // Serial.print("Raw Data:");
            // Serial.println(data_);
            Serial.printf("Index size: %d of %d\n",j, dataIndex);
            int n = data_.indexOf("@");
            String dat_ = data_.substring(0, n);
            //Serial.printf("Extracted Data: %s\n",dat_);
            bool c = uploadInfo(dat_);
            if (!c)
            {
              c = uploadInfo(dat_);
            }
            data_.remove(0, n + 1);
            // Serial.print("Remaining Data: ");
            // Serial.println(data_);
            myDelay(20000);
          }
          drawInfo("UPLOADED");
          data_ = String();
          dataIndex = 0;
          deleteFile(SPIFFS, dataPath);
          writeFile(SPIFFS, indexPath, "0");
          myDelay(1000);
        }
      }
          WiFi.disconnect(true);
          WiFi.mode(WIFI_OFF);
          myDelay(1000);
}