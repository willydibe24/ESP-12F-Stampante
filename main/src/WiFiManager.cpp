#include "../include/WiFiManager.hpp"


WiFiManager::WiFiManager()
{

}


WiFiManager::~WiFiManager()
{

}


void WiFiManager::connect() 
{
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.mode(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connessione in corso...");
    }

    m_localIP = WiFi.localIP().toString();
    Serial.println("WI-FI connesso!");
    Serial.print("Indirizzo IP ESP-12F: ");
    Serial.println(m_localIP);
}


String WiFiManager::getLocalIP()
{
    return m_localIP;
}