# Whammy - A bluetooth expression pedal using tilt angle to trigger effects

The Whammy system consists of two pieces of hardware, both of which
are compiled from this repository.    Much of the source code is
common.

The software runs atop the Zephyr real time operating system, and
makes use of an application module for Zephyr named 'ztacx' (also
written by the author of whammy).

The hardware components are

## Whammy Puck

A bluetooth Low-Energy (BLE aka Bluetooth 4, which is a totally
different protocol than older versions of bluetooth) 
peripheral that contains an accelerometer and magnetometer.

In BLE you can think of "peripheral" as meaning "server".
It exposes all the sensors as "characteristics", characteristics being
in essence "pages" that the server can serve.

The puck is battery powered, and attaches to your instrument via a
suction cup.   The battery should last many days (but should be
disconnected when not in use).

Bluetooth characteristics are identified by UUIDs, which can be 
from a standard service profile, or vendor specific.

For ease of use we use the standard "Generic Level" characteristic ID 
to serve the "wham factor".   Many other sensors and details are also
presented.   You can test the puck using the 'nrfConnect' app on android or iphone to
scan for the device and browse these characteristics.  (Note the device will not be
accessible from your phone if something else is already connected to
it, like the receiver).

The puck has an LED.   1 second blink rate means "waiting for
connection", 5 second blink rate means "somebody is connected".

The puck is configured to expose the "X-Axis Acceleration" in
cm/sec/sec as the "generic level".  For tilt this would range from
-980 (normal acceleration due to gravity), through zero (the device is
perfectly flat therefore there is no acceleration in the X-direction),
to 980 (1G or 9.8m/s/s, x-axis aligned straight down).   Under conditions of shock or
shaking acceleration could be much higher (the device is configured
for a max of 2G).

## Whammy Receiver

The receiver has a colour (RGB) LED and a digital-to-analog converter (DAC), built
into the back of a Tip-Ring-Sleeve phono plug.   Sleeve is ground, Tip
is 5-12 volts input, and Ring is analog out (0-to-3.3 volts).

The receiver is intended to plug into the "expression pedal" jack of
any audo effect box.

The receiver acts as a "bluetooth central" device (bluetooth-speak for
client).   The receiver scans for a peripheral named "whammy puck"
(LED will be orange).   When it connects (LED goes green) it looks for
the "Generic Level" characteristic, and monitors its value.   Whenever
the value changes the receiver blinks the LED blue and updates the
DAC.

The receiver converts the "generic level" range of [-980, 980] to the
DAC's parameter range [0,4095].  The DAC expresses its parameter as a
voltage, from [0, 3.3v].

It is intended that your effect be configured to treat this analog
voltage as an input to its effect processing.

A test fixture is supplied which uses a USB cable to supply 5 volts to
the receiver, and displays the output voltage on a voltmeter.

