# ExpanseAirTank

This project is intended to be paired with https://www.thingiverse.com/thing:3406844

I built mine using an Adafruit 2.4" TFT FeatherWing, Adafruit Feather nRF52 Bluefruit LE -nRF52832, and a 400mAh Lipo battery. 

Links:
https://www.adafruit.com/product/3315
https://www.adafruit.com/product/3406
https://www.adafruit.com/product/3898

Please buy them from Adafruit if you can. They do great stuff, provide support, and are awesome folks.

This project is ROUGH around the edges, as my C skills are rusty at best.  So other boards or displays may not work.

Version 1 starts out our capacity bars at some value, then exponentially decays them.
Version 2 will do a better job and change the color from blue (100%-50%), Yellow (50%-25%), Red (bellow 25%).

If you ar curious the text is AL=Activity Level, BR=Breathing Rate, ST=?, HR=Heart Rate, 02=Oxygen.

You will need to make sure that your Arduino IDE has the support for your board setup:
for the nrf52 board:
https://learn.adafruit.com/bluefruit-nrf52-feather-learning-guide/arduino-bsp-setup

And install the required Libraries:
Adafruit GFX
Adafruit ILI9341
gfxfont
