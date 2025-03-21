// ---------------------------------------------------------------------------
// This project demonstrates how to read humidity and temperature data from
// a ChipCap2 sensor over I2C, and display these readings on an SH1106 128×64 OLED
// using Adafruit libraries. The display uses both large and small fonts
// (from the Adafruit GFX library) to show the measurement values and units
// in a clear, centered format.
// ---------------------------------------------------------------------------

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// 1) Choose a font from the Adafruit GFX library:
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
// Other options include FreeSansBold9pt7b.h, FreeSerif9pt7b.h, etc.

// I2C address for the ChipCap2 sensor
const uint8_t CC2_ADDRESS = 0x28;

// Create a concrete class for the SH1106 display (128x64)
Adafruit_SH1106G display(128, 64, &Wire, -1);

// ------------------------------------------------------------------
// You can MODIFY these two variables to shift the text:
//
// - HORIZONTAL_OFFSET:  + value = move right,  - value = move left
// - VERTICAL_OFFSET:    + value = move down,   - value = move up
// ------------------------------------------------------------------
int16_t HORIZONTAL_OFFSET = 0;
int16_t VERTICAL_OFFSET   = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize the display (0x3C is the most common I2C address)
  if (!display.begin(0x3C)) {
    Serial.println("No SH1106 display found at address 0x3C!");
    while (1) { delay(10); }
  }

  // Clear the display
  display.clearDisplay();
  display.display();

  Serial.println("ChipCap2 + SH1106 + FreeFont example");
}

void loop() {
  float humidity = 0.0;
  float temperature = 0.0;

  // Read sensor
  readChipCap2(&humidity, &temperature);

  // Display humidity for 2 seconds
  showCenteredValue(humidity, " %", &FreeSans18pt7b);
  delay(2000);

  // Display temperature for 2 seconds
  showCenteredValue(temperature, " C", &FreeSans18pt7b);
  delay(2000);
}

// ------------------------------------------------------
// Reads humidity and temperature from the ChipCap2 sensor
// ------------------------------------------------------
void readChipCap2(float* humidity, float* temperature) {
  Wire.beginTransmission(CC2_ADDRESS);
  Wire.endTransmission();
  delay(50);

  uint8_t gotBytes = Wire.requestFrom(CC2_ADDRESS, (uint8_t)4);
  if (gotBytes == 4) {
    uint8_t data[4];
    for (uint8_t i = 0; i < 4; i++) {
      data[i] = Wire.read();
    }

    uint16_t rawHumidity = ((data[0] & 0x3F) << 8) | data[1];
    float hum = (rawHumidity / 16383.0) * 100.0;

    uint16_t rawTemp = ((data[2] << 8) | data[3]) >> 2;
    float temp = (rawTemp / 16383.0) * 165.0 - 40.0;

    *humidity = hum;
    *temperature = temp;
  } else {
    *humidity = -1.0;
    *temperature = -999.9;
  }
}

// ------------------------------------------------------
// Displays a float value (in a large font) plus its unit
// (in a smaller font) centered on a 128×64 display.
//
// NOTE: HORIZONTAL_OFFSET and VERTICAL_OFFSET
// help shift the entire text block (value + unit).
// ------------------------------------------------------
void showCenteredValue(float value, const char *unit, const GFXfont *font) {
  display.clearDisplay();

  // 1) Convert the numeric value to a string (e.g., "23.4")
  char valStr[10];
  dtostrf(value, 0, 1, valStr); // one decimal place

  // Choose a smaller font for the unit
  const GFXfont* unitFont = &FreeSans9pt7b;  

  // --- 1) Bounding box for the large text (measurement value) ---
  display.setFont(font);
  int16_t bx1, by1;
  uint16_t bw, bh;
  display.getTextBounds(valStr, 0, 0, &bx1, &by1, &bw, &bh);

  // --- 2) Bounding box for the smaller unit text ---
  display.setFont(unitFont);
  int16_t ux1, uy1;
  uint16_t uw, uh;
  display.getTextBounds(unit, 0, 0, &ux1, &uy1, &uw, &uh);

  // --- 3) Calculate total width = large text width + spacing + unit width
  int16_t spacing = 6;  // adjust spacing between the value and unit here
  uint16_t totalW = bw + spacing + uw;

  // Use the larger of the two text heights
  uint16_t totalH = (bh > uh) ? bh : uh;

  // --- 4) Center the text block on the 128x64 display
  int16_t xPos = (128 - totalW) / 2;
  int16_t yPos = (64 - totalH) / 2; 

  //  -> offset for the large text
  xPos -= bx1;  
  yPos -= by1;

  //  -> apply HORIZONTAL_OFFSET, VERTICAL_OFFSET
  xPos += HORIZONTAL_OFFSET;
  yPos += VERTICAL_OFFSET;

  // --- 5) Draw the large value ---
  display.setFont(font);
  display.setCursor(xPos, yPos);
  display.setTextColor(SH110X_WHITE);
  display.print(valStr);

  // --- 6) Draw the unit ---
  // Place the unit after the large text: x + bw + spacing
  // Note: correct for offsets (ux1, uy1) so the smaller text
  //       bounding box starts at the right position
  int16_t unitX = xPos + bw + spacing - ux1;
  int16_t unitY = yPos + (by1 - uy1); 
    // by1 - uy1 helps align them on the same baseline.
    // Increase or decrease it to shift the unit up/down.

  display.setFont(unitFont);
  display.setCursor(unitX, unitY);
  display.print(unit);

  display.display();
}
