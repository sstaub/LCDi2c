/* Arduino TextLCD Library, for a 4-bit LCD based on HD44780
 * Copyright (c) 2020, sstaub
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "LCDi2c.h"

LCDi2c::LCDi2c(uint8_t address) {
	this->address = address;
	backlight = LCD_BACKLIGHT_ON;
	}

void LCDi2c::begin(uint8_t columns, uint8_t rows, uint8_t dotsize) {
	Wire.begin();
	this->rows = rows;
	this->columns = columns;
	this->dotsize = dotsize;
	setRowOffsets(0x00, 0x40, 0x00 + columns, 0x40 + columns);
	displayfunction = LCD_4BIT_MODE | LCD_1_LINE | LCD_5x8DOTS;
	if (rows > 1) {
		displayfunction |= LCD_2_LINE;
	}
	if ((dotsize != 0) && (rows == 1)) {
		displayfunction |= LCD_5x10DOTS;
	}
	_delay_ms(50); 
	expanderWrite(backlight);
  write4bits(0x03 << 4);
  _delay_us(4500);
  write4bits(0x03 << 4);
  _delay_us(4500);
  write4bits(0x03 << 4); 
  _delay_us(150);
  write4bits(0x02 << 4); 
	command(LCD_FUNCTION_SET | displayfunction);  
	displaycontrol = LCD_DISPLAY_ON | LCD_CURSOR_OFF | LCD_BLINK_OFF;
	display(DISPLAY_ON);
	display(CURSOR_OFF);
	display(BLINK_OFF);
	cls();
	displaymode = LCD_ENTRY_LEFT | LCD_ENTRY_SHIFT_DECREMENT;
	command(LCD_ENTRY_MODE_SET | displaymode);
	home();
	}

void LCDi2c::setRowOffsets(int row0, int row1, int row2, int row3) {
	rowOffsets[0] = row0;
	rowOffsets[1] = row1;
	rowOffsets[2] = row2;
	rowOffsets[3] = row3;
	}

void LCDi2c::cls() {
	command(LCD_CLEAR_DISPLAY);
	_delay_us(2000);
	}

void LCDi2c::home() {
	command(LCD_RETURN_HOME);
	_delay_us(2000);
	}

void LCDi2c::locate(uint8_t column, uint8_t row) {
	const size_t max_rows = sizeof(rowOffsets) / sizeof(*rowOffsets);
	if (row >= max_rows) row = max_rows - 1;
	if (row >= rows) row = rows - 1;
	command(LCD_SET_DDRAM_ADDR | (column + rowOffsets[row]));
	}

void LCDi2c::display(mode_t mode) {
	switch(mode) {
		case DISPLAY_ON :
			displaycontrol |= LCD_DISPLAY_ON;
			command(LCD_DISPLAY_CONTROL | displaycontrol);
			break;
		case DISPLAY_OFF:
			displaycontrol &= ~LCD_DISPLAY_ON;
			command(LCD_DISPLAY_CONTROL | displaycontrol);
			break;
		case CURSOR_ON:
			displaycontrol |= LCD_CURSOR_ON;
			command(LCD_DISPLAY_CONTROL | displaycontrol);
			break;
		case CURSOR_OFF:
			displaycontrol &= ~LCD_CURSOR_ON;
			command(LCD_DISPLAY_CONTROL | displaycontrol);
			break;
		case BLINK_ON:
			displaycontrol |= LCD_BLINK_ON;
			command(LCD_DISPLAY_CONTROL | displaycontrol);
			break;
		case BLINK_OFF:
			displaycontrol &= ~LCD_BLINK_ON;
			command(LCD_DISPLAY_CONTROL | displaycontrol);
			break;
		case SCROLL_LEFT:
			command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_LEFT);
			break;
		case SCROLL_RIGHT:
			command(LCD_CURSOR_SHIFT | LCD_DISPLAY_MOVE | LCD_MOVE_RIGHT);
			break;
		case LEFT_TO_RIGHT:
			displaymode |= LCD_ENTRY_LEFT;
			command(LCD_ENTRY_MODE_SET | displaymode);
			break;
		case RIGHT_TO_LEFT:
			displaymode &= ~LCD_ENTRY_LEFT;
			command(LCD_ENTRY_MODE_SET | displaymode);
			break;
		case AUTOSCROLL_ON:
			displaymode |= LCD_ENTRY_SHIFT_INCREMENT;
			command(LCD_ENTRY_MODE_SET | displaymode);
			break;
		case AUTOSCROLL_OFF:
			displaymode &= ~LCD_ENTRY_SHIFT_INCREMENT;
			command(LCD_ENTRY_MODE_SET | displaymode);
			break;
		case BACKLIGHT_ON:
			backlight = LCD_BACKLIGHT_ON;
			expanderWrite(0);
			break;
		case BACKLIGHT_OFF:
			backlight = LCD_BACKLIGHT_OFF;
			expanderWrite(0);
			break;
		}
	}

void LCDi2c::create(uint8_t location, uint8_t charmap[]) {
	location &= 0x7;
	command(LCD_SET_CGRAM_ADDR | (location << 3));
	for (int i=0; i<8; i++) {
		write(charmap[i]);
		}
	}

void LCDi2c::character(uint8_t column, uint8_t row, char c) {
	locate(column, row);
	write(c);
	}

void LCDi2c::printf(const char *format, ...) {
	char lcd_buffer[columns + 1];
	va_list args;
	va_start(args, format);
	vsnprintf(lcd_buffer, columns + 1, format, args);
	va_end(args);
	for (char *i = lcd_buffer; *i; i++) {
		write(*i);
		}
	}

void LCDi2c::command(uint8_t value) {
	send(value, 0);
	}

size_t LCDi2c::write(uint8_t value) {
	send(value, RS);
	return 1;
	}

void LCDi2c::send(uint8_t value, uint8_t mode) {
	uint8_t highnib = value & 0xf0;
	uint8_t lownib = (value << 4) & 0xf0;
	write4bits((highnib)|mode);
	write4bits((lownib)|mode);
	}

void LCDi2c::pulseEnable(uint8_t value) {
	expanderWrite(value & ~EN);	// En low
	_delay_us(1);	// commands need > 37us to settle
	expanderWrite(value | EN);	// En high
	_delay_us(1);	// enable pulse must be >450ns
	expanderWrite(value & ~EN);	// En low
	_delay_us(40);// commands need > 37us to settle
	}

void LCDi2c::write4bits(uint8_t value) {
	expanderWrite(value);
	pulseEnable(value);
	}

void LCDi2c::expanderWrite(uint8_t data) {
	Wire.beginTransmission(address);
	Wire.write((int)(data) | backlight);
	Wire.endTransmission();
}