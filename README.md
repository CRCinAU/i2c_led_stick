# i2c_led_stick
Arduino nano program to listen act as an i2c slave and use a basic protocol to control 8 x NeoPixel LEDs.

Uses the Adafruit NeoPixel library for LEDs, and wire.h for i2c comms.

To send a packet to the Nano via i2c:
```
# i2cset -y 0 0x08 0x01 0x01 0x00 0xFF 0x00 0x10 s
```

Protocol format:
```
# i2cset -y 0 <i2c address> <LED 0-7> <mode> <r> <g> <b> <brightness> s
```

Mode can be:
0 = not-init - Raise brightness from 0 -> 100 then reset.
1 = solid - Keep the LED on at all times.
2 = flash - Flash the led as 150ms on / 150ms off.
