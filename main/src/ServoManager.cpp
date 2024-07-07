#include "../include/ServoManager.hpp"


ServoManager::ServoManager()
{
    m_interactionStartTime = 0;
    m_active = false;       // when /trigger route is called, the variable switches to true for 500ms
    m_value = 90;           // default value is set to 90 deg
}


ServoManager::~ServoManager()
{
    
}


void ServoManager::init()
{
    m_servo.attach(2);
}


bool ServoManager::getActive()
{
    return m_active;
}


unsigned long ServoManager::getInteractionStartTime()
{
    return m_interactionStartTime;
}


int ServoManager::getValue()
{
    return m_value;
}


void ServoManager::setActive(bool active)
{
    m_active = active;
}


void ServoManager::setPrevTime(unsigned long time)
{
    m_interactionStartTime = time;
}


void ServoManager::setValue(int value)
{
    m_value = value;
}


void ServoManager::write()
{
    m_servo.write(m_value);
}


int ServoManager::read()
{
    return m_servo.read();
}