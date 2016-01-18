/*
 * USB RetroPad Adapter - PC/PS3 USB adapter for retro-controllers!
 * Copyright (c) 2012 Bruno Freitas - bruno@brunofreitas.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <WProgram.h>
#include "XBOXPad.h"
#include "../PS2Pad.h"
#include "../genesis.h"
#include "../saturn.h"
#include "../NESPad.h"
#include "../GCPad_16Mhz.h"
#include "../tg16.h"

// Arcade mode detection pin
#define ARCADE_DB9_PIN	12

// Extension cable detection pins
#define DETPIN0 6  // DB9P2
#define DETPIN1	8  // DB9P4
#define DETPIN2	9  // DB9P6
#define DETPIN3	10 // DB9P7
#define DETPIN4	11 // DB9P9

// Possible values (as of today) returned by the detectPad() routine
// Normal pads
#define PAD_ARCADE		-1
#define PAD_GENESIS		0b00111
#define PAD_NES 		0b00110
#define PAD_SNES 		0b00101
#define PAD_PS2 		0b00100
#define PAD_GC	 		0b00011
#define PAD_N64			0b00010
#define PAD_NEOGEO		0b00001
#define PAD_WIICC		0b00000
// Extended pads (uses DB9 pin 4 and/or 2 for identification)
#define PAD_SATURN		0b01111
#define PAD_TG16		0b10111
#define PAD_DFU_DONGLE	0b01110 // Reserved for USBRA DFU dongle

/*
 * This is the new auto-detect function (non jumper based) which detects the extension
 * cable plugged in the DB9 port. It uses grounded pins from DB9 (4, 6, 7 and 9) for
 * the detection.
 *
 *  -1 - Arcade
 * 00111 - Sega Genesis (Default)
 * 00110 - NES
 * 00101 - SNES
 * 00100 - PS2
 * 00011 - Game Cube
 * 00010 - Nintendo 64
 * 00001 - Neo Geo
 * 00000 - Reserved 1
 * 01111 - Sega Saturn
 * 10111 - TurboGrafx 16
 */
int detectPad() {
	int pad;

	// Set pad/arcade detection pins as input, turning pull-ups on
	pinMode(DETPIN0, INPUT);
	digitalWrite(DETPIN0, HIGH);

	pinMode(DETPIN1, INPUT);
	digitalWrite(DETPIN1, HIGH);

	pinMode(DETPIN2, INPUT);
	digitalWrite(DETPIN2, HIGH);

	pinMode(DETPIN3, INPUT);
	digitalWrite(DETPIN3, HIGH);

	pinMode(DETPIN4, INPUT);
	digitalWrite(DETPIN4, HIGH);

	pinMode(ARCADE_DB9_PIN, INPUT);
	digitalWrite(ARCADE_DB9_PIN, HIGH);

	// Check switch for Arcade position
	if(digitalRead(ARCADE_DB9_PIN))
		return PAD_ARCADE;

	pad = (!digitalRead(DETPIN0) << 4) | (!digitalRead(DETPIN1) << 3) | (digitalRead(DETPIN2) << 2) | (digitalRead(DETPIN3) << 1) | (digitalRead(DETPIN4));

	if((pad >> 3) & 0b11) {
		switch(pad) {
		case 0b11011:
		case 0b10111:
			return PAD_TG16;
			break;
		case 0b11111:
		case 0b01111:
			return PAD_SATURN;
			break;
		case 0b11100:
			return PAD_PS2;
			break;
		default:
			return PAD_GENESIS;
			break;
		}
	}

	return (pad & 0b111);
}


void setup() {
	// Initialize USB joystick driver
	xbox_init(true);

	// Delay for waiting XBOX pad to be detected...
	while(!xbox_pad_detected()) {
		xbox_reset_watchdog();
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}
}

void genesis_loop() {
	int button_data;

	genesis_init();

	for (;;) {

		xbox_reset_watchdog();

		button_data = genesis_read();

		(button_data & GENESIS_UP)    ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & GENESIS_DOWN)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & GENESIS_LEFT)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & GENESIS_RIGHT) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.a = ((button_data & GENESIS_A) > 0) * 0xFF;

		gamepad_state.b = ((button_data & GENESIS_B) > 0) * 0xFF;

		gamepad_state.black = ((button_data & GENESIS_C) > 0) * 0xFF;

		gamepad_state.x = ((button_data & GENESIS_X) > 0) * 0xFF;

		gamepad_state.y = ((button_data & GENESIS_Y) > 0) * 0xFF;

		gamepad_state.white = ((button_data & GENESIS_Z) > 0) * 0xFF;

		(button_data & GENESIS_MODE) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);

		(button_data & GENESIS_START) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		((button_data & GENESIS_UP) && (button_data & GENESIS_START)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void arcade_loop() {
	int button_data;

	NESPad::init(6, 7, 13);

	for (;;) {

		xbox_reset_watchdog();

		button_data = NESPad::read(16);

		(button_data & 0x01) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & 0x02) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & 0x04) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & 0x08) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.x = ((button_data & 0x10) > 0) * 0xFF;

		gamepad_state.a = ((button_data & 0x20) > 0) * 0xFF;

		gamepad_state.y = ((button_data & 0x40) > 0) * 0xFF;

		gamepad_state.b = ((button_data & 0x80) > 0) * 0xFF;

		gamepad_state.white = ((button_data & 0x100) > 0) * 0xFF;

		gamepad_state.black = ((button_data & 0x200) > 0) * 0xFF;

		gamepad_state.l = ((button_data & 0x400) > 0) * 0xFF;

		gamepad_state.r = ((button_data & 0x800) > 0) * 0xFF;

		((button_data & 0x1000) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);

		((button_data & 0x2000) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		((button_data & 0x4000) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_LEFT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_LEFT_STICK);

		((button_data & 0x8000) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void nes_loop() {
	int button_data;

	NESPad::init(5, 6, 7);

	for (;;) {

		xbox_reset_watchdog();

		button_data = NESPad::read(8);

		(button_data & 16)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & 32)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & 64)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & 128) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.b = ((button_data & 2) > 0) * 0xFF;

		gamepad_state.a = ((button_data & 1) > 0) * 0xFF;

		((button_data & 4) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);

		((button_data & 8) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		((button_data & 4) && (button_data & 8)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void snes_loop() {
	int button_data;

	NESPad::init(5, 6, 7);

	for (;;) {

		xbox_reset_watchdog();

		button_data = NESPad::read(16);

		(button_data & 16)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & 32)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & 64)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & 128) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.a = ((button_data & 1) > 0) * 0xFF;
		gamepad_state.b = ((button_data & 256) > 0) * 0xFF;
		gamepad_state.x = ((button_data & 2) > 0) * 0xFF;
		gamepad_state.y = ((button_data & 512) > 0) * 0xFF;

		gamepad_state.l = ((button_data & 1024) > 0) * 0xFF;
		gamepad_state.r = ((button_data & 2048) > 0) * 0xFF;

		((button_data & 4) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);

		((button_data & 8) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		((button_data & 4) && (button_data & 8)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void ps2_loop() {
	byte dir = 0;

	while (PS2Pad::init(true)) {
		xbox_reset_watchdog();
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}

	for (;;) {

		xbox_reset_watchdog();

		PS2Pad::read();

		if(PS2Pad::type() == 0) { // Digital Pad
			gamepad_state.l_x = 0;
			gamepad_state.l_y = 0;
			gamepad_state.r_x = 0;
			gamepad_state.r_y = 0;
		} else {
			gamepad_state.l_x = map(PS2Pad::stick(PSS_LX), 0, 255, -32768, 32767);
			gamepad_state.l_y = map(PS2Pad::stick(PSS_LY), 0, 255, 32767, -32768);
			gamepad_state.r_x = map(PS2Pad::stick(PSS_RX), 0, 255, -32768, 32767);
			gamepad_state.r_y = map(PS2Pad::stick(PSS_RY), 0, 255, 32767, -32768);
		}


		PS2Pad::button(PSB_PAD_UP)    ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		PS2Pad::button(PSB_PAD_DOWN)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		PS2Pad::button(PSB_PAD_LEFT)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		PS2Pad::button(PSB_PAD_RIGHT) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.x = PS2Pad::button(PSB_SQUARE) * 0xFF;
		gamepad_state.y = PS2Pad::button(PSB_TRIANGLE) * 0xFF;
		gamepad_state.a = PS2Pad::button(PSB_CROSS) * 0xFF;
		gamepad_state.b = PS2Pad::button(PSB_CIRCLE) * 0xFF;

		PS2Pad::button(PSB_START) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);
		PS2Pad::button(PSB_SELECT) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);
		PS2Pad::button(PSB_L3) ? bitSet(gamepad_state.digital_buttons, XBOX_LEFT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_LEFT_STICK);
		PS2Pad::button(PSB_R3) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		gamepad_state.l = PS2Pad::button(PSB_L2) * 0xFF;
		gamepad_state.r = PS2Pad::button(PSB_R2) * 0xFF;

		gamepad_state.white = PS2Pad::button(PSB_L1) * 0xFF;
		gamepad_state.black = PS2Pad::button(PSB_R1) * 0xFF;

		xbox_send_pad_state();
	}
}

void gc_loop() {
	byte *button_data;
	byte dir = 0;

	while(GCPad_init() == 0) {
		xbox_reset_watchdog();
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}

	for(;;) {

		xbox_reset_watchdog();

		button_data = GCPad_read();

		((button_data[1] & 0x08) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		((button_data[1] & 0x04) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		((button_data[1] & 0x01) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		((button_data[1] & 0x02) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.x = ((button_data[0] & 0x08) > 0) * 0xFF;

		gamepad_state.a = ((button_data[0] & 0x02) > 0) * 0xFF;

		gamepad_state.y = ((button_data[0] & 0x04) > 0) * 0xFF;

		gamepad_state.b = ((button_data[0] & 0x01) > 0) * 0xFF;

		((button_data[0] & 0x10) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		gamepad_state.l = ((button_data[1] & 0x40) > 0) * 0xFF;

		gamepad_state.r = ((button_data[1] & 0x20) > 0) * 0xFF;

		gamepad_state.black = ((button_data[1] & 0x10) > 0) * 0xFF;

		gamepad_state.l_x = map(button_data[2], 32, 223, -32768, 32767);
		gamepad_state.l_y = map(~button_data[3], 32, 223, 32767, -32768);
		gamepad_state.r_x = map(button_data[4], 32, 223, -32768, 32767);
		gamepad_state.r_y = map(~button_data[5], 32, 223, 32767, -32768);

		((button_data[1] & 0x08) && (button_data[0] & 0x10)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();

		delayMicroseconds(5000);
	}
}

void n64_loop() {
	byte *button_data;
	byte dir = 0;
	byte lx, ly;

	while(GCPad_init() == 0) {
		xbox_reset_watchdog();
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}

	for(;;) {

		xbox_reset_watchdog();

		button_data = N64Pad_read();

		((button_data[0] & 0x08) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		((button_data[0] & 0x04) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		((button_data[0] & 0x02) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		((button_data[0] & 0x01) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.x = ((button_data[0] & 0x40) > 0) * 0xFF;

		gamepad_state.a = ((button_data[0] & 0x80) > 0) * 0xFF;

		((button_data[0] & 0x10) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		gamepad_state.l = ((button_data[1] & 0x20) > 0) * 0xFF;

		gamepad_state.r = ((button_data[1] & 0x10) > 0) * 0xFF;

		gamepad_state.black = ((button_data[0] & 0x20) > 0) * 0xFF;

		lx = ((button_data[2] >= 128) ? button_data[2] - 128 : button_data[2] + 128);
		ly = ~((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128);

		gamepad_state.l_x = map(lx, 32, 223, -32768, 32767);
		gamepad_state.l_y = map(ly, 32, 223, 32767, -32768);

		gamepad_state.r_x = 0x00;
		gamepad_state.r_y = 0x00;

		if(button_data[1] & 0x08) { // C Up
			gamepad_state.r_y = 32767;
		} else if(button_data[1] & 0x04) { // C Down
			gamepad_state.r_y = -32768;
		}

		if(button_data[1] & 0x02) { // C Left
			gamepad_state.r_x = -32768;
		} else if(button_data[1] & 0x01) { // C Right
			gamepad_state.r_x = 32767;
		}

		((button_data[0] & 0x08) && (button_data[0] & 0x10)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void neogeo_loop() {
	int button_data;

	NESPad::init(5, 6, 7);

	for (;;) {

		xbox_reset_watchdog();

		button_data = NESPad::read(16);

		(button_data & 0x04)   ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & 0x1000) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & 0x02)   ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & 0x800) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.x = ((button_data & 0x8000) > 0) * 0xFF;

		gamepad_state.a = ((button_data & 0x01) > 0) * 0xFF;

		gamepad_state.b = ((button_data & 0x400) > 0) * 0xFF;

		gamepad_state.y = ((button_data & 0x200) > 0) * 0xFF; // D button is also 0x2000

		((button_data & 0x100) > 0)  ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);

		((button_data & 0x4000) > 0) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		((button_data & 0x100) && (button_data & 0x4000)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void saturn_loop() {
	int button_data;

	saturn_init();

	for (;;) {

		xbox_reset_watchdog();

		button_data = saturn_read();

		(button_data & SATURN_UP)    ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & SATURN_DOWN)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & SATURN_LEFT)  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & SATURN_RIGHT) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.a = ((button_data & SATURN_A) > 0) * 0xFF;

		gamepad_state.b = ((button_data & SATURN_B) > 0) * 0xFF;

		gamepad_state.black = ((button_data & SATURN_C) > 0) * 0xFF;

		gamepad_state.x = ((button_data & SATURN_X) > 0) * 0xFF;

		gamepad_state.y = ((button_data & SATURN_Y) > 0) * 0xFF;

		gamepad_state.white = ((button_data & SATURN_Z) > 0) * 0xFF;
		
		gamepad_state.l = ((button_data & SATURN_L) > 0) * 0xFF;

		gamepad_state.r = ((button_data & SATURN_R) > 0) * 0xFF;

		(button_data & SATURN_START) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		((button_data & SATURN_UP) && (button_data & SATURN_START)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void tg16_loop(void) {
	int button_data;

	tg16_init();

	for (;;) {
		xbox_reset_watchdog();

		button_data = tg16_read();

		(button_data & (1 << TG16_UP))    ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & (1 << TG16_DOWN))  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & (1 << TG16_LEFT))  ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & (1 << TG16_RIGHT)) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

		gamepad_state.a = ((button_data & (1 << TG16_I)) > 0) * 0xFF;

		gamepad_state.x = ((button_data & (1 << TG16_II)) > 0) * 0xFF;

		(button_data & (1 << TG16_RUN)) ? bitSet(gamepad_state.digital_buttons, XBOX_START) : bitClear(gamepad_state.digital_buttons, XBOX_START);

		(button_data & (1 << TG16_SELECT)) ? bitSet(gamepad_state.digital_buttons, XBOX_BACK) : bitClear(gamepad_state.digital_buttons, XBOX_BACK);

		((button_data & (1 << TG16_RUN)) && (button_data & (1 << TG16_SELECT))) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}


void unsupported_pad(void) {
	for(;;);
}

void loop() {
	switch (detectPad()) {
	case PAD_ARCADE:
		arcade_loop();
		break;
	case PAD_NES:
		nes_loop();
		break;
	case PAD_SNES:
		snes_loop();
		break;
	case PAD_PS2:
		ps2_loop();
		break;
	case PAD_GC:
		gc_loop();
		break;
	case PAD_N64:
		n64_loop();
		break;
	case PAD_NEOGEO:
		neogeo_loop();
		break;
	case PAD_SATURN:
		saturn_loop();
		break;
	case PAD_TG16:
		tg16_loop();
		break;
	case PAD_WIICC:
		unsupported_pad();
		break;
	default:
		genesis_loop();
		break;
	}
}
