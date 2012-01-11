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

/*
NES Controller pinouts:
----------------------

           +---+
GND      - | O  \
CLOCK    - | O O + - +5V
LATCH    - | O O | - NC
DATA OUT - | O O | - NC
           +-----+


NES Pad read data format:
--------------------
      +-------+------+------+----+-------+--------+---+---+
Bit:  |   7   |   6  |   5  |  4 |   3   |    2   | 1 | 0 |
      +-------+------+------+----+-------+--------+---+---+
Data: | RIGHT | LEFT | DOWN | UP | START | SELECT | B | A |
      +-------+------+------+----+-------+--------+---+---+


SNES Clock pulse and corresponding button

1   B
    Y
    Select
    Start
    North
    South
    West
    East
    A
    X
    L
    R
    [no button, always high]
    [no button, always high]
    [no button, always high]
16  [no button, always high]
*/

#ifndef NESPAD_H_
#define NESPAD_H_

#include <WProgram.h>

class NESPad {

private:
	static int _clock, _latch, _data;

public:
	static void init(int clock, int latch, int data);
	static int read(int bits);

};

#endif /* NESPAD_H_ */
