#pragma once
#include "Credentials.hpp"
#include <Arduino.h>
#include <ESP8266WiFi.h>

class WiFiManager {
public:
    inline static WiFiManager& getInstance()
    {
        static WiFiManager instance;
        return instance;
    }

    ~WiFiManager();

    WiFiManager(const WiFiManager&) = delete;
    void operator=(const WiFiManager&) = delete;

    void connect();
    String getLocalIP();

private:
    WiFiManager();
    String m_localIP;
};