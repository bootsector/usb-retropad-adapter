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
#include "PS2Pad.h"
#include "digitalWriteFast.h"

byte PS2Pad::_type;
byte PS2Pad::_pad_data[21];
byte PS2Pad::_read_delay = 1;

byte PS2Pad::gamepad_spi(byte send_data) {
	byte recv_data = 0;

	noInterrupts();

	for(byte i = 0; i < 8; i++) {

		digitalWriteFast(CLK_PIN, LOW);

		if(send_data & (1 << i)) {
			digitalWriteFast(CMD_PIN, HIGH);
		} else {
			digitalWriteFast(CMD_PIN, LOW);
		}

		interrupts();

		delayMicroseconds(CTRL_CLK);

		noInterrupts();

		digitalWriteFast(CLK_PIN, HIGH);

		if(digitalReadFast(DAT_PIN)) {
			recv_data |= (1 << i);
		}

		delayMicroseconds(CTRL_CLK);
	}

	interrupts();

	digitalWriteFast(CLK_PIN, HIGH);

	delayMicroseconds(CTRL_BYTE_DELAY);

	return recv_data;
}

void PS2Pad::send_command(byte data[], byte size) {

	noInterrupts();

	digitalWriteFast(ATT_PIN, LOW);
	digitalWriteFast(CMD_PIN, HIGH);

	digitalWriteFast(CLK_PIN, HIGH);

	interrupts();

	delayMicroseconds(CTRL_BYTE_DELAY*2);

	for(byte i = 0; i < size; i++) {
		data[i] = PS2Pad::gamepad_spi(data[i]);
	}

	digitalWriteFast(ATT_PIN, HIGH);

	delayMicroseconds(PS2Pad::_read_delay * 1000);
}

void PS2Pad::read() {
	PS2Pad::_pad_data[0] = 0x01;
	PS2Pad::_pad_data[1] = 0x42;

	delayMicroseconds(PS2Pad::_read_delay * 1000);

	for (byte i = 2; i < 21; i++) {
		PS2Pad::_pad_data[i] = 0x00;
	}

	PS2Pad::send_command(PS2Pad::_pad_data, 21);
}

int PS2Pad::init() {
	pinModeFast(DAT_PIN, INPUT);
	digitalWriteFast(DAT_PIN, HIGH);

	pinModeFast(CLK_PIN, OUTPUT);
	pinModeFast(ATT_PIN, OUTPUT);
	pinModeFast(CMD_PIN, OUTPUT);

	// Init pad

	noInterrupts();

	digitalWriteFast(CLK_PIN, HIGH);
	digitalWriteFast(CMD_PIN, HIGH);

	interrupts();

	PS2Pad::read();

	if(PS2Pad::_pad_data[1] != 0x41 && PS2Pad::_pad_data[1] != 0x73 && PS2Pad::_pad_data[1] != 0x79) {
		return 1;
	}

	PS2Pad::_read_delay = 1;

	for(byte i = 0; i <= 2; i++) {

		// Enter Config Mode
		byte enter_config_command[] = {0x01, 0x43, 0x00, 0x01, 0x00};
		PS2Pad::send_command(enter_config_command, 5);

		// Get pad type
		byte get_pad_type_command[] = {0x01, 0x45, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
		PS2Pad::send_command(get_pad_type_command, 9);

		PS2Pad::_type = get_pad_type_command[3];

		// Lock to Analog Mode on Stick
		byte lock_analog_mode_command[] = {0x01, 0x44, 0x00, 0x01, 0x03, 0x00, 0x00, 0x00, 0x00};
		PS2Pad::send_command(lock_analog_mode_command, 9);

		// Exit config mode
		byte exit_config_command[] = {0x01, 0x43, 0x00, 0x00, 0x5A, 0x5A, 0x5A, 0x5A, 0x5A};
		PS2Pad::send_command(exit_config_command, 9);

		PS2Pad::read();

		if(PS2Pad::_pad_data[1] == 0x73)
			break;

		PS2Pad::_read_delay++;
	}

	return 0;
}

word PS2Pad::psx_buttons() {
	word buttons = *(word*)(PS2Pad::_pad_data + 3);
	return ~buttons;
}

byte PS2Pad::button(word button) {
	uint16_t buttons = psx_buttons();
	return ((buttons & button) > 0);
}

byte PS2Pad::stick(word stick) {
	return PS2Pad::_pad_data[stick];
}

byte PS2Pad::type() {
	if(PS2Pad::_type == 0x03)
		return 1;
	else if(PS2Pad::_type == 0x01)
		return 2;

	return 0;
}
