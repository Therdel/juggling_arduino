## Getting Started
1. install rust target with ```rustup target add thumbv7em-none-eabi```

## Fixes from cargo pio
1. set target ```thumbv7em-none-eabi``` in platformio.ini and .cargo/config.toml (rust binary using VFP registers, while pio firmware doesn't)
2. apply ```cpp extern "C"``` to ```setup()``` and ```loop()``` in main.cpp (undefined reference to setup() and loop() error)

### todo
[ ] thumbv7em-none-eabihf could be usable, nRF52840 has hw float support, after all. Try this https://community.platformio.org/t/changing-mfloat-abi-build-option-not-working/26306 
[ ] report cargo pio ```setup()``` & ```loop()``` linker problems (test if including ```Arduino.h``` fixes it)
