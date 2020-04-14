# kernelcon-2020-badge
## built by @zonksec and @scotcsec
the badge was a dual pcb design with mouting hardware carfully located to make it hold a custom image reel. controlled by an atmel atmeg328p-au. powered by 3 AAA batteries. the 9 LEDs are addressable RGBs (apa102). two act as backlights for the custom image reel. the badge offered a serial port to play a ctf mini game, but the mini game was removed when the conference went virtual. the badge was programed via an ICSP interface using pogo pins (see conference talk for more details). EEPROM was used to save mode type between power cycles. mode indicator functionality was added to better track what mode you are in. a button can be used to change modes, which are the following:
* fader mode, each LED faded between colors independently 
* freeze of of whatever the previous state was
* fader mode, all LEDs faded between colors in unison
* freeze of of whatever the previous state was
* just backlit reel
* rave mode (flashing colors)
* looping animation with a color change after each lap
* knight rider 
* windmill
* pseudorandom iterations color and LED
* pseudorandom selection of led made white (twinkle twinkle)

Check out "/reels" for what images were used to make the custom reel.


![picture](images/badge.gif)
![picture](images/badges.jpg)
![picture](images/fullreel.jpg)
