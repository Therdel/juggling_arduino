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