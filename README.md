# Paragliding simulator control unit.

This project uses an IR controller to flash two LEDs in pre-defined sequences.

## Documentation

The device will start with both LEDs on, then a short flash sequence.  If one LDE does not show at start-up there is a fault.

The onboard (red) LED will flash periodically, indicating the device is alive.

The IR control is relatively weak, and the IR signal needs to travel through the blue box.  The sensor is adjacent to the internal LEDs and points upwards (in the same direction).  The IR control works from a distance of approximately 30cm or less, and an angle of approx +-30 degrees from vertical. (Not great, but hopefully workable.)

Buttons do the following.

* `0` A short flash sequence, then turn off both LEDs.
* `1` LED sequence 1.
* `2` LED sequence 2.
* `3` LED sequence 3.
* `7` Flash the left hand LED continuously (red wire).  Press a second time to stop.
* `9` Flash the right hand LED continuously (green wire).  Press a second time to stop.

Sequences 1, 2 and 3 can be used for the experiment.

The other buttons can be used to test, and reset the device.

Note, there is no interrupt function.  Once an LED sequence is initiated it cannot be stopped.  To interrupt and reset the device switch it on and off using the toggle switch.

## Parts list

1. Arduino Nano V3.0 board ATmega328P
2. Hammond Blue & Black ABS Plastic Enclosure Project Box  (Translucent Blue, 1591XXMTBU (85 x 56 x 24mm), but a bit too small)
3. HX1838 Infrared IR Wireless Remote Control Module VS1838
4. PCB Prototype Circuit Board
5. PC ATX Desktop Power Switch Cable set (For LEDs only)
6. 9V PP3 batteries, connector and toggle switch.
7. 2 * 330ohm resistors.
