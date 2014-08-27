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

#ifndef TG16_H_
#define TG16_H_

#define TG16_UP		0
#define TG16_RIGHT	1
#define TG16_DOWN	2
#define TG16_LEFT	3
#define TG16_I		4
#define TG16_II		5
#define TG16_SELECT	6
#define TG16_RUN	7
#define TG16_III	8
#define TG16_IV		9
#define TG16_V		10
#define TG16_VI		11

void tg16_init(void);
int tg16_read(void);

#endif /* TG16_H_ */
