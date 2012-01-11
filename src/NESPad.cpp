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
#include "NESPad.h"
#include "digitalWriteFast.h"

int NESPad::_clock, NESPad::_latch, NESPad::_data;

void NESPad::init(int clock, int latch, int data) {
	_clock = clock;
	_latch = latch;
	_data = data;

	pinModeFast(_clock, OUTPUT);
	pinModeFast(_latch, OUTPUT);
	pinModeFast(_data, INPUT);

	// Turns data pin pull-out resistor ON
	digitalWriteFast(_data, HIGH);
}

int NESPad::read(int bits) {
	int state, i;

	digitalWriteFast(_latch, LOW);
	digitalWriteFast(_clock, LOW);

	digitalWriteFast(_latch, HIGH);
	delayMicroseconds(1);
	digitalWriteFast(_latch, LOW);

	state = digitalReadFast(_data);

	for (i = 1; i < bits; i++) {
		digitalWriteFast(_clock, HIGH);
		delayMicroseconds(1);
		digitalWriteFast(_clock, LOW);

		state = state | (digitalReadFast(_data) << i);
	}

	return ~state;
}
