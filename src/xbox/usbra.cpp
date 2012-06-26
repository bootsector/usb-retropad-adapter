/*
 * USB RetroPad Adapter - PC/PS3 USB adapter for retro-controllers!
 * Copyright (c) 2012 Bruno Freitas - bootsector@ig.com.br
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

// Arcade mode detection pin
#define ARCADE_DB9_PIN	12

// Extension cable detection pins
#define DETPIN0	8
#define DETPIN1	9
#define DETPIN2	10
#define DETPIN3	11

// Possible values (as of today) returned by the detectPad() routine
// Normal pads
#define PAD_ARCADE		-1
#define PAD_GENESIS		0b0111
#define PAD_NES 		0b0110
#define PAD_SNES 		0b0101
#define PAD_PS2 		0b0100
#define PAD_GC	 		0b0011
#define PAD_N64			0b0010
#define PAD_NEOGEO		0b0001
#define PAD_RESERVED1	0b0000
// Extended pads (uses DB9 pin 4 for identification)
#define PAD_SATURN		0b1111
#define PAD_DO_NOT_USE	0b1100 // 3 LSB overlaps with PS2 pad, which uses DB9 pin 4 for CLK.

/*
 * This is the new auto-detect function (non jumper based) which detects the extension
 * cable plugged in the DB9 port. It uses grounded pins from DB9 (4, 6, 7 and 9) for
 * the detection.
 *
 *  -1 - Arcade
 * 0111 - Sega Genesis (Default)
 * 0110 - NES
 * 0101 - SNES
 * 0100 - PS2
 * 0011 - Game Cube
 * 0010 - Nintendo 64
 * 0001 - Neo Geo
 * 0000 - Reserved 1
 * 1111 - Sega Saturn
 */
int detectPad() {
	int pad;

	// Set pad/arcade detection pins as input, turning pull-ups on
	pinMode(DETPIN1, INPUT);
	digitalWrite(DETPIN1, HIGH);

	pinMode(DETPIN2, INPUT);
	digitalWrite(DETPIN2, HIGH);

	pinMode(DETPIN3, INPUT);
	digitalWrite(DETPIN3, HIGH);

	pinMode(ARCADE_DB9_PIN, INPUT);
	digitalWrite(ARCADE_DB9_PIN, HIGH);

	// Check switch for Arcade position
	if(digitalRead(ARCADE_DB9_PIN))
		return PAD_ARCADE;

	// Read extension detection pins statuses
	pad = (digitalRead(DETPIN1) << 2) | (digitalRead(DETPIN2) << 1) | (digitalRead(DETPIN3));

	// Check if pad is not PS2 pad, that uses DB9 pin 4.
	// If not, then use pin 4 for additional pads
	if(pad != PAD_PS2) {
		pinMode(DETPIN0, INPUT);
		digitalWrite(DETPIN0, HIGH);

		pad |= ((!digitalRead(DETPIN0)) << 3);

		digitalWrite(DETPIN0, LOW);
	}

	return pad;
}


void setup() {
	// Initialize USB joystick driver
	xbox_init(false);

	// Delay for waiting XBOX pad to be detected...
	for(int i = 0; i < 100; i++) {
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}
}

void genesis_loop() {
	int button_data;

	genesis_init();

	for (;;) {

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

	while (PS2Pad::init()) {
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}

	for (;;) {

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
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}

	for(;;) {

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

		gamepad_state.l_x = map(button_data[2], 0, 255, -32768, 32767);
		gamepad_state.l_y = map(~button_data[3], 0, 255, 32767, -32768);
		gamepad_state.r_x = map(button_data[4], 0, 255, -32768, 32767);
		gamepad_state.r_y = map(~button_data[5], 0, 255, 32767, -32768);

		((button_data[1] & 0x08) && (button_data[0] & 0x10)) ? bitSet(gamepad_state.digital_buttons, XBOX_RIGHT_STICK) : bitClear(gamepad_state.digital_buttons, XBOX_RIGHT_STICK);

		xbox_send_pad_state();
	}
}

void n64_loop() {
	byte *button_data;
	byte dir = 0;
	byte lx, ly;

	while(GCPad_init() == 0) {
		delayMicroseconds(10000); // 10ms delay
		xbox_send_pad_state();
	}

	for(;;) {

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

		gamepad_state.l_x = map(lx, 0, 255, -32768, 32767);
		gamepad_state.l_y = map(ly, 0, 255, 32767, -32768);

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

		button_data = NESPad::read(16);

		(button_data & 0x03)   ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_UP)    : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_UP);
		(button_data & 0x1000) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_DOWN)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_DOWN);
		(button_data & 0x02)   ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_LEFT)  : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_LEFT);
		(button_data & 0x8008) ? bitSet(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT) : bitClear(gamepad_state.digital_buttons, XBOX_DPAD_RIGHT);

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
	default:
		genesis_loop();
		break;
	}
}
