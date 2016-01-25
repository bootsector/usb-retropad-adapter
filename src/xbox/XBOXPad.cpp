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

#include "XBOXPad.h"

static int padDetected = 0;

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

//XXX: Dual Strike defines /* bMaxPacketSize0 0x40 */ as 0x08
const PROGMEM char usbDescriptorDevice[18] = {	/* USB device descriptor */
		0x12,						/* bLength 0x12 */
		USBDESCR_DEVICE,			/* bDescriptorType 0x01 */
		0x10, 0x01,					/* bcdUSB 0x0110 (USB spec. 01.10) */
		USB_CFG_DEVICE_CLASS,		/* bDeviceClass 0x00 (ifc's specify own) */
		USB_CFG_DEVICE_SUBCLASS,	/* bDeviceSubClass 0x00 */
		0,							/* bDeviceProtocol 0x00 */
		0x08,						/* bMaxPacketSize0 0x40 */
		USB_CFG_VENDOR_ID,			/* idVendor 0x045E (unknown) */
		USB_CFG_DEVICE_ID,			/* idProduct 0x0202 */
		USB_CFG_DEVICE_VERSION,		/* bcdDevice 0x0100 (release 01.00) */
		0,							/* iManufacturer 0x00 */
		0,							/* iProduct 0x00 */
		0,							/* iSerialNumber 0x00 */
		1,							/* bNumConfigurations 0x01 (1) */
};

//XXX: Dual Strike defines /* wMaxPacketSize 0x0020- Must be full speed device */ as 0x08, 0x00
const PROGMEM char usbDescriptorConfiguration[32] = {
		0x09,				/* bLength 0x09 */
		USBDESCR_CONFIG,	/* bDescriptorType 0x02 */
		0x20, 0x00,			/* wTotalLength 0x0020 (32) */
		0x01,				/* bNumInterfaces 0x01 (1) */
		0x01,				/* bConfigurationValue 0x01 */
		0x00,				/* iConfiguration 0x00 */
		0x80,				/* bmAttributes 0x80 (bus powered) */
		0x32,				/* MaxPower 0x32 (100 mA) */
		0x09,				/* bLength 0x09 */
		USBDESCR_INTERFACE,	/* bDescriptorType 0x04 */
		0x00,				/* bInterfaceNumber 0x00 */
		0x00,				/* bAlternateSetting 0x00 */
		0x02,				/* bNumEndpoints 0x02 */
		0x58,				/* bInterfaceClass 0x58 (USB specified device class) */
		0x42,				/* bInterfaceSubClass 0x42 */
		0x00,				/* bInterfaceProtocol 0x00 (not vendor or class specific) */
		0x00,				/* iInterface 0x00 */
		0x07,				/* bLength 0x07 */
		USBDESCR_ENDPOINT,	/* bDescriptorType 0x05 */
		0x82,				/* bEndpointAddress 0x82 (ep #2, IN) */
		0x03,				/* bmAttributes 0x03 (Transfer Type : Interrupt) */
		0x20, 0x00,			/* wMaxPacketSize 0x0020- Must be full speed device */
		0x04,				/* bInterval 0x04 (4 milliseconds) */
		0x07,				/* bLength 0x07 */
		USBDESCR_ENDPOINT,	/* bDescriptorType 0x05 */
		0x02,				/* bEndpointAddress 0x02 (ep #2, OUT) */
		0x03,				/* bmAttributes 0x03 (Transfer Type : Interrupt) */
		0x08, 0x00,			/* wMaxPacketSize 0x0020- Must be full speed device */
		0x04,				/* bInterval 0x04 (4 milliseconds) */
};

/* ------------------------------------------------------------------------- */

gamepad_state_t gamepad_state;

void xbox_reset_pad_status() {
	memset(&gamepad_state, 0x00, sizeof(gamepad_state_t));

	gamepad_state.rsize = 20;

	gamepad_state.reserved_2[0] = 16; // 16 - must be greater than 7, length of this report?
	gamepad_state.reserved_2[1] = 66; //  0 - needed, USB interface protocol?
	gamepad_state.reserved_2[2] =  0; //  0 - needed, USB interface protocol?
	gamepad_state.reserved_2[3] =  1; //  1 - must be greater than 0, number of interfaces?
	gamepad_state.reserved_2[4] =  1; //  1 - needed, configuration index?
	gamepad_state.reserved_2[5] =  2; //  2 - must be greater than 0, number of endpoints?
	gamepad_state.reserved_2[6] = 20; // 20 - must be less or equal than max packet size for in endpoint, in max packet size?
	gamepad_state.reserved_2[7] =  6; //  6 - must be less or equal than max packet size for out endpoint, out max packet size?

	for(int i = 0; i < 8; i++) {
		gamepad_state.reserved_3[i] = 0xFF;
	}
}

void xbox_init(bool watchdog) {
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

	xbox_reset_pad_status();

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

void xbox_reset_watchdog() {
	wdt_reset();
}

void xbox_send_pad_state() {
	while (!usbInterruptIsReady3())
		usbPoll();
	usbSetInterrupt3((unsigned char *) &gamepad_state, 20);
}

usbMsgLen_t usbFunctionSetup(uchar data[8]) {
	usbRequest_t *rq = (usbRequest_t *) data;

	if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) { /* class request type */

		if (rq->bRequest == USBRQ_HID_GET_REPORT) { /* wValue: ReportType (highbyte), ReportID (lowbyte) */
			usbMsgPtr = (unsigned char*) &gamepad_state;
			return 20;
		}

	} else	if ((rq-> bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_VENDOR) {
		if(rq->bRequest == 0x06) {
			/*
			receivedData == { 0xc1, 0x06, 0x00, 0x42, 0x00, 0x00, 0x10, 0x00 }
			probably means:
			-bmRequestType:
			 +direction: device to host
			 +type:      vendor
			 +recipient: interface
			-request: 0x06
			-wValue:  0, 0x42
			-wIndex:  0, 0
			-wLength: 16
			 */
			padDetected = 1;
			usbMsgPtr = (unsigned char*) (gamepad_state.reserved_2);
			return 16;
		}
	}
	else {
		/* no vendor specific requests implemented */
	}

	return 0; /* default for not implemented requests: return no data back to host */
}

int xbox_pad_detected() {
	return padDetected;
}

