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
#include "PS2X_lib.h"
#include "genesis.h"
#include "NESPad.h"
#include "GCPad_16Mhz.h"
#include "digitalWriteFast.h"

// Arcade mode and extension cable detection pins
#define ARCADE_DB9_PIN	12
#define DETPIN1	9
#define DETPIN2	10
#define DETPIN3	11

// Possible values (as of today) returned by the detectPad() routine
#define PAD_ARCADE	-1
#define PAD_GENESIS	0b111
#define PAD_NES 	0b110
#define PAD_SNES 	0b101
#define PAD_PS2 	0b100
#define PAD_GC	 	0b011
#define PAD_N64		0b010

// Pad directions vector
byte pad_dir[16] = {8, 2, 6, 8, 4, 3, 5, 8, 0, 1, 7, 8, 8, 8, 8, 8};

/*
 * This is the new auto-detect function (non jumper based) which detects the extension
 * cable plugged in the DB9 port. It uses last 3 data pins from DB9 (6, 7 and 9) for
 * the detection.
 *
 *  -1 - Arcade
 * 111 - Sega Genesis (Default)
 * 110 - NES
 * 101 - SNES
 * 100 - PS2
 * 011 - Game Cube
 * 010 - Nintendo 64
 */
int detectPad() {
	int pad;

	if(!digitalReadFast(ARCADE_DB9_PIN))
		return PAD_ARCADE;

	pad = (digitalReadFast(DETPIN1) << 2) | digitalReadFast(DETPIN2) << 1 | digitalReadFast(DETPIN3);

	return pad;
}

void setup() {
	// Initialize USB joystick driver
	vs_init(false);

	// Set pad/arcade detection pins as input, turning pull-ups on
	pinModeFast(DETPIN1, INPUT);
	digitalWriteFast(DETPIN1, HIGH);

	pinModeFast(DETPIN2, INPUT);
	digitalWriteFast(DETPIN2, HIGH);

	pinModeFast(DETPIN3, INPUT);
	digitalWriteFast(DETPIN3, HIGH);

	pinModeFast(ARCADE_DB9_PIN, INPUT);
	digitalWriteFast(ARCADE_DB9_PIN, HIGH);
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

		gamepad_state.cross_btn = (button_data & GENESIS_B) > 0;

		gamepad_state.circle_btn = (button_data & GENESIS_C) > 0;

		gamepad_state.l1_btn = (button_data & GENESIS_X) > 0;

		gamepad_state.triangle_btn = (button_data & GENESIS_Y) > 0;

		gamepad_state.r1_btn = (button_data & GENESIS_Z) > 0;

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

		gamepad_state.cross_btn = (button_data & 0x20) > 0;

		gamepad_state.triangle_btn = (button_data & 0x40) > 0;

		gamepad_state.circle_btn = (button_data & 0x80) > 0;

		gamepad_state.l1_btn = (button_data & 0x100) > 0;

		gamepad_state.r1_btn = (button_data & 0x200) > 0;

		gamepad_state.l2_btn = (button_data & 0x400) > 0;

		gamepad_state.r2_btn = (button_data & 0x800) > 0;

		gamepad_state.select_btn = (button_data & 0x1000) > 0;

		gamepad_state.start_btn = (button_data & 2000) > 0;

		gamepad_state.direction = (button_data & 4000) ? 0 : 8;

		gamepad_state.ps_btn = (button_data & 8000) > 0;

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

		gamepad_state.cross_btn = (button_data & 1) > 0;

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

		gamepad_state.cross_btn = (button_data & 1) > 0;

		gamepad_state.circle_btn = (button_data & 256) > 0;

		gamepad_state.l1_btn = (button_data & 1024) > 0;

		gamepad_state.triangle_btn = (button_data & 512) > 0;

		gamepad_state.r1_btn = (button_data & 2048) > 0;

		gamepad_state.select_btn = (button_data & 4) > 0;

		gamepad_state.start_btn = (button_data & 8) > 0;

		gamepad_state.ps_btn = (button_data & 4) && (button_data & 8); // SELECT + START = PS Button

		vs_send_pad_state();
	}
}

void ps2_loop() {
	PS2X psPad;
	byte dir = 0;

	while (psPad.config_gamepad(8, 6, 7, 5, false, false) == 1);

	for (;;) {
		//vs_reset_watchdog();

		psPad.read_gamepad();

		if(psPad.readType() == 0) {
			gamepad_state.l_x_axis = 0x80;
			gamepad_state.l_y_axis = 0x80;

			if(psPad.Button(PSB_PAD_LEFT)) {
				gamepad_state.l_x_axis = 0x00;
			} else if (psPad.Button(PSB_PAD_RIGHT)) {
				gamepad_state.l_x_axis = 0xFF;
			}

			if(psPad.Button(PSB_PAD_UP)) {
				gamepad_state.l_y_axis = 0x00;
			} else if (psPad.Button(PSB_PAD_DOWN)) {
				gamepad_state.l_y_axis = 0xFF;
			}

		} else {
			gamepad_state.l_x_axis = psPad.Analog(PSS_LX);
			gamepad_state.l_y_axis = psPad.Analog(PSS_LY);
			gamepad_state.r_x_axis = psPad.Analog(PSS_RX);
			gamepad_state.r_y_axis = psPad.Analog(PSS_RY);

			dir = psPad.Button(PSB_PAD_UP) << 3 | psPad.Button(PSB_PAD_DOWN) << 2 | psPad.Button(PSB_PAD_LEFT) << 1 | psPad.Button(PSB_PAD_RIGHT);

			gamepad_state.direction = pad_dir[dir];
		}

		gamepad_state.square_btn = psPad.Button(PSB_SQUARE);

		gamepad_state.cross_btn = psPad.Button(PSB_CROSS);

		gamepad_state.circle_btn = psPad.Button(PSB_CIRCLE);

		gamepad_state.l1_btn =psPad.Button(PSB_L1);

		gamepad_state.l2_btn =psPad.Button(PSB_L2);

		gamepad_state.triangle_btn = psPad.Button(PSB_TRIANGLE);

		gamepad_state.r1_btn = psPad.Button(PSB_R1);

		gamepad_state.r2_btn = psPad.Button(PSB_R2);

		gamepad_state.select_btn = psPad.Button(PSB_SELECT);

		gamepad_state.start_btn = psPad.Button(PSB_START);

		gamepad_state.ps_btn = psPad.Button(PSB_SELECT) && psPad.Button(PSB_START); // SELECT + START = PS Button

		vs_send_pad_state();
	}
}

void gc_loop() {
	byte *button_data;
	byte dir = 0;

	GCPad_init();

	for(;;) {
		//vs_reset_watchdog();

		button_data = GCPad_read();

		dir = ((button_data[1] & 0x08) > 0) << 3 | ((button_data[1] & 0x04) > 0) << 2 | ((button_data[1] & 0x01) > 0) << 1 | ((button_data[1] & 0x02) > 0);

		gamepad_state.direction = pad_dir[dir];

		gamepad_state.square_btn = (button_data[0] & 0x08) > 0;

		gamepad_state.cross_btn = (button_data[0] & 0x02) > 0;

		gamepad_state.triangle_btn = (button_data[0] & 0x04) > 0;

		gamepad_state.circle_btn = (button_data[0] & 0x01) > 0;

		gamepad_state.start_btn = (button_data[0] & 0x10) > 0;

		gamepad_state.l1_btn = (button_data[1] & 0x40) > 0;

		gamepad_state.r1_btn = (button_data[1] & 0x20) > 0;

		gamepad_state.l2_btn = (button_data[1] & 0x10) > 0;

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

	GCPad_init();

	for(;;) {
		//vs_reset_watchdog();

		button_data = N64Pad_read();

		dir = ((button_data[0] & 0x08) > 0) << 3 | ((button_data[0] & 0x04) > 0) << 2 | ((button_data[0] & 0x02) > 0) << 1 | ((button_data[0] & 0x01) > 0);

		gamepad_state.direction = pad_dir[dir];

		gamepad_state.square_btn = (button_data[0] & 0x40) > 0;

		gamepad_state.cross_btn = (button_data[0] & 0x80) > 0;

		gamepad_state.start_btn = (button_data[0] & 0x10) > 0;

		gamepad_state.l1_btn = (button_data[1] & 0x20) > 0;

		gamepad_state.r1_btn = (button_data[1] & 0x10) > 0;

		gamepad_state.l2_btn = (button_data[0] & 0x20) > 0;

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
	default:
		genesis_loop();
		break;
	}
}
