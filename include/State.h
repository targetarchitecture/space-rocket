#ifndef State_h
#define State_h

#include <Arduino.h>

extern void sendMessage(String topic, String message);
extern void sendMessage(String topic, uint8_t message);
extern void printMessage(String message);
extern void printMessage(String message, uint8_t arg1);

class State
{
private:
    int volume_;
    String ipAddress_;
    int fileNumber_;
    String error_;
    String warning_;
    String current_;

public:
    void volume(const int &value);
    const int &volume() const { return volume_; }

    void ipAddress(const String &value);
    const String &ipAddress() const { return ipAddress_; }

    void fileNumber(const int &value);
    const int &fileNumber() const { return fileNumber_; }

    void error(const String &value);
    const String &error() const { return error_; }

    void warning(const String &value);
    const String &warning() const { return warning_; }

    void current(const String &value);
    const String &current() const { return current_; }

    void ready();
};

#endif
