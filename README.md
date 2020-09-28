# Arduino LCD I2C library for HD47780

This library is for the HD47780 LCDs with I2C interface for use with Arduino.
The most difference to the original LiquidCrystal library from Arduino is a new API and this library supports *printf()* functionality.<br>
When using PlatformIO, to support *float* you must add following line in platformio.ini for AVR devices
```
#build_flags = -Wl,-u,vfprintf -lprintf_flt -lm
```


## Installation

1. "Download":https://github.com/sstaub/LCD/archive/master.zip the Master branch from GitHub.
2. Unzip and modify the folder name to "LCDi2c"
3. Move the modified folder on your Library folder.


## Example
Here is a simple example which shows the capabilities of the display 
```cpp
// example for Arduino LCD library

#include "Arduino.h"
#include "LCDi2c.h"

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
```

# Documentation

## Constructor
**LCD(uint8_t address)**
create a LCD object with a given address of the I2C interface

## Functions

### **void begin(uint8_t columns = 16, uint8_t rows = 2, uint8_t dotsize = LCD_5x8DOTS)**
initialize the display, you can determine the row and column number of the display and the dotsize

### **void cls()**
clear the screen, set cursor to home

### **void display(modes_t display)**
set the modes of the display

- DISPLAY_ON Turn the display on
- DISPLAY_OFF Turn the display off
- CURSOR_ON Turns the underline cursor on
- CURSOR_OFF Turns the underline cursor off
- BLINK_ON Turn the blinking cursor on
- BLINK_OFF Turn the blinking cursor off
- SCROLL_LEFT These command scroll the display without changing the RAM
- SCROLL_RIGHT These commands scroll the display without changing the RAM
- LEFT_TO_RIGHT This is for text that flows Left to Right
- RIGHT_TO_LEFT This is for text that flows Right to Left
- AUTOSCROLL_ON This will 'right justify' text from the cursor
- AUTOSCROLL_OFF This will 'left justify' text from the cursor
- BACKLIGHT_ON This will turn the Backlight ON
- BACKLIGHT_OFF This will turn the Backlight OFF

### **void locate(uint8_t column, uint8_t row)**
set poition of the cursor

### **void home()**
set cursor to home position (0/0)

### **void create(uint8_t location, uint8_t charmap[])**
Create a user defined char object allows us to fill the first 8 CGRAM locations with custom characters

### **void character(uint8_t column, uint8_t row, uint8_t c)**
draw a single character on given position, usefull for user chars
