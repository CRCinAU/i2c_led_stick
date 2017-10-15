# i2c_led_stick
Arduino nano program to listen act as an i2c slave and use a basic protocol to control 8 x NeoPixel LEDs.

Uses the Adafruit NeoPixel library for LEDs, and wire.h for i2c comms.

To send a packet to the Nano via i2c:
```
# i2cset -y 0 0x08 0x01 0x01 0x00 0xFF 0x00 0x10 s
```

Protocol format:
```
# i2cset -y 0 <i2c address> <LED 0-7> <r> <g> <b> <brightness> s
```
