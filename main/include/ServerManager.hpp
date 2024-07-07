#pragma once
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "PrefsManager.hpp"
#include "ServoManager.hpp"


class  ServerManager {
public:
    ServerManager() = delete;
    ServerManager(const ServerManager&) = delete;
    ServerManager& operator=(const ServerManager&) = delete;

    static void init();
    static void handleClient();

private:
    static ESP8266WebServer m_server;
    static void callbackRoot();
    static void callbackTrigger();
    static void callbackSetTriggerValue();
    static void callbackInfo();
    static void callbackResetValues();
};