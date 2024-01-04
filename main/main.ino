#include <Servo.h>
#include <ArduinoOTA.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "credentials.hpp"


unsigned long servoPrevTime = 0;
bool servoTrigger = false;          // when /trigger route is called, the variable switches to true for 500ms
int servoValue = 90;                // default value is set to 90 deg

// unsigned long printerStandByAfterMs = 300000;
unsigned long printerStandByAfterMs = 10000;
unsigned long prevPrinterInteractionTime = 0;

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
    
    bool printerOn = prefs.getBool("printerOn", false);
    prefs.putBool("printerOn", false);

    // END PREFERENCES (LOCALSTORAGE) CONFIG

    // ---------------------------------
    // OTA CONFIG
    // ---------------------------------

    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);
    ArduinoOTA.onError([](ota_error_t error)
    {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();

    // END OTA CONFIG

    servo.attach(2);
}



void loop()
{
    ArduinoOTA.handle();
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
                setPrinterOn();
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
    StaticJsonDocument<216> JsonDocument;
    JsonDocument["ip"] = WiFi.localIP().toString();
    JsonDocument["counter"] = prefs.getInt("counter", 0);
    JsonDocument["resets"] = prefs.getInt("resets", 0);
    JsonDocument["servoValue"] = servo.read();
    JsonDocument["servoTriggerValue"] = prefs.getInt("servoTriggerValue", 35);
    JsonDocument["printerOn"] = prefs.getBool("printerOn", false);
    JsonDocument["secondsFromLastPrinterInteraction"] = (millis() - prevPrinterInteractionTime) / 1000; 

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



void setPrinterOn()
{
    bool printerOn = prefs.getBool("printerOn", false);
    if (printerOn && millis() - prevPrinterInteractionTime > printerStandByAfterMs) 
    {
        prevPrinterInteractionTime = millis();
        return;
    }
    prevPrinterInteractionTime = millis();
    prefs.putBool("printerOn", !printerOn);
}