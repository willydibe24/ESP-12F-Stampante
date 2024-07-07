#include "../include/PrefsManager.hpp"


PrefsManager::PrefsManager()
{
    m_printerStandByAfterMs = 150000;
    m_lastInteractionTime = 0;
}

PrefsManager::~PrefsManager()
{

}


void PrefsManager::init()
{
    m_prefs.begin("smart-print");
    m_prefs.putInt("resets", m_prefs.getInt("resets", 0));
    m_prefs.putBool("printerOn", false);
}


void PrefsManager::reset()
{
    int prevValue = PrefsManager::getServoTriggerValue();
    m_prefs.clear();
    m_prefs.putInt("servoTriggerValue", prevValue);
}


int PrefsManager::getServoTriggerValue()
{
    return m_prefs.getInt("servoTriggerValue", 35);
}


int PrefsManager::getCounter()
{
    return m_prefs.getInt("counter", 0);
}


int PrefsManager::getResets()
{
    return m_prefs.getInt("resets", 0);
}


unsigned long PrefsManager::getLastInteractionTime()
{
    return m_lastInteractionTime;
}


bool PrefsManager::isPrinterOn()
{
    return m_prefs.getBool("printerOn", false);
}


void PrefsManager::setServoTriggerValue(int value)
{
    m_prefs.putInt("servoTriggerValue", value);
}


void PrefsManager::incrementCounter()
{
    m_prefs.putInt("counter", getCounter() + 1);
}


void PrefsManager::toggleIsPrinterOn()
{
    if (isPrinterOn() && millis() - m_lastInteractionTime > m_printerStandByAfterMs)
    {
        m_lastInteractionTime = millis();
        return;
    }
    m_lastInteractionTime = millis();
    toggleIsPrinterOn();
}
