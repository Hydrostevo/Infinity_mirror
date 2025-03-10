# Infinity_mirror
An ESP8266 powered infinity mirror with WS2812B RGBLEDs and an 0.96" OLED

## Concept:
Using stuff from a box of salvaged components, a damaged 0.96" I2C OLED screen and some one way mirror film build an infinity mirror that looks like it fell out of a 90's SciFi film.

### Parts
- ESP8266
- 0.96" OLED
- WS2812B RGBLEDs
- Perf board (the cheap stuff from Amazon)
- Random components that sort of look cool
- One way mirror film
- 3mm Acrylic sheet

## Code sources
- [Random Nerds - OLED](https://randomnerdtutorials.com/esp8266-0-96-inch-oled-display-with-arduino-ide/)
- [Random Nerds - OTA](https://randomnerdtutorials.com/esp8266-nodemcu-ota-elegantota-arduino/)
- [FastLED](https://fastled.io/)
- [FastLED - Multiple animations](https://github.com/marmilicious/FastLED_examples)
- [NerdForge - Infinite Ghost Library](https://youtu.be/oR80TuOCoic?si=QbNQUZnfUyZrZtNT)
- [OLED label 'Droidobesh' font](https://www.fontspace.com/droidobesh-depot-font-f55049)
- [Sinewave generator for OLED](https://mymixblogwork.blogspot.com/2024/11/sine-wave-on-oled-display-code.html)

### Development actions
- OTA Updates - Done, issue with FastLED and ElegantOTA resolved
- Interactive Webserver (Pending)
  - 'System start up' sequence
  - 'Red Alert' sequence
  - 'System failure' sequence
- Additional I/O triggers (Pending)
  - Touch/proximity sequence activation
  - Response to ambient temperature/light levels
- Mesh with other ESP8266 to build an interactive system  (Pending)

