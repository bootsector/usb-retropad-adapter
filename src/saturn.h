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

#ifndef SATURN_H_
#define SATURN_H_

void saturn_init();
int saturn_read();

#define SATURN_UP		0x100
#define SATURN_DOWN		0x200
#define SATURN_LEFT		0x400
#define SATURN_RIGHT	0x800
#define SATURN_A 		0x40
#define SATURN_B 		0x10
#define SATURN_C 		0x20
#define SATURN_X 		0x04
#define SATURN_Y 		0x02
#define SATURN_Z 		0x01
#define SATURN_START	0x80
#define SATURN_L 		0x1000
#define SATURN_R 		0x08

#endif /* SATURN_H_ */
