#include "../include/EspalexaManager.hpp"

Espalexa EspalexaManager::m_espalexa;

void EspalexaManager::init()
{
    m_espalexa.addDevice("ESP Stampante Cameretta", callbackServo);
    Serial.print("Dispositivo individuabile dall'app Alexa come: ");
    Serial.println(m_espalexa.getDevice(0)->getName());
    m_espalexa.begin();
}


void EspalexaManager::loop()
{
    m_espalexa.loop();
    delay(1);
}


void EspalexaManager::callbackServo(uint8_t brightness)
{
    if (brightness)
    {
        ServoManager::getInstance().setActive(true);
    }
}