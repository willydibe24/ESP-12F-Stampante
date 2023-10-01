#include <Servo.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "credentials.hpp"


unsigned long servoPrevTime = 0;
bool servoTrigger = false; // when /trigger route is called, the variable switches to true for 500ms
int servoValue = 90;       // default value is set to 90 deg

ESP8266WebServer server(80);
Preferences prefs;
Servo servo;



void setup()
{
  Serial.begin(115200);

  // ---------------------------------
  // WIFI CONFIG
  // ---------------------------------

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connessione in corso...");
  }

  IPAddress localIP = WiFi.localIP();
  String ipAddress = localIP.toString();
  Serial.println("WI-FI connesso!");
  Serial.print("Indirizzo IP ESP-12F: ");
  Serial.println(ipAddress);

  // END WIFI CONFIG

  // ---------------------------------
  // HTTP SERVER CONFIG
  // ---------------------------------

  server.enableCORS(true);
  server.on("/", HTTP_GET, serverRoot);
  server.on("/trigger", HTTP_POST, serverTrigger);
  server.on("/set-trigger-value", HTTP_POST, serverSetTriggerValue);
  server.on("/info", HTTP_GET, serverInfo);
  server.on("/reset-values", HTTP_POST, serverResetValues);
  server.begin();

  // END HTTP SERVER CONFIG

  // ---------------------------------
  // PREFERENCES (LOCALSTORAGE) CONFIG
  // ---------------------------------

  prefs.begin("smart-print");

  int resets = prefs.getInt("resets", 0);
  prefs.putInt("resets", resets + 1);
  
  bool printerStatus = prefs.getBool("printerStatus", false);
  prefs.putBool("printerStatus", false);

  // END PREFERENCES (LOCALSTORAGE) CONFIG

  servo.attach(2);
}



void loop()
{
  server.handleClient();
  unsigned long currentTime = millis();


  if (servoTrigger)
  {
    if (currentTime - servoPrevTime >= 1200)
    {
      if (servoValue == 90)
      {
        int servoTriggerValue = prefs.getInt("servoTriggerValue", 35);
        servoValue = servoTriggerValue;
        incrementServoCounter();
        setPrinterStatus();
      }
      else
      {
        servoValue = 90;
        servoTrigger = false;
      }

      servo.write(servoValue);
      servoPrevTime = currentTime;
    }
  }
}



void serverRoot()
{
  server.send(200, "text/plain", "Hello from ESP-12F!");
}



void serverTrigger()
{
  servoTrigger = true;
  server.send(200, "text/plain", "TRIGGER OK.");
}



void serverSetTriggerValue()
{
  if(!server.arg("value"))
    return server.send(400, "text/plan", "Missing \"value\" parameter.");

  int value = atoi(server.arg("value").c_str());
  if(value == 0)
    return server.send(400, "text/plain", "Bad parameter.");

  prefs.putInt("servoTriggerValue", value);
  String message = "VALUE OK. New value is " + server.arg("value") + ".";

  server.send(200, "text/plain", message);
}



void serverInfo()
{
  DynamicJsonDocument JsonDocument(200);
  JsonDocument["ip"] = WiFi.localIP().toString();
  JsonDocument["counter"] = prefs.getInt("counter", 0);
  JsonDocument["resets"] = prefs.getInt("resets", 0);
  JsonDocument["servoValue"] = servo.read();
  JsonDocument["servoTriggerValue"] = prefs.getInt("servoTriggerValue", 35);
  JsonDocument["printerStatus"] = prefs.getBool("printerStatus", false);

  String jsonString;
  serializeJson(JsonDocument, jsonString);
  server.send(200, "application/json", jsonString);
}



void serverResetValues() 
{
  int prevValue = prefs.getInt("servoTriggerValue", 35);
  prefs.clear();
  prefs.putInt("servoTriggerValue", prevValue);
  server.send(200, "text/plain", "RESET VALUES OK.");
}



void incrementServoCounter()
{
  int counter = prefs.getInt("counter", 0);
  prefs.putInt("counter", counter + 1);
}



void setPrinterStatus()
{
  bool printerStatus = prefs.getBool("printerStatus", false);
  prefs.putBool("printerStatus", !printerStatus);
}