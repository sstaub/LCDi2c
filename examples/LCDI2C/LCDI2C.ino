#include "Arduino.h"
#include <LCDi2c.h>

#define LCD_CHARS   16
#define LCD_LINES   2

// special chars
uint8_t upArrow[8] = {  
	0b00100,
	0b01010,
	0b10001,
	0b00100,
	0b00100,
	0b00100,
	0b00000,
	};

uint8_t downArrow[8] = {
	0b00000,
	0b00100,
	0b00100,
	0b00100,
	0b10001,
	0b01010,
	0b00100,
	};

uint8_t rightArrow[8] = {
	0b00000,
	0b00100,
	0b00010,
	0b11001,
	0b00010,
	0b00100,
	0b00000,
	};

uint8_t leftArrow[8] = {
	0b00000,
	0b00100,
	0b01000,
	0b10011,
	0b01000,
	0b00100,
	0b00000,
	};

float data = 0.1f;

LCDi2c lcd(0x27); // I2C address

void setup() {
  lcd.begin(LCD_CHARS, LCD_LINES);
  lcd.create(0, downArrow);
	lcd.create(1, upArrow);
	lcd.create(2, rightArrow);
	lcd.create(3, leftArrow);

	lcd.cls();
	lcd.locate(0, 0);
	lcd.printf("hello world %f", data);
	// print user chars
	lcd.character(0, 1, 0);
	lcd.character(2, 1, 1);
	lcd.character(4, 1, 2);
	lcd.character(6, 1, 3);

	delay(2000);
	lcd.display(DISPLAY_OFF);
	delay(2000);
	lcd.display(DISPLAY_ON);
	delay(2000);
	lcd.display(BACKLIGHT_OFF);
	delay(2000);
	lcd.display(BACKLIGHT_ON);
	delay(2000);
	lcd.display(CURSOR_ON);
	delay(2000);
	lcd.display(BLINK_ON);
	delay(2000);
	lcd.display(BLINK_OFF);
	delay(2000);
	lcd.display(CURSOR_OFF);
  delay(2000);
  }

void loop() {
  for (uint8_t pos = 0; pos < 13; pos++) {
		// scroll one position to left
		lcd.display(SCROLL_LEFT);
		// step time
		delay(500);
		}

	// scroll 29 positions (string length + display length) to the right
	// to move it offscreen right
	for (uint8_t pos = 0; pos < 29; pos++) {
		// scroll one position to right
		lcd.display(SCROLL_RIGHT);
		// step time
		delay(500);
		}

	// scroll 16 positions (display length + string length) to the left
	// to move it back to center
	for (uint8_t pos = 0; pos < 16; pos++) {
		// scroll one position to left
		lcd.display(SCROLL_LEFT);
		// step time
		delay(500);
		}

	delay(1000);
  }
