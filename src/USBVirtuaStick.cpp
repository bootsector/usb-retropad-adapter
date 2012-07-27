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

#include "USBVirtuaStick.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

PROGMEM char usbHidReportDescriptor[112] = { /* USB report descriptor, size must match usbconfig.h */
0x05, 0x01, // USAGE_PAGE (Generic Desktop)
		0x09, 0x05, // USAGE (Gamepad)
		0xa1, 0x01, // COLLECTION (Application)
		0x15, 0x00, //   LOGICAL_MINIMUM (0)
		0x25, 0x01, //   LOGICAL_MAXIMUM (1)
		0x35, 0x00, //   PHYSICAL_MINIMUM (0)
		0x45, 0x01, //   PHYSICAL_MAXIMUM (1)
		0x75, 0x01, //   REPORT_SIZE (1)
		0x95, 0x0d, //   REPORT_COUNT (13)
		0x05, 0x09, //   USAGE_PAGE (Button)
		0x19, 0x01, //   USAGE_MINIMUM (Button 1)
		0x29, 0x0d, //   USAGE_MAXIMUM (Button 13)
		0x81, 0x02, //   INPUT (Data,Var,Abs)
		0x95, 0x03, //   REPORT_COUNT (3)
		0x81, 0x01, //   INPUT (Cnst,Ary,Abs)
		0x05, 0x01, //   USAGE_PAGE (Generic Desktop)
		0x25, 0x07, //   LOGICAL_MAXIMUM (7)
		0x46, 0x3b, 0x01, //   PHYSICAL_MAXIMUM (315)
		0x75, 0x04, //   REPORT_SIZE (4)
		0x95, 0x01, //   REPORT_COUNT (1)
		0x65, 0x14, //   UNIT (Eng Rot:Angular Pos)
		0x09, 0x39, //   USAGE (Hat switch)
		0x81, 0x42, //   INPUT (Data,Var,Abs,Null)
		0x65, 0x00, //   UNIT (None)
		0x95, 0x01, //   REPORT_COUNT (1)
		0x81, 0x01, //   INPUT (Cnst,Ary,Abs)
		0x26, 0xff, 0x00, //   LOGICAL_MAXIMUM (255)
		0x46, 0xff, 0x00, //   PHYSICAL_MAXIMUM (255)
		0x09, 0x30, //   USAGE (X)
		0x09, 0x31, //   USAGE (Y)
		0x09, 0x32, //   USAGE (Z)
		0x09, 0x35, //   USAGE (Rz)
		0x75, 0x08, //   REPORT_SIZE (8)
		0x95, 0x04, //   REPORT_COUNT (4)
		0x81, 0x02, //   INPUT (Data,Var,Abs)
		0x06, 0x00, 0xff, //   USAGE_PAGE (Vendor Specific)
		0x09, 0x20, //   Unknown
		0x09, 0x21, //   Unknown
		0x09, 0x22, //   Unknown
		0x09, 0x23, //   Unknown
		0x09, 0x24, //   Unknown
		0x09, 0x25, //   Unknown
		0x09, 0x26, //   Unknown
		0x09, 0x27, //   Unknown
		0x09, 0x28, //   Unknown
		0x09, 0x29, //   Unknown
		0x09, 0x2a, //   Unknown
		0x09, 0x2b, //   Unknown
		0x95, 0x0c, //   REPORT_COUNT (12)
		0x81, 0x02, //   INPUT (Data,Var,Abs)
		0x0a, 0x21, 0x26, //   Unknown
		0x95, 0x08, //   REPORT_COUNT (8)
		0xb1, 0x02, //   FEATURE (Data,Var,Abs)
		0xc0, // END_COLLECTION
		};

/* ------------------------------------------------------------------------- */

gamepad_state_t gamepad_state;
static uchar ps3_magic_bytes[8] = { 0x21, 0x26, 0x01, 0x07, 0x00, 0x00, 0x00,
		0x00 };
static uchar idleRate;

void vs_reset_pad_status() {
	memset(&gamepad_state, 0x00, sizeof(gamepad_state_t));
	gamepad_state.direction = 0x08;
	gamepad_state.l_x_axis = 0x80;
	gamepad_state.l_y_axis = 0x80;
	gamepad_state.r_x_axis = 0x80;
	gamepad_state.r_y_axis = 0x80;
}

void vs_init(bool watchdog) {
	uchar i;

	// disable timer 0 overflow interrupt (enabled by Arduino's init() function).
	// PS3 was having difficulties detecting the adapter if that's enabled.
	// WARNING: This will mess up with micros(), millis() and delay() Arduino functions!
	// Use alternate timer functions instead!
#if defined(TIMSK) && defined(TOIE0)
	(_SFR_BYTE(TIMSK) &= ~_BV(TOIE0));
#elif defined(TIMSK0) && defined(TOIE0)
	(_SFR_BYTE(TIMSK0) &= ~_BV(TOIE0));
#endif

	vs_reset_pad_status();

	if(watchdog) {
		wdt_enable(WDTO_2S);
	} else {
		wdt_disable();
	}
	/* Even if you don't use the watchdog, turn it off here. On newer devices,
	 * the status of the watchdog (on/off, period) is PRESERVED OVER RESET!
	 */
	/* RESET status: all port bits are inputs without pull-up.
	 * That's the way we need D+ and D-. Therefore we don't need any
	 * additional hardware initialization.
	 */

	usbInit();
	usbDeviceDisconnect(); /* enforce re-enumeration, do this while interrupts are disabled! */
	i = 0;
	while (--i) { /* fake USB disconnect for > 250 ms */
		wdt_reset();
		_delay_ms(1);
	}
	usbDeviceConnect();
	sei();
}

void vs_reset_watchdog() {
	wdt_reset();
}

void vs_send_pad_state() {
	while (!usbInterruptIsReady())
		usbPoll();
	usbSetInterrupt((unsigned char *) &gamepad_state, 8);
	while (!usbInterruptIsReady())
		usbPoll();
	usbSetInterrupt((unsigned char *) &gamepad_state + 8, 8);
	while (!usbInterruptIsReady())
		usbPoll();
	usbSetInterrupt((unsigned char *) &gamepad_state + 16, 3);
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t *rq = (usbRequest_t *) data;

	if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) { /* class request type */

		if (rq->bRequest == USBRQ_HID_GET_REPORT) { /* wValue: ReportType (highbyte), ReportID (lowbyte) */

			if (rq->wValue.bytes[1] == 0x03) { // #define HID_REPORT_TYPE_FEATURE 3
				if (rq->wValue.bytes[0] == 0) {
					usbMsgPtr = (uchar *) ps3_magic_bytes;
					return sizeof(ps3_magic_bytes);
				}
			}

		} else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
			usbMsgPtr = &idleRate;
			return 1;
		} else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
			idleRate = rq->wValue.bytes[1];
		}

	} else {
		/* no vendor specific requests implemented */
	}

	return 0; /* default for not implemented requests: return no data back to host */
}
