//
// Arduino main stub file. Calls into Rust.
//

extern "C" void arduino_setup();
extern "C" void arduino_loop();

extern "C" void setup() {
    arduino_setup();
}

extern "C" void loop() {
    arduino_loop();
}
