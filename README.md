ChipCap2 + SH1106 Display + Arduino

This project demonstrates how to read humidity and temperature data from a ChipCap2 sensor over I2C and display these readings on an SH1106 OLED using Adafruit libraries.
The display shows both large and small fonts (from the Adafruit GFX library) for clear, centered measurement readouts.

---
Project Motivation:

I originally created this to get a more accurate humidity reading for my 3D printing environment.
Monitoring ambient humidity helps improve print quality by preventing filament moisture issues.
I also 3D printed an enclosure for both the display and sensor to keep things neat and protected, and plan to share those enclosure designs on Makerworld in the near future.

---

Features:

Reads temperature and humidity from the ChipCap2 sensor.
Displays the data on a 128×64 SH1106 OLED.
Centers the measurement value (large font) and the unit (smaller font).
Easily shift the entire text on the display using HORIZONTAL_OFFSET and VERTICAL_OFFSET.

---

Required Libraries:

Adafruit GFX Library

Adafruit SH110X Library

You can install these libraries via the Arduino IDE’s Library Manager.

---

This project is licensed under the MIT License.

Feel free to use, modify, and distribute this code for personal or commercial projects. See the LICENSE file for more details.
