/*
* Wii RetroPad Adapter - Nintendo Wiimote adapter for retro-controllers!
* Copyright (c) 2011 Bruno Freitas - bootsector@ig.com.br
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
byte ps2_data[22];
byte read_data_cmd[] = {0x01, 0x42, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

void psx_wait_ack() {
	delayMicroseconds(50);
}

void psx_command(byte *command, byte size) {
	byte data;
	byte i, j;

	if(size > 22)
		return;

	delayMicroseconds(50);

	for(j = 0; j < size; j++) {

		data = 0;

		for (i = 0; i < 8; i++) {
			if (*command & (1 << i))
				digitalWriteFast(CMD, HIGH);
			else
				digitalWriteFast(CMD, LOW);

			digitalWriteFast(CLK, LOW);

			delayMicroseconds(PSXDELAY);

			data >>= 1;

			digitalWriteFast(CLK, HIGH);

			delayMicroseconds(PSXDELAY - 1);

			if (digitalReadFast(DAT))
				data |= (1 << 7); //(1<<i);
		}

		ps2_data[j] = data;
		command++;
	}

	delayMicroseconds(CTRL_BYTE_DELAY);
}

psxpad_state_t* psx_read(byte* pad_id) {
	byte data, id;

	digitalWriteFast(ATT, LOW);
	delayMicroseconds(1);

	psx_command(read_data_cmd, sizeof(read_data_cmd));

	id = ps2_data[1];

	*pad_id = id;

	if ((id == PSX_ID_DIGITAL) | (id == PSX_ID_A_RED)
			| (id == PSX_ID_A_GREEN)) {

		if (ps2_data[2] == 0x5a) {

			pad_state.select_btn = !(ps2_data[3] & (1));
			pad_state.start_btn = !(ps2_data[3] & (8));

			pad_state.up_btn = !(ps2_data[3] & (16));
			pad_state.down_btn = !(ps2_data[3] & (64));
			pad_state.left_btn = !(ps2_data[3] & (128));
			pad_state.right_btn = !(ps2_data[3] & (32));

			if (id == PSX_ID_A_RED) {
				pad_state.l3_btn = !(ps2_data[3] & (2));
				pad_state.r3_btn = !(ps2_data[3] & (4));
			}

			pad_state.l2_btn = !(ps2_data[4] & (1));
			pad_state.r2_btn = !(ps2_data[4] & (2));
			pad_state.l1_btn = !(ps2_data[4] & (4));
			pad_state.r1_btn = !(ps2_data[4] & (8));
			pad_state.triangle_btn = !(ps2_data[4] & (16));
			pad_state.circle_btn = !(ps2_data[4] & (32));
			pad_state.cross_btn = !(ps2_data[4] & (64));
			pad_state.square_btn = !(ps2_data[4] & (128));

			if ((id == PSX_ID_A_RED) | (id == PSX_ID_A_GREEN)) {
				pad_state.r_x_axis = ps2_data[5];
				pad_state.r_y_axis = ps2_data[6];
				pad_state.l_x_axis = ps2_data[7];
				pad_state.l_y_axis = ps2_data[8];
			}
		}
	}

	digitalWriteFast(ATT, HIGH);

	return &pad_state;
}

void psx_init() {
	pinModeFast(DAT, INPUT);
	pinModeFast(CMD, OUTPUT);
	pinModeFast(ATT, OUTPUT);
	pinModeFast(CLK, OUTPUT);

	digitalWriteFast(DAT, HIGH);
	digitalWriteFast(CMD, HIGH);
	digitalWriteFast(ATT, HIGH);
	digitalWriteFast(CLK, HIGH);

	memset(ps2_data, 0x00, sizeof(ps2_data));

	memset(&pad_state, 0x00, sizeof(psxpad_state_t));

	pad_state.l_x_axis = 0x80;
	pad_state.l_y_axis = 0x80;
	pad_state.r_x_axis = 0x80;
	pad_state.r_y_axis = 0x80;
}
