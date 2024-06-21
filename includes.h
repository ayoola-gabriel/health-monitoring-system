#include <Adafruit_FONA.h>
#include <HardwareSerial.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <esp_sleep.h>
#include <Update.h>
#include <Firebase_ESP_Client.h>
#include <math.h>

#include "sources/oleddisplay.h"
#include "sources/server.h"
#include "sources/sim.h"

#include "sources/images.h"
#include "timestamp32bits.h"
#include "addons/TokenHelper.h"


#define RESET_PIN 27
#define MFIO_PIN 25
#define RAWDATA_BUFFLEN 250


#define chargePin 35
#define screenPin 33
#define buttonPin 26

#define FORMAT_SPIFFS_IF_FAILED true