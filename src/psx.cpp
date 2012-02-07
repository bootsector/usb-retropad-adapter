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
#include "digitalWriteFast.h"
#include "psx.h"

psxpad_state_t pad_state;

void psx_init() {
	pinModeFast(DAT, INPUT);
	pinModeFast(CMD, OUTPUT);
	pinModeFast(ATT, OUTPUT);
	pinModeFast(CLK, OUTPUT);

	digitalWriteFast(DAT, HIGH);
	digitalWriteFast(CMD, HIGH);
	digitalWriteFast(ATT, HIGH);
	digitalWriteFast(CLK, HIGH);

	memset(&pad_state, 0x00, sizeof(psxpad_state_t));
	pad_state.l_x_axis = 0x80;
	pad_state.l_y_axis = 0x80;
	pad_state.r_x_axis = 0x80;
	pad_state.r_y_axis = 0x80;
}

void psx_wait_ack() {
	delayMicroseconds(50);
}

byte psx_command(byte command) {
	byte i = 0;
	byte data = 0;

	delayMicroseconds(50);

	for (i = 0; i < 8; i++) {
		if (command & 1)
			digitalWriteFast(CMD, HIGH);
		else
			digitalWriteFast(CMD, LOW);

		command >>= 1;

		digitalWriteFast(CLK, LOW);

		delayMicroseconds(PSXDELAY);

		data >>= 1;

		digitalWriteFast(CLK, HIGH);

		delayMicroseconds(PSXDELAY - 1);

		if (digitalReadFast(DAT))
			data |= (1 << 7); //(1<<i);
	}

	delayMicroseconds(CTRL_BYTE_DELAY);

	return data;
}

psxpad_state_t* psx_read(byte* pad_id) {
	byte data, id;

	digitalWriteFast(ATT, LOW);
	delayMicroseconds(1);

	data = psx_command(0x01); // Issue start command, data is discarded

	psx_wait_ack();

	id = psx_command(0x42); // Request controller ID

	*pad_id = id;

	if ((id == PSX_ID_DIGITAL) | (id == PSX_ID_A_RED)
			| (id == PSX_ID_A_GREEN)) {

		data = psx_command(0xff); // expect 0x5a from controller

		if (data == 0x5a) {

			data = psx_command(0xff);

			pad_state.select_btn = !(data & (1));
			pad_state.start_btn = !(data & (8));

			pad_state.up_btn = !(data & (16));
			pad_state.down_btn = !(data & (64));
			pad_state.left_btn = !(data & (128));
			pad_state.right_btn = !(data & (32));

			if (id == PSX_ID_A_RED) {
				pad_state.l3_btn = !(data & (2));
				pad_state.r3_btn = !(data & (4));
			}

			data = psx_command(0xff);

			pad_state.l2_btn = !(data & (1));
			pad_state.r2_btn = !(data & (2));
			pad_state.l1_btn = !(data & (4));
			pad_state.r1_btn = !(data & (8));
			pad_state.triangle_btn = !(data & (16));
			pad_state.circle_btn = !(data & (32));
			pad_state.cross_btn = !(data & (64));
			pad_state.square_btn = !(data & (128));

			if ((id == PSX_ID_A_RED) | (id == PSX_ID_A_GREEN)) {
				data = psx_command(0xff);
				pad_state.r_x_axis = data;

				data = psx_command(0xff);
				pad_state.r_y_axis = data;

				data = psx_command(0xff);
				pad_state.l_x_axis = data;

				data = psx_command(0xff);
				pad_state.l_y_axis = data;
			}
		}
	}

	digitalWriteFast(ATT, HIGH);

	return &pad_state;
}
