#include "../include/ServerManager.hpp"


ESP8266WebServer ServerManager::m_server(8080);


void ServerManager::init()
{
    m_server.enableCORS(true);
    m_server.on("/", HTTP_GET, callbackRoot);
    m_server.on("/trigger", HTTP_POST, callbackTrigger);
    m_server.on("/set-trigger-value", HTTP_POST, callbackSetTriggerValue);
    m_server.on("/info", HTTP_GET, callbackInfo);
    m_server.on("/reset-values", HTTP_POST, callbackResetValues);
    m_server.begin();
}


void ServerManager::callbackRoot()
{
    m_server.send(200, "text/plain", "Hello from ESP-12F!");
}


void ServerManager::callbackTrigger()
{
    ServoManager::getInstance().setActive(true);
    m_server.send(200, "text/plain", "TRIGGER OK.");
}


void ServerManager::callbackSetTriggerValue()
{
    if(!m_server.arg("value"))
      return m_server.send(400, "text/plan", "Missing \"value\" parameter.");

    int value = atoi(m_server.arg("value").c_str());
    if(value <= 0)
      return m_server.send(400, "text/plain", "Bad parameter.");

    PrefsManager::getInstance().setServoTriggerValue(value);
    String message = "VALUE OK. New value is " + m_server.arg("value") + ".";

    m_server.send(200, "text/plain", message);
}


void ServerManager::callbackInfo()
{
    PrefsManager& prefsManager = PrefsManager::getInstance();

    StaticJsonDocument<216> JsonDocument;
    JsonDocument["ip"] = WiFi.localIP().toString();
    JsonDocument["counter"] = prefsManager.getCounter();
    JsonDocument["resets"] = prefsManager.getResets();
    JsonDocument["servoValue"] = ServoManager::getInstance().read();
    JsonDocument["servoTriggerValue"] = prefsManager.getServoTriggerValue();
    JsonDocument["printerOn"] = prefsManager.isPrinterOn();
    JsonDocument["secondsFromLastPrinterInteraction"] = (millis() - prefsManager.getLastInteractionTime()) / 1000; 

    String jsonString;
    serializeJson(JsonDocument, jsonString);
    m_server.send(200, "application/json", jsonString);
}


void ServerManager::callbackResetValues() 
{
    PrefsManager::getInstance().reset();
    m_server.send(200, "text/plain", "RESET VALUES OK.");
}


void ServerManager::handleClient()
{
    m_server.handleClient();
}