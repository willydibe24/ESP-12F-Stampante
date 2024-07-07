#include "include/Credentials.hpp"
#include "include/WiFiManager.hpp"
#include "include/ServerManager.hpp"
#include "include/PrefsManager.hpp"
#include "include/ServoManager.hpp"
#include "include/OtaManager.hpp"
#include "include/EspalexaManager.hpp"


void setup()
{
    Serial.begin(115200);

    WiFiManager::getInstance().connect();

    ServerManager::init();

    PrefsManager::getInstance().init();

    OtaManager::init();

    ServoManager::getInstance().init();    

    EspalexaManager::init();
}


void loop()
{
    ServoManager& servoManager = ServoManager::getInstance();
    
    ArduinoOTA.handle();
    ServerManager::handleClient();
    EspalexaManager::loop();

    if (servoManager.getActive())
    {
        if (millis() - servoManager.getInteractionStartTime() >= 1200)
        {
            if (servoManager.getValue() == 90)
            {
                PrefsManager& prefsManager = PrefsManager::getInstance();
                servoManager.setValue(prefsManager.getServoTriggerValue());
                prefsManager.incrementCounter();
                prefsManager.toggleIsPrinterOn();
            }
            else
            {
                servoManager.setValue(90);
                servoManager.setActive(false);
            }
            servoManager.write();
            servoManager.setInteractionStartTime(millis());
        }
    }
}
