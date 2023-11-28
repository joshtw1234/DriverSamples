/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include "public.h"

EXTERN_C_START

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    ULONG PrivateDeviceData;  // just a placeholder

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

/*
 * Intel SMBus memory register structure (D31,F1)
 */
typedef struct _INTELSMBUS_REGS_ {
	/* [OFFSET]     [DESC]				[DEFAULT]	[ATTRIBUTE]	*/
	UCHAR HST_STS;			/* 00h		Host Status			00h		R/WC, RO	*/
	UCHAR Reserve1;			/* 01h		N/A				N/A		N/A		*/
	UCHAR HST_CNT;			/* 02h		Host Control			00h		R/W, WO		*/
	UCHAR HST_CMD;			/* 03h		Host Command			00h		R/W		*/
	UCHAR XMIT_SLVA;		/* 04h		Transmit Slave Address		00h		R/W		*/
	UCHAR HST_D0;			/* 05h		Host Data 0			00h		R/W		*/
	UCHAR HST_D1;			/* 06h		Host Data 1			00h		R/W		*/
	UCHAR HOST_BLOCK_DB;		/* 07h		Host Block Data Byte		00h		R/W		*/
	UCHAR PEC;			/* 08h		Packet Error Check		00h		R/W		*/
	UCHAR RCV_SLVA;			/* 09h		Receive Slave Address		44h		R/W		*/
	USHORT SLV_DATA;		/* 0Ah-0Bh	Receive Slave Data		0000h		RO		*/
	UCHAR AUX_STS;			/* 0Ch		Auxiliary Status		00h		R/WC, RO	*/
	UCHAR AUX_CTL;			/* 0Dh		Auxiliary Control		00h		R/W		*/
	UCHAR SMLINK_PIN_CTL;		/* 0Eh		SMLink Pin Control		N/A		R/W, RO		*/
	UCHAR SMBus_PIN_CTL;		/* 0Fh		SMBus Pin Control		N/A		R/W, RO		*/
	UCHAR SLV_STS;			/* 10h		Slave Status			00h		R/WC		*/
	UCHAR SLV_CMD;			/* 11h		Slave Command			00h		R/W		*/
	UCHAR Reserve2;			/* 12h		N/A				N/A		N/A		*/
	UCHAR Reserve3;			/* 13h		N/A				N/A		N/A		*/
	UCHAR NOTIFY_DADDR;		/* 14h		Notify Device Address		00h		RO		*/
	UCHAR Reserve4;			/* 15h		N/A				N/A		N/A		*/
	UCHAR NOTIFY_DLOW;		/* 16h		Notify Data Low Byte		00h		RO		*/
	UCHAR NOTIFY_DHIGH;		/* 17h		Notify Data High Byte		00h		RO		*/
} INTELSMBUS_REGS, * PINTELSMBUS_REGS;

/*
 * The device extension for the device object
 */
typedef struct _DEVICE_EXTENSION {
	WDFDEVICE Device;
	/* HW resources */
	PINTELSMBUS_REGS Regs;			// registers address.
	ULONG RegsLength;			// registers base length.
	BOOLEAN PortMapped;			// TRUE if mapped port addr
	PINTELSMBUS_REGS PortStruct;		// port register address.
	ULONG PortLength;			// port base length.
	WDFINTERRUPT Interrupt;			// returned by InterruptCreate.
	UCHAR resMask;				// HW resource mask.
	/*
	 * Thread for polling HW status.
	 */
	UCHAR thFlag;				// thread flag
	HANDLE fspsmbth;			// thread handle.
	CLIENT_ID fspsmbthId;			// thread ID.
} DEVICE_EXTENSION, * PDEVICE_EXTENSION;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, DeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
KMDFSmbusCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_EXTENSION, FilterDevGetExt)

/*
* WDF D0 Events
*/
EVT_WDF_DEVICE_D0_ENTRY KMDFSmbusEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT KMDFSmbusEvtDeviceD0Exit;
//EVT_WDF_DEVICE_PREPARE_HARDWARE KMDFSmbusEvtDevicePrepareHardware;
//EVT_WDF_DEVICE_RELEASE_HARDWARE KMDFSmbusEvtDeviceReleaseHardware;
EXTERN_C_END
