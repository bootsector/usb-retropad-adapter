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
#include "NESPad.h"
#include "digitalWriteFast.h"

void NESPad::init() {
	pinModeFast(CLOCK_PIN, OUTPUT);
	pinModeFast(LATCH_PIN, OUTPUT);
	pinModeFast(DATA_PIN, INPUT);

	// Turns data pin pull-out resistor ON
	digitalWriteFast(DATA_PIN, HIGH);
}

int NESPad::read(int bits) {
	int state, i;

	digitalWriteFast(LATCH_PIN, LOW);
	digitalWriteFast(CLOCK_PIN, LOW);

	digitalWriteFast(LATCH_PIN, HIGH);
	delayMicroseconds(1);
	digitalWriteFast(LATCH_PIN, LOW);

	state = digitalReadFast(DATA_PIN);

	for (i = 1; i < bits; i++) {
		digitalWriteFast(CLOCK_PIN, HIGH);
		delayMicroseconds(1);
		digitalWriteFast(CLOCK_PIN, LOW);

		state = state | (digitalReadFast(DATA_PIN) << i);
	}

	return ~state;
}
