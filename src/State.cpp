#include <Arduino.h>
#include "State.h"

// void State::x(const int &value) { x_ = value; }

void State::ipAddress(const String &value)
{
    ipAddress_ = value;

    sendMessage("sn1/state/ip", ipAddress_);
}