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

#ifndef XBOXPAD_H_
#define XBOXPAD_H_

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */
#include <string.h>			/* for memset() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */

extern "C" {				/* without extern "C", this won't link as a .cpp file */
#include "usbdrv.h"
}

typedef struct {
	uint8_t rid;
	uint8_t rsize;
	uint8_t digital_buttons;
	uint8_t reserved_1;
	uint8_t a;
	uint8_t b;
	uint8_t x;
	uint8_t y;
	uint8_t black;
	uint8_t white;
	uint8_t l;
	uint8_t r;
	int l_x;
	int l_y;
	int r_x;
	int r_y;
	uint8_t reserved_2[8];
	uint8_t reserved_3[8];
} gamepad_state_t;

void xbox_reset_pad_status();
void xbox_init(bool watchdog);
void xbox_reset_watchdog();
void xbox_send_pad_state();
int xbox_pad_detected();

extern gamepad_state_t gamepad_state;

#define XBOX_DPAD_UP		0
#define XBOX_DPAD_DOWN		1
#define XBOX_DPAD_LEFT		2
#define XBOX_DPAD_RIGHT		3
#define XBOX_START			4
#define XBOX_BACK			5
#define XBOX_LEFT_STICK		6
#define XBOX_RIGHT_STICK	7

#endif /* XBOXPAD_H_ */
