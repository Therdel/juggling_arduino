#pragma once
#include <atomic>
#include <Arduino.h>

class MyLog {
public:
    MyLog(bool use_serial);
    static auto init(bool use_serial = true) -> const MyLog&;

    template <class ... Ts>
    static auto log(Ts && ... inputs) -> void;

private:
    bool use_serial;
};

template <class ... Ts>
auto MyLog::log(Ts && ... inputs) -> void {
    const MyLog &instance = init();
    if (instance.use_serial) {
        (Serial.print(std::forward<Ts>(inputs)), ...);
        Serial.println();
    }
}