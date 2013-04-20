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

#include "tg16.h"
#include "digitalWriteFast.h"

void tg16_init(void) {
	// Configure Data pins
	pinModeFast(5, INPUT);
	digitalWriteFast(5, HIGH);

	pinModeFast(7, INPUT);
	digitalWriteFast(7, HIGH);

	pinModeFast(8, INPUT);
	digitalWriteFast(8, HIGH);

	pinModeFast(9, INPUT);
	digitalWriteFast(9, HIGH);

	// Configure Data Select and /OE pins
	pinModeFast(10, OUTPUT);
	digitalWriteFast(10, HIGH);

	pinModeFast(11, OUTPUT);
	digitalWriteFast(11, LOW);
}

int tg16_read(void) {
	int retval = 0;

	// Data Select HIGH
	digitalWriteFast(10, HIGH);

	// /OE LOW
	digitalWriteFast(11, LOW);

	for(int i = 0; i < 2; i++) {
		// /OE LOW
		digitalWriteFast(11, LOW);
		delayMicroseconds(1);

		// If four directions are low, then it's an Avenue6 Pad
		if(!digitalReadFast(5) && !digitalReadFast(7) && !digitalReadFast(8) && !digitalReadFast(9)) {
			// Data Select LOW
			digitalWriteFast(10, LOW);
			delayMicroseconds(1);

			retval |= (!digitalReadFast(5) << 8);  // III
			retval |= (!digitalReadFast(7) << 9);  // IV
			retval |= (!digitalReadFast(8) << 10); // V
			retval |= (!digitalReadFast(9) << 11); // VI
		} else {
			// Normal pad reading
			retval |= (!digitalReadFast(5) << 0); // UP
			retval |= (!digitalReadFast(7) << 1); // RIGHT
			retval |= (!digitalReadFast(8) << 2); // DOWN
			retval |= (!digitalReadFast(9) << 3); // LEFT

			// Data Select LOW
			digitalWriteFast(10, LOW);
			delayMicroseconds(1);

			retval |= (!digitalReadFast(5) << 4); // I
			retval |= (!digitalReadFast(7) << 5); // II
			retval |= (!digitalReadFast(8) << 6); // SELECT
			retval |= (!digitalReadFast(9) << 7); // RUN
		}

		// Data Select HIGH
		digitalWriteFast(10, HIGH);

		// /OE HIGH
		digitalWriteFast(11, HIGH);
	}

	return retval;
}
