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
   // int x_;
    String ipAddress_;

public:
    // void x(const int &value); // { x_ = value; }
    // const int &x() const { return x_; }

    void ipAddress(const String &value); // { x_ = value; }
    const String &ipAddress() const { return ipAddress_; }
};

#endif
