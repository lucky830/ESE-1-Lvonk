# ESE-1-Lvonk
All my projects and code related to my 1st year Embedded Systems Engineering study

## RMPocket2ESP32
This is a test/research (mini)project to test if it was possible to use the 'BLE joystick' mode of the Radiomaster Pocket. <br>
I wanted to control the esp32, or at least create functions of the controls. So I am able to use it for future projects, if I want to use my Radiomaster Pocket to control actuators remotely.

**This is *to my rough knowledge* how it roughly works:**

The working of converting the raw bluetooth signals of the Radiomaster Pocket into usable signals:

The Raw data of the radiomaster pocket is 18 byte using little-endian formatting, 0x7FFF = 32767 <br> (standard maximum for a 16 bit joystick axis).

so it uses byte pairs, the first one is the low byte and the second is the high byte (exact working I don't know)
```
0 09 40 09 40 13 00 31 40 09 40 13 00 09 40 13 00 00 = example hex dump
0 1  2  3  4  5  6  7  8  9  10 11 12 13 14 15 16 17 = byte location
```
**I have figured out the byte-pair location of all the controls:** <br>
**L y-axis:** 12/13 <br> **L x-axis:** 8/9 <br> **R y-axis:** 4/5 <br>**R x-axis:** 2/3 <br> **Left Button (SA):** 6/7 <br>  **Right Button (SD):** 16/17

**3pos switch Left (SB):** 10/11 <br> **3pos switch Right (SC):** 14/15 <br> L**eft bottom button (SE):** 1 (value only 0 or 1)
  

After converting the byte pairs to 16-bit integers the range is 0-32767 or in reality 19 - 32748 (in my case),
but for ease of use you want a 0-100% range, so you can do: % = (signal - 19) / 32748
or, % = (signal - signal_min_value) / signal_max_value if you wish to calibrate the range first

I wanted to control a H-bridge dc motor controller which works with a PWM signal of 0-255 so it can be converted like this:
((signal_value - 19) / 128.42352941175), where the 128.4... comes from simplifying; (signal_value - 19) / 32767 * 2.55

***PS:** This may be - and very possibly is not the most optimal way to achieve my goal, but it works, so I'm happy :)*
