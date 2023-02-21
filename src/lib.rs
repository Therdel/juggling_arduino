// Remove if STD is supported for your platform and you plan to use it
#![no_std]

// Remove if STD is supported for your platform and you plan to use it
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {
    loop {}
}

//
// FFI C -> Rust
//
#[no_mangle]
extern "C" fn arduino_setup() {
}

#[no_mangle]
extern "C" fn arduino_loop() {
    unsafe {
        rampLED();
    }
}

//
// FFI Rust -> C
// source: https://web.mit.edu/rust-lang_v1.25/arch/amd64_ubuntu1404/share/doc/rust/html/book/first-edition/ffi.html#foreign-calling-conventions
//
#[allow(non_snake_case)]
extern "C" {
    fn rampLED();
}
// #[allow(non_snake_case)]
// extern "C" {
//     fn rampLED();
//     fn sleepMS(ms: u32);
//     fn println(msg: *const core::ffi::c_char);
// }

// extern "C" void sleepMS(uint32_t ms) {
//     const auto duration = std::chrono::milliseconds(ms);
//     rtos::ThisThread::sleep_for(duration);
// }

// extern "C" void println(const char *msg) {
//     Serial.print(msg);
//     Serial.print('\n');
//     // Serial.println(msg);
// }