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

#ifndef PSX_H_
#define PSX_H_

#define DAT	5
#define	CMD	6
#define	ATT	7
#define	CLK	8

#define PSXDELAY	10
#define CTRL_BYTE_DELAY 3

#define	PSX_ID_DIGITAL	0x41
#define PSX_ID_A_RED	0x73
#define	PSX_ID_A_GREEN	0x53

typedef struct {
	// digital buttons, 0 = off, 1 = on

	uint8_t square_btn : 1;
	uint8_t cross_btn : 1;
	uint8_t circle_btn : 1;
	uint8_t triangle_btn : 1;

	uint8_t l1_btn : 1;
	uint8_t r1_btn : 1;
	uint8_t l2_btn : 1;
	uint8_t r2_btn : 1;

	uint8_t select_btn : 1;
	uint8_t start_btn : 1;
	uint8_t l3_btn : 1;
	uint8_t r3_btn : 1;
	uint8_t : 4;

	uint8_t up_btn : 1;
	uint8_t down_btn : 1;
	uint8_t left_btn : 1;
	uint8_t right_btn : 1;
	uint8_t : 4;

	// left and right analog sticks, 0x00 left/up, 0x80 middle, 0xff right/down

	uint8_t l_x_axis;
	uint8_t l_y_axis;
	uint8_t r_x_axis;
	uint8_t r_y_axis;
} psxpad_state_t;

void psx_init();
psxpad_state_t* psx_read(byte* pad_id);

#endif /* PSX_H_ */
