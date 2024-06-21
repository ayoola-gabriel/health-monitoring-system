#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include "spiffspath.h"
#include "delays.h"
#include <ESPmDNS.h>
#include "userconfig.h"
#include "spiffsreadwrite.h"
#include "settings_app.h"


// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void runWebServer(){
 String ap_ssid = readFile(SPIFFS, adminUserPath);
 String ap_password = readFile(SPIFFS, adminPassPath);

 if(ap_ssid == ""){
    ap_ssid = DEFAULT_AP_SSID;
 }

 if(ap_password == ""){
  ap_password = DEFUALT_AP_PASSWORD;
 }
 
 WiFi.softAP(ap_ssid.c_str(), ap_password.c_str());

  myDelay(100);

  if (!MDNS.begin("hms")) {
    Serial.println("Error setting up MDNS responder!");
  }
  Serial.println("mDNS responder started");

  MDNS.addService("http", "tcp", 80);

  myDelay(100);


  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send_P(200, "text/html", index_html);
  });

  server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(401);
  });

  server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", logout_html);
    Serial.println("ESP Restarting...");
    myDelay(5000);
    ESP.restart();
  });



  server.on("/admin", HTTP_GET, [](AsyncWebServerRequest *request) {
   
    if(request->hasParam("fullname")) writeFile(SPIFFS, fullNamePath, request->getParam("fullname")->value().c_str());
    if(request->hasParam("username")) writeFile(SPIFFS, adminUserPath, request->getParam("username")->value().c_str()); 
    if(request->hasParam("password")) {
      String r = request->getParam("password")->value();
      r.replace("hash","#");
      writeFile(SPIFFS, adminPassPath, r.c_str());
    }
    if(request->hasParam("ssid")) writeFile(SPIFFS, ssidPath, request->getParam("ssid")->value().c_str());
    if(request->hasParam("pass")) {
      String c = request->getParam("pass")->value();
      c.replace("hash","#");
      writeFile(SPIFFS, passPath, c.c_str());
    }
    if(request->hasParam("interval")) writeFile(SPIFFS, intervalPath, request->getParam("interval")->value().c_str());
    if(request->hasParam("sbp")) writeFile(SPIFFS, sbpPath, request->getParam("sbp")->value().c_str());
    if(request->hasParam("dbp")) writeFile(SPIFFS, dbpPath, request->getParam("dbp")->value().c_str());
    if(request->hasParam("sos1")) writeFile(SPIFFS, sos1Path, request->getParam("sos1")->value().c_str());
    if(request->hasParam("sos2")) writeFile(SPIFFS, sos2Path, request->getParam("sos2")->value().c_str());
    if(request->hasParam("net")) writeFile(SPIFFS, apnPath, request->getParam("net")->value().c_str());
    
    request->send_P(200, "text/plain", "Saved");
    Serial.println("Settings saved succesfully");
    writeFile(SPIFFS, firstStart, "true");
  });

  server.on("/load", HTTP_GET, [](AsyncWebServerRequest *request) {
    String fullname = readFile(SPIFFS, fullNamePath);
    request->send(200, "text/plain", fullname.c_str());
  });

  server.begin();
}