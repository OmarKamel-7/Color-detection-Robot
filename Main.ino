#include <Wire.h>

// Raspberry Pi Pico I2C0 wiring:
// OLED SDA -> GP4, OLED SCK/SCL -> GP5
const uint8_t OLED_ADDRESS = 0x3C;
const uint8_t OLED_WIDTH = 128;
const uint8_t OLED_PAGES = 8; // 64 pixels / 8

void oledCommand(uint8_t command) {
  Wire.beginTransmission(OLED_ADDRESS);
  Wire.write(0x00); // command control byte
  Wire.write(command);
  Wire.endTransmission();
}

void oledInit() {
  delay(100);
  oledCommand(0xAE); // display off
  oledCommand(0xD5); oledCommand(0x80); // clock
  oledCommand(0xA8); oledCommand(0x3F); // multiplex = 64 rows
  oledCommand(0xD3); oledCommand(0x00); // display offset
  oledCommand(0x40); // start line 0
  oledCommand(0x8D); oledCommand(0x14); // charge pump on
  oledCommand(0x20); oledCommand(0x00); // horizontal addressing
  oledCommand(0xA1); // segment remap
  oledCommand(0xC8); // scan direction
  oledCommand(0xDA); oledCommand(0x12); // 128x64 COM pins
  oledCommand(0x81); oledCommand(0x7F); // contrast
  oledCommand(0xD9); oledCommand(0xF1); // pre-charge
  oledCommand(0xDB); oledCommand(0x40); // VCOM detect
  oledCommand(0xA4); // resume RAM display
  oledCommand(0xA6); // normal display
  oledCommand(0xAF); // display on
}

void oledPattern(bool checkerboard) {
  for (uint8_t page = 0; page < OLED_PAGES; page++) {
    oledCommand(0xB0 | page); // page address
    oledCommand(0x00);         // lower column
    oledCommand(0x10);         // upper column

    Wire.beginTransmission(OLED_ADDRESS);
    Wire.write(0x40); // data control byte
    for (uint8_t column = 0; column < OLED_WIDTH; column++) {
      uint8_t value;
      if (checkerboard) {
        value = ((column / 8 + page) % 2) ? 0xAA : 0x55;
      } else {
        value = 0xFF;
      }
      Wire.write(value);
    }
    Wire.endTransmission();
  }
}

void setup() {
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();
  oledInit();
}

void loop() {
  // Alternating patterns confirm that the OLED is receiving I2C data.
  oledPattern(true);
  delay(2000);
  oledPattern(false);
  delay(2000);
}
