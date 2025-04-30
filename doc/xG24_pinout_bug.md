# xG24 Explorer Kit Pinout Bug

For xG24 explorer kit, there is currently a bug in Si Labs Arduino Core **Version 2.3.0**.

The Serial1 pins (Tx1 and Rx1) are assigned to INT (A6/PB01/Pin 13) and PWM (A7/PA00/Pin 14) pins on the board respectively, instead of the pins which are marked as TX (A4/PD4/Pin 11) and RX (A5/PD5/Pin 12).

[Link to GitHub issue](https://github.com/SiliconLabs/arduino/issues/123)

This bug is accomodated for in `src/TouchScreen/src/ts_globalValues.h` by connecting the RA8875's interrupt line to pin 11.

> `#define RA8875_INT 11 // MikroBUS-TX on xG24`

This bug is currently being investigated and might be fixed in future releases of the Si Labs Arduino Core.

Here is the response from an Si Labs representative.

![Si Labs Representative Response](../img/Si%20Labs%20Arduino%20Core%20Bug.png)

If the bug is fixed, the interrupt definition should be swapped to

> `#define RA8875_INT 13 // INT on xG24`