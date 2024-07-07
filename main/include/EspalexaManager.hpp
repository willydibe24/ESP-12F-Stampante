#pragma once
#include <Espalexa.h>
#include "ServoManager.hpp"


class EspalexaManager {
public:
    EspalexaManager() = delete;
    EspalexaManager(const EspalexaManager&) = delete;
    EspalexaManager& operator=(const EspalexaManager&) = delete;

    static void init();
    static void loop();

private:
    static Espalexa m_espalexa;
    static void callbackServo(uint8_t brightness);
};