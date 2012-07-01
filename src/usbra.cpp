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
#include "USBVirtuaStick.h"
#include "PS2Pad.h"
#include "genesis.h"
#include "saturn.h"
#include "NESPad.h"
#include "GCPad_16Mhz.h"

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
#define PAD_DFU_DONGLE	0b1110 // Reserved for USBRA DFU dongle
#define PAD_DO_NOT_USE	0b1100 // 3 LSB overlaps with PS2 pad, which uses DB9 pin 4 for CLK.

// Pad directions vector
byte pad_dir[16] = {8, 2, 6, 8, 4, 3, 5, 8, 0, 1, 7, 8, 8, 8, 8, 8};

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
	vs_init(false);
}

void genesis_loop() {
	int button_data;

	genesis_init();

	for (;;) {
		//vs_reset_watchdog();

		button_data = genesis_read();

		gamepad_state.l_x_axis = 0x80;
		gamepad_state.l_y_axis = 0x80;

		if(button_data & GENESIS_LEFT) {
			gamepad_state.l_x_axis = 0x00;
		} else if (button_data & GENESIS_RIGHT) {
			gamepad_state.l_x_axis = 0xFF;
		}

		if(button_data & GENESIS_UP) {
			gamepad_state.l_y_axis = 0x00;
		} else if (button_data & GENESIS_DOWN) {
			gamepad_state.l_y_axis = 0xFF;
		}

		gamepad_state.square_btn = (button_data & GENESIS_A) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data & GENESIS_B) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = (button_data & GENESIS_C) > 0;
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.l1_btn = (button_data & GENESIS_X) > 0;
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = (button_data & GENESIS_Y) > 0;
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = (button_data & GENESIS_Z) > 0;
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.select_btn = (button_data & GENESIS_MODE) > 0;

		gamepad_state.start_btn = (button_data & GENESIS_START) > 0;

		gamepad_state.ps_btn = (button_data & GENESIS_UP) && (button_data & GENESIS_START);

		vs_send_pad_state();
	}
}

void arcade_loop() {
	int button_data;

	NESPad::init(6, 7, 13);

	for (;;) {
		//vs_reset_watchdog();

		button_data = NESPad::read(16);

		gamepad_state.l_x_axis = 0x80;
		gamepad_state.l_y_axis = 0x80;

		if(button_data & 0x01) {
			gamepad_state.l_y_axis = 0x00;
		} else if (button_data & 0x02) {
			gamepad_state.l_y_axis = 0xFF;
		}


		if(button_data & 0x04) {
			gamepad_state.l_x_axis = 0x00;
		} else if (button_data & 0x08) {
			gamepad_state.l_x_axis = 0xFF;
		}

		gamepad_state.square_btn = (button_data & 0x10) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data & 0x20) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = (button_data & 0x40) > 0;
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = (button_data & 0x80) > 0;
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.l1_btn = (button_data & 0x100) > 0;
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = (button_data & 0x200) > 0;
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.l2_btn = (button_data & 0x400) > 0;
		gamepad_state.l2_axis = (gamepad_state.l2_btn ? 0xFF : 0x00);

		gamepad_state.r2_btn = (button_data & 0x800) > 0;
		gamepad_state.r2_axis = (gamepad_state.r2_btn ? 0xFF : 0x00);

		gamepad_state.select_btn = (button_data & 0x1000) > 0;

		gamepad_state.start_btn = (button_data & 0x2000) > 0;

		gamepad_state.l3_btn = (button_data & 0x4000) > 0;

		gamepad_state.ps_btn = (button_data & 0x8000) > 0;

		vs_send_pad_state();
	}
}

void nes_loop() {
	int button_data;

	NESPad::init(5, 6, 7);

	for (;;) {
		//vs_reset_watchdog();

		button_data = NESPad::read(8);

		gamepad_state.l_x_axis = 0x80;
		gamepad_state.l_y_axis = 0x80;

		if(button_data & 64) {
			gamepad_state.l_x_axis = 0x00;
		} else if (button_data & 128) {
			gamepad_state.l_x_axis = 0xFF;
		}

		if(button_data & 16) {
			gamepad_state.l_y_axis = 0x00;
		} else if (button_data & 32) {
			gamepad_state.l_y_axis = 0xFF;
		}

		gamepad_state.square_btn = (button_data & 2) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data & 1) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.select_btn = (button_data & 4) > 0;

		gamepad_state.start_btn = (button_data & 8) > 0;

		gamepad_state.ps_btn = (button_data & 4) && (button_data & 8); // SELECT + START = PS Button

		vs_send_pad_state();
	}
}

void snes_loop() {
	int button_data;

	NESPad::init(5, 6, 7);

	for (;;) {
		//vs_reset_watchdog();

		button_data = NESPad::read(16);

		gamepad_state.l_x_axis = 0x80;
		gamepad_state.l_y_axis = 0x80;

		if(button_data & 64) {
			gamepad_state.l_x_axis = 0x00;
		} else if (button_data & 128) {
			gamepad_state.l_x_axis = 0xFF;
		}

		if(button_data & 16) {
			gamepad_state.l_y_axis = 0x00;
		} else if (button_data & 32) {
			gamepad_state.l_y_axis = 0xFF;
		}

		gamepad_state.square_btn = (button_data & 2) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data & 1) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = (button_data & 256) > 0;
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.l1_btn = (button_data & 1024) > 0;
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = (button_data & 512) > 0;
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = (button_data & 2048) > 0;
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.select_btn = (button_data & 4) > 0;

		gamepad_state.start_btn = (button_data & 8) > 0;

		gamepad_state.ps_btn = (button_data & 4) && (button_data & 8); // SELECT + START = PS Button

		vs_send_pad_state();
	}
}

void ps2_loop() {
	byte dir = 0;

	while (PS2Pad::init()) {
		delayMicroseconds(10000); // 10ms delay
		vs_send_pad_state();
	}

	for (;;) {
		//vs_reset_watchdog();

		PS2Pad::read();

		if(PS2Pad::type() == 0) {
			gamepad_state.r_x_axis = 0x80;
			gamepad_state.r_y_axis = 0x80;

			if(PS2Pad::button(PSB_PAD_LEFT)) {
				gamepad_state.l_x_axis = 0x00;
			} else if (PS2Pad::button(PSB_PAD_RIGHT)) {
				gamepad_state.l_x_axis = 0xFF;
			} else {
				gamepad_state.l_x_axis = 0x80;
			}

			if(PS2Pad::button(PSB_PAD_UP)) {
				gamepad_state.l_y_axis = 0x00;
			} else if (PS2Pad::button(PSB_PAD_DOWN)) {
				gamepad_state.l_y_axis = 0xFF;
			} else {
				gamepad_state.l_y_axis = 0x80;
			}

		} else {
			gamepad_state.l_x_axis = PS2Pad::stick(PSS_LX);
			gamepad_state.l_y_axis = PS2Pad::stick(PSS_LY);
			gamepad_state.r_x_axis = PS2Pad::stick(PSS_RX);
			gamepad_state.r_y_axis = PS2Pad::stick(PSS_RY);

			dir = PS2Pad::button(PSB_PAD_UP) << 3 | PS2Pad::button(PSB_PAD_DOWN) << 2 | PS2Pad::button(PSB_PAD_LEFT) << 1 | PS2Pad::button(PSB_PAD_RIGHT);

			gamepad_state.direction = pad_dir[dir];
		}

		gamepad_state.square_btn = PS2Pad::button(PSB_SQUARE);
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = PS2Pad::button(PSB_CROSS);
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = PS2Pad::button(PSB_CIRCLE);
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.l1_btn =PS2Pad::button(PSB_L1);
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.l2_btn =PS2Pad::button(PSB_L2);
		gamepad_state.l2_axis = (gamepad_state.l2_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = PS2Pad::button(PSB_TRIANGLE);
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = PS2Pad::button(PSB_R1);
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.r2_btn = PS2Pad::button(PSB_R2);
		gamepad_state.r2_axis = (gamepad_state.r2_btn ? 0xFF : 0x00);

		gamepad_state.l3_btn = PS2Pad::button(PSB_L3);

		gamepad_state.r3_btn = PS2Pad::button(PSB_R3);

		gamepad_state.select_btn = PS2Pad::button(PSB_SELECT);

		gamepad_state.start_btn = PS2Pad::button(PSB_START);

		gamepad_state.ps_btn = PS2Pad::button(PSB_SELECT) && PS2Pad::button(PSB_START); // SELECT + START = PS Button

		vs_send_pad_state();
	}
}

void gc_loop() {
	byte *button_data;
	byte dir = 0;

	while(GCPad_init() == 0) {
		delayMicroseconds(10000); // 10ms delay
		vs_send_pad_state();
	}

	for(;;) {
		//vs_reset_watchdog();

		button_data = GCPad_read();

		dir = ((button_data[1] & 0x08) > 0) << 3 | ((button_data[1] & 0x04) > 0) << 2 | ((button_data[1] & 0x01) > 0) << 1 | ((button_data[1] & 0x02) > 0);

		gamepad_state.direction = pad_dir[dir];

		gamepad_state.square_btn = (button_data[0] & 0x08) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data[0] & 0x02) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = (button_data[0] & 0x04) > 0;
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = (button_data[0] & 0x01) > 0;
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.start_btn = (button_data[0] & 0x10) > 0;

		gamepad_state.l1_btn = (button_data[1] & 0x40) > 0;
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = (button_data[1] & 0x20) > 0;
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.l2_btn = (button_data[1] & 0x10) > 0;
		gamepad_state.l2_axis = (gamepad_state.l2_btn ? 0xFF : 0x00);

		gamepad_state.ps_btn = (button_data[1] & 0x08) && (button_data[0] & 0x10); // UP + START = PS button

		gamepad_state.l_x_axis = button_data[2];
		gamepad_state.l_y_axis = ~button_data[3];
		gamepad_state.r_x_axis = button_data[4];
		gamepad_state.r_y_axis = ~button_data[5];

		vs_send_pad_state();
	}
}

void n64_loop() {
	byte *button_data;
	byte dir = 0;

	while(GCPad_init() == 0) {
		delayMicroseconds(10000); // 10ms delay
		vs_send_pad_state();
	}

	for(;;) {
		//vs_reset_watchdog();

		button_data = N64Pad_read();

		dir = ((button_data[0] & 0x08) > 0) << 3 | ((button_data[0] & 0x04) > 0) << 2 | ((button_data[0] & 0x02) > 0) << 1 | ((button_data[0] & 0x01) > 0);

		gamepad_state.direction = pad_dir[dir];

		gamepad_state.square_btn = (button_data[0] & 0x40) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data[0] & 0x80) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.start_btn = (button_data[0] & 0x10) > 0;

		gamepad_state.l1_btn = (button_data[1] & 0x20) > 0;
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = (button_data[1] & 0x10) > 0;
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.l2_btn = (button_data[0] & 0x20) > 0;
		gamepad_state.l2_axis = (gamepad_state.l2_btn ? 0xFF : 0x00);

		gamepad_state.ps_btn = (button_data[0] & 0x08) && (button_data[0] & 0x10);  // UP + START = PS button

		gamepad_state.l_x_axis = ((button_data[2] >= 128) ? button_data[2] - 128 : button_data[2] + 128);
		gamepad_state.l_y_axis = ~((button_data[3] >= 128) ? button_data[3] - 128 : button_data[3] + 128);

		gamepad_state.r_x_axis = 0x80;
		gamepad_state.r_y_axis = 0x80;

		if(button_data[1] & 0x08) { // C Up
			gamepad_state.r_y_axis = 0x00;
		} else if(button_data[1] & 0x04) { // C Down
			gamepad_state.r_y_axis = 0xFF;
		}

		if(button_data[1] & 0x02) { // C Left
			gamepad_state.r_x_axis = 0x00;;
		} else if(button_data[1] & 0x01) { // C Right
			gamepad_state.r_x_axis = 0xFF;
		}

		vs_send_pad_state();
	}
}

void neogeo_loop() {
	int button_data;

	NESPad::init(5, 6, 7);

	for (;;) {
		//vs_reset_watchdog();

		button_data = NESPad::read(16);

		gamepad_state.l_x_axis = 0x80;
		gamepad_state.l_y_axis = 0x80;

		if(button_data & 0x02) {
			gamepad_state.l_x_axis = 0x00; // LEFT
		} else if (button_data & 0x800) {
			gamepad_state.l_x_axis = 0xFF; // RIGHT
		}

		if(button_data & 0x04) {
			gamepad_state.l_y_axis = 0x00; // UP
		} else if (button_data & 0x1000) {
			gamepad_state.l_y_axis = 0xFF; // DOWN
		}

		gamepad_state.square_btn = (button_data & 0x8000) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data & 0x01) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = (button_data & 0x400) > 0;
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = (button_data & 0x200) > 0; // D button is also 0x2000
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.select_btn = (button_data & 0x100) > 0;

		gamepad_state.start_btn = (button_data & 0x4000) > 0;

		gamepad_state.ps_btn = (button_data & 0x100) && (button_data & 0x4000); // SELECT + START = PS Button

		vs_send_pad_state();
	}
}

void saturn_loop() {
	int button_data;

	saturn_init();

	for (;;) {
		//vs_reset_watchdog();

		button_data = saturn_read();

		gamepad_state.l_x_axis = 0x80;
		gamepad_state.l_y_axis = 0x80;

		if(button_data & SATURN_LEFT) {
			gamepad_state.l_x_axis = 0x00;
		} else if (button_data & SATURN_RIGHT) {
			gamepad_state.l_x_axis = 0xFF;
		}

		if(button_data & SATURN_UP) {
			gamepad_state.l_y_axis = 0x00;
		} else if (button_data & SATURN_DOWN) {
			gamepad_state.l_y_axis = 0xFF;
		}

		gamepad_state.square_btn = (button_data & SATURN_A) > 0;
		gamepad_state.square_axis = (gamepad_state.square_btn ? 0xFF : 0x00);

		gamepad_state.cross_btn = (button_data & SATURN_B) > 0;
		gamepad_state.cross_axis = (gamepad_state.cross_btn ? 0xFF : 0x00);

		gamepad_state.circle_btn = (button_data & SATURN_C) > 0;
		gamepad_state.circle_axis = (gamepad_state.circle_btn ? 0xFF : 0x00);

		gamepad_state.l1_btn = (button_data & SATURN_X) > 0;
		gamepad_state.l1_axis = (gamepad_state.l1_btn ? 0xFF : 0x00);

		gamepad_state.triangle_btn = (button_data & SATURN_Y) > 0;
		gamepad_state.triangle_axis = (gamepad_state.triangle_btn ? 0xFF : 0x00);

		gamepad_state.r1_btn = (button_data & SATURN_Z) > 0;
		gamepad_state.r1_axis = (gamepad_state.r1_btn ? 0xFF : 0x00);

		gamepad_state.l2_btn = (button_data & SATURN_L) > 0;
		gamepad_state.l2_axis = (gamepad_state.l2_btn ? 0xFF : 0x00);

		gamepad_state.r2_btn = (button_data & SATURN_R) > 0;
		gamepad_state.r2_axis = (gamepad_state.r2_btn ? 0xFF : 0x00);

		gamepad_state.start_btn = (button_data & SATURN_START) > 0;

		gamepad_state.ps_btn = (button_data & SATURN_UP) && (button_data & SATURN_START);

		vs_send_pad_state();
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
