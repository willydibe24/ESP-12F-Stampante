#pragma once
#include "Servo.h"

class ServoManager {
public:
    inline static ServoManager& getInstance()
    {
        static ServoManager instance;
        return instance;
    }

    ~ServoManager();

    ServoManager(const ServoManager&) = delete;
    void operator=(const ServoManager&) = delete;

    void init();
    bool getActive();
    unsigned long getInteractionStartTime();
    int getValue();

    void setActive(bool active);
    void setInteractionStartTime(unsigned long time);
    void setValue(int value);
    void write();
    int read();

private:
    ServoManager();

    Servo m_servo;
    bool m_active;
    unsigned long m_interactionStartTime;
    int m_value;
};