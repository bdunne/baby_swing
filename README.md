## Fixing a Graco Simple Sway baby swing

I got a baby swing for free.  It was not in working condition and missing the power adapter, but otherwise in good shape.
Since I enjoy tinkering with electronics in my free time, I opened it up. I didn't see anything visibly wrong with the circuit board, but couldn't get it to do anything.  It accepts 4 D size batteries, so I assumed it should work with somewhere around 5-6 volts.  I tested the motor independently and it (and the gear box) worked fine, so I decided to take a stab at replacing the board with an Arduino and some off the shelf components.
My to-do list was as follows:
- Reverse engineer the optical encoder to determine the pinout
- Connect the optical encoder to the arduino and use it to determine which direction the swing is moving
- Power the motor in the direction of travel for a set amount of time
- Reverse the power to the motor for the same amount of time once the swing direction has changed

### Reverse engineer the optical encoder
There's a plastic disc connected to the motor shaft with some cutouts in it and 2 photo interrupt sensors that are mounted to the gearbox housing.  As the disc turns, a window opens for one sensor before the other.  Electrically the sensors each output a square waveform and they are offset from each other slightly.  There are 2 LEDs on one side of the wheel requiring power and ground and 2 "switches" on the other side of the wheel which require power and have an output to the processor, so at least 4 pins (power, ground, output A, output B).

### Connect the optical encoder to the arduino
Powering leds is simple, so I started there and connected the emitter LEDs to power (guessing at resistor values).  Using a volt meter on the receiver side I was trying to get an understanding of the pinout and determine whether or not the LEDs were even working since they are IR LEDs and not in the visible spectrum.  Based on the burning electronics smell after a very short time, I guess my resistor value was too low.
A couple days later a package of new photo interrupt sensors arrived.  This time I had a datasheet available, so I knew both the resistor value necessary and the pinout.  The new sensors were not exactly the same size, so I made up a small PCB (with the correct resistors on board) out of some stripboard, mounted it to the gearbox and connected it to my Arduino UNO on digital pins 2 & 3 (they can be used for interrupts).  I set an interrupt on pin 2 to detect the rising edge of sensor A.  When detected, we read the value of sensor B.  If it's high, the swing is moving forward, it it's low, the swing is moving backwards.
Since we get the interrupt every time we have a rising edge, I added a small amount of code so that we only set the direction variable when the direction has changed.  At the same time I also set a variable for `pushRequired` that my loop code can use to figure out when to turn the motor on.

### Power the motor in the direction of travel
I had some 2N2222 transistors, so I used one to turn on an LED for a short time whenever the swing was moving forward.  Once I was happy with this I connected it to the motor and tried it out.  It worked while empty, but started to slow down if there was weight in the seat.

### Reverse the power to the motor
To get a more consistent swing motion, we need to push the swing for the same amount of time in both directions.  In order to do that, we can reverse the polarity to the motor with an H-bridge built out of 4 2N2222 transistors (I don't have any h-bridge chips hanging around). Since I'm running everything off of the 5 volt arduino supply it works.  If the motor supply were higher than the arduino can pull the output pin, I woud need more transistors, but since it works on 5 volts, I'm going to stick with this.  I also added the recommended diodes pointing toward +5V to protect the transistors.

### Results
At this point I have something working and a few seconds after startup it settles into a swinging motion to approximately match the motor on time.  I pulled out a spare ATmega328P, uploaded a the Arduino bootloader and set up the minimum number of components on the breadboard to make it work (boarduino).  After reconnecting everything, I ran some more thorough tests (varying amounts of weight, run it off of a USB power bank).
This whole time the motor has been a little bit jittery and I thought this was due to the small amount of play in the gears and the optical encoder picking that up as a direction change too early, possibly needing a software correction.  But during one of my tests, the motor stopped being driven completely.  After some investigating with the scope, I found that turning the motor on caused a slight voltage drop, and turning it off (allowing the carrier to free swing) sent a voltage spike across the line.  Even with the wide input voltage range that the Atmega allows, both of these conditions are bad for it.  Adding a diode from the input power line to the ATmega along with a small capacitor helped with the voltage drop.  Adding another diode from the input power line to the motor feed prevented the spikes from hitting the rest of the electronics (ATmega & optical sensor).  Surprisingly, this also fixed the jitter issue.

Next up:
- See if I can salvage and use the speed selector switch and use it to vary the motor on time (for a longer swing)
- Make up another PCB to hold the ATmega and H-bridge that fits nicely in the housing
- Work out a permanent power source (I really like the USB power bank setup)
