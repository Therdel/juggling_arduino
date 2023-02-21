## Getting Started
1. install vscode & PlatformIO plugin, then
2. install rust
3. install rust target with ```rustup target add thumbv7em-none-eabi```

## Fixes from cargo pio
1. set target ```thumbv7em-none-eabi``` in platformio.ini and .cargo/config.toml (rust binary using VFP registers, while pio firmware doesn't)
2. apply ```cpp extern "C"``` to ```setup()``` and ```loop()``` in main.cpp (undefined reference to setup() and loop() error)

## Fix slow code completion (generate compile commands JSON)
1. Try not coding in header, but in source files
1. Follow PlatformIO tutorial: https://docs.platformio.org/en/latest/integration/compile_commands.html
    - if pio command doesn't work: https://community.platformio.org/t/how-to-fix-pio-command-not-working/19084/2
2. Put in the path (may be relative) returned by the pio command here: https://code.visualstudio.com/docs/cpp/c-cpp-properties-schema-reference#_example

### todo
[ ] thumbv7em-none-eabihf could be usable, nRF52840 has hw float support, after all. Try this https://community.platformio.org/t/changing-mfloat-abi-build-option-not-working/26306 
[ ] report cargo pio ```setup()``` & ```loop()``` linker problems (test if including ```Arduino.h``` fixes it)
