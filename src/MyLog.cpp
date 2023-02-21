#include "MyLog.hpp"

MyLog::MyLog(bool use_serial): use_serial(use_serial) {
    if (use_serial) {
        Serial.begin(9600);
        while (!Serial) _NOP();
        Serial.println("Started");
    }
}

auto MyLog::init(bool use_serial) -> const MyLog& {
    static MyLog log(use_serial);
    return log;
}