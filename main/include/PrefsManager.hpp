#pragma once
#include <Preferences.h>


class PrefsManager {
public:
    inline static PrefsManager& getInstance()
    {
        static PrefsManager instance;
        return instance;
    }

    ~PrefsManager();

    PrefsManager(const PrefsManager&) = delete;
    void operator=(const PrefsManager&) = delete;

    void init();
    void reset();
    int getServoTriggerValue();
    int getCounter();
    int getResets();
    unsigned long getLastInteractionTime();
    bool isPrinterOn();

    void setServoTriggerValue(int value);
    void incrementCounter();
    void toggleIsPrinterOn();

private:
    PrefsManager();

    Preferences m_prefs;
    unsigned long m_printerStandByAfterMs;
    unsigned long m_lastInteractionTime;
};