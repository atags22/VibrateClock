# VibrateClock


This is a project meant to run on an MSP430. I'm using the G2102 variant. It'll probably work with minimal modification on most G-series chips.

The important parts of the circuit are as follows:
	The chip is connected to power and ground, and a 10k resistor connects VCC to the RST pin (pin 16 on my 20-pin chip).
	Pin 2.3 (pin 11 on my 20-pin chip) is connected to a button. The circuit is normally open, but when the button is pressed it brings pin 2.3 down to ground. The code enables an internal pullup resistor.
	Pin 1.0 (pin 2 on my 20-pin chip) is connected to the Collector pin of a transistor via a 10k resistor. The transistor's other two pins connect to Ground and the negative lead of my vibration motor (I used an NPN transistor).

Less importantly, I'm powering the MSP430 from a CR2032 button cell battery and the vibration motor from a rechargable ML2020 button cell battery. There's a built-in charging circuit.
I made this decision because I want the MSP430 to still be powered even if the vibration motor's battery dies. This way the clock will keep the time for many many years.
