#include <Arduino.h>
#include "State.h"

void State::ipAddress(const String &value)
{
    ipAddress_ = value;

    sendMessage("sn1/state/ip", ipAddress_);
}

void State::ready()
{
     sendMessage("sn1/state", "ready");
}

void State::volume(const int &value)
{
    volume_ = value;

    sendMessage("sn1/state/volume", volume_);
}

void State::fileNumber(const int &value)
{
    fileNumber_ = value;

    sendMessage("sn1/state/fileNumber", fileNumber_);
}

void State::error(const String &value)
{
    error_ = value;

    sendMessage("sn1/state/error", error_);
}

void State::warning(const String &value)
{
    warning_ = value;

    sendMessage("sn1/state/warning", warning_);
}

void State::current(const String &value)
{
    current_ = value;

    sendMessage("sn1/state/current", current_);
}