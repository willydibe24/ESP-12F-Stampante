#pragma once
#include <ArduinoOTA.h>
#include "Credentials.hpp"


class OtaManager {
public:
    OtaManager() = delete;
    OtaManager(const OtaManager&) = delete;
    OtaManager& operator=(const OtaManager&) = delete;

    static void init();
};