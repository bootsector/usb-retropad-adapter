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

#include "XBOXPad.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

//XXX: Dual Strike defines /* bMaxPacketSize0 0x40 */ as 0x08
PROGMEM char usbDescriptorDevice[18] = {	/* USB device descriptor */
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

PROGMEM int usbDescriptorStringDevice[22] = {
		USB_STRING_DESCRIPTOR_HEADER(USB_CFG_DEVICE_NAME_LEN),
		USB_CFG_DEVICE_NAME,
};

//XXX: Dual Strike defines /* wMaxPacketSize 0x0020- Must be full speed device */ as 0x08, 0x00
PROGMEM char usbDescriptorConfiguration[32] = {
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

//XXX: Dual Strike doesn't define this for XBOX?
PROGMEM char usbHidReportDescriptor[256] = {
    0x05, 0x01,                    // USAGE_PAGE (Generic Desktop)
    0x09, 0x05,                    // USAGE (Game Pad)
    0xa1, 0x01,                    // COLLECTION (Application)
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x09, 0x3a,                    //   USAGE (Counted Buffer)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x3b,                    //     USAGE (Byte Count)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x75, 0x01,                    //       REPORT_SIZE (1)
    0x15, 0x00,                    //       LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //       LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //       PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //       PHYSICAL_MAXIMUM (1)
    0x95, 0x04,                    //       REPORT_COUNT (4)
    0x05, 0x01,                    //       USAGE_PAGE (Generic Desktop)
    0x09, 0x90,                    //       USAGE (Undefined)
    0x09, 0x91,                    //       USAGE (Undefined)
    0x09, 0x93,                    //       USAGE (Undefined)
    0x09, 0x92,                    //       USAGE (Undefined)
    0x81, 0x02,                    //       INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x25, 0x01,                    //     LOGICAL_MAXIMUM (1)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x45, 0x01,                    //     PHYSICAL_MAXIMUM (1)
    0x95, 0x04,                    //     REPORT_COUNT (4)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x07,                    //     USAGE_MINIMUM (Button 7)
    0x29, 0x0a,                    //     USAGE_MAXIMUM (Button 10)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x75, 0x01,                    //     REPORT_SIZE (1)
    0x95, 0x08,                    //     REPORT_COUNT (8)
    0x81, 0x01,                    //     INPUT (Cnst,Ary,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
    0x95, 0x06,                    //     REPORT_COUNT (6)
    0x05, 0x09,                    //     USAGE_PAGE (Button)
    0x19, 0x01,                    //     USAGE_MINIMUM (Button 1)
    0x29, 0x06,                    //     USAGE_MAXIMUM (Button 6)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
    0x95, 0x02,                    //     REPORT_COUNT (2)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x32,                    //     USAGE (Z)
    0x09, 0x35,                    //     USAGE (Rz)
    0x81, 0x02,                    //     INPUT (Data,Var,Abs)
    0x75, 0x10,                    //     REPORT_SIZE (16)
    0x16, 0x00, 0x80,              //     LOGICAL_MINIMUM (-32768)
    0x26, 0xff, 0x7f,              //     LOGICAL_MAXIMUM (32767)
    0x36, 0x00, 0x80,              //     PHYSICAL_MINIMUM (-32768)
    0x46, 0xff, 0x7f,              //     PHYSICAL_MAXIMUM (32767)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x05, 0x01,                    //       USAGE_PAGE (Generic Desktop)
    0x09, 0x30,                    //       USAGE (X)
    0x09, 0x31,                    //       USAGE (Y)
    0x81, 0x02,                    //       INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //     USAGE (Pointer)
    0xa1, 0x00,                    //     COLLECTION (Physical)
    0x95, 0x02,                    //       REPORT_COUNT (2)
    0x05, 0x01,                    //       USAGE_PAGE (Generic Desktop)
    0x09, 0x33,                    //       USAGE (Rx)
    0x09, 0x34,                    //       USAGE (Ry)
    0x81, 0x02,                    //       INPUT (Data,Var,Abs)
    0xc0,                          //     END_COLLECTION
    0xc0,                          //   END_COLLECTION
    0x05, 0x01,                    //   USAGE_PAGE (Generic Desktop)
    0x09, 0x3a,                    //   USAGE (Counted Buffer)
    0xa1, 0x02,                    //   COLLECTION (Logical)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x01,                    //     OUTPUT (Cnst,Ary,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x05, 0x01,                    //     USAGE_PAGE (Generic Desktop)
    0x09, 0x3b,                    //     USAGE (Byte Count)
    0x91, 0x01,                    //     OUTPUT (Cnst,Ary,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x01,                    //     OUTPUT (Cnst,Ary,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x06, 0x00, 0xff,              //     USAGE_PAGE (Generic Desktop)
    0x09, 0x01,                    //     USAGE (Vendor Usage 1)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x91, 0x01,                    //     OUTPUT (Cnst,Ary,Abs)
    0x75, 0x08,                    //     REPORT_SIZE (8)
    0x15, 0x00,                    //     LOGICAL_MINIMUM (0)
    0x26, 0xff, 0x00,              //     LOGICAL_MAXIMUM (255)
    0x35, 0x00,                    //     PHYSICAL_MINIMUM (0)
    0x46, 0xff, 0x00,              //     PHYSICAL_MAXIMUM (255)
    0x95, 0x01,                    //     REPORT_COUNT (1)
    0x06, 0x00, 0xff,              //     USAGE_PAGE (Generic Desktop)
    0x09, 0x02,                    //     USAGE (Vendor Usage 2)
    0x91, 0x02,                    //     OUTPUT (Data,Var,Abs)
    0xc0,                          //   END_COLLECTION
    0xc0                           // END_COLLECTION
};

/* ------------------------------------------------------------------------- */

gamepad_state_t gamepad_state;
static uchar idleRate;

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
		wdt_enable(WDTO_1S);
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
		} else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
			usbMsgPtr = &idleRate;
			return 1;
		} else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
			idleRate = rq->wValue.bytes[1];
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
			usbMsgPtr = (unsigned char*) ((&gamepad_state) + 20);
			return 16;
		}
	}
	else {
		/* no vendor specific requests implemented */
	}

	return 0; /* default for not implemented requests: return no data back to host */
}
