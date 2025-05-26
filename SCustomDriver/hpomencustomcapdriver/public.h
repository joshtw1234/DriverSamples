/*
   Copyright (c) 2017, HP Development Company, L.P. All rights reserved.
   This software contains confidential and proprietary information of HP.
   The user of this software agrees not to disclose, disseminate or copy
   such Confidential Information and shall use the software only in accordance
   with the terms of the license agreement the user entered into with HP.
*/

//
// Define an Interface Guid for bus enumerator class.
// This GUID is used to register (IoRegisterDeviceInterface)
// an instance of an interface so that enumerator application
// can send an ioctl to the bus driver._TOASTER
//

DEFINE_GUID(GUID_DEVINTERFACE_BUSENUM_HPCUSTOMCAP,
	0x3860f06f, 0x2c39, 0x45bf, 0xbb, 0x14, 0x47, 0xa3, 0xc8, 0xf, 0xcc, 0x66);
// {3860F06F-2C39-45BF-BB14-47A3C80FCC66} //Generated new

//
// Define an Interface Guid for toaster device class.
// This GUID is used to register (IoRegisterDeviceInterface)
// an instance of an interface so that user application
// can control the toaster device.
//

DEFINE_GUID(GUID_DEVINTERFACE_HPCUSTOMCAP,
	    0x64b769a8, 0x41e9, 0x4803, 0x8b, 0x3,  0x7,  0x71, 0x5d, 0xf3, 0x87, 0x8e);
// {64B769A8-41E9-4803-8B03-07715DF3878E}	//Generated new

//
// Define a Setup Class GUID for Toaster Class. This is same
// as the TOASTSER CLASS guid in the INF files.
//

DEFINE_GUID(GUID_DEVCLASS_HPCUSTOMCAP,
	0xfb219259, 0xb48d, 0x4e01, 0x8e, 0x73, 0x48, 0x3f, 0xa3, 0x5b, 0x74, 0x77);
// {FB219259-B48D-4E01-8E73-483FA35B7477} //Generated new

//
// Define a WMI GUID to get busenum info.
//

DEFINE_GUID(HPCUSTOMCAP_BUS_WMI_STD_DATA_GUID,
	0xbaa8c482, 0x7648, 0x4092, 0x8d, 0x98, 0xed, 0x48, 0xf3, 0x6, 0xa2, 0x4d);
// {BAA8C482-7648-4092-8D98-ED48F306A24D} //Generated new

//
// Define a WMI GUID to get toaster device info.
//

DEFINE_GUID(HPCUSTOMCAP_WMI_STD_DATA_GUID,
	0x9c6981d5, 0xcdc8, 0x406e, 0xbf, 0xdd, 0x20, 0x8f, 0x9, 0xe4, 0x5f, 0xa9);
// {9C6981D5-CDC8-406E-BFDD-208F09E45FA9} //Generated new

//
// Define a WMI GUID to represent device arrival notification WMIEvent class.
//

DEFINE_GUID(HPCUSTOMCAP_NOTIFY_DEVICE_ARRIVAL_EVENT,
	0xb447f123, 0x9bcf, 0x4cd7, 0xbe, 0x33, 0xa, 0x0, 0x13, 0x57, 0x5d, 0x62);
// {B447F123-9BCF-4CD7-BE33-0A0013575D62} //Generated new

//
// GUID definition are required to be outside of header inclusion pragma to avoid
// error during precompiled headers.
//

#ifndef __PUBLIC_H
#define __PUBLIC_H

#define BUS_HARDWARE_IDS L"{FB219259-B48D-4E01-8E73-483FA35B7477}\\HpCustomCapMs\0"
#define BUS_HARDWARE_IDS_LENGTH sizeof (BUS_HARDWARE_IDS)

#define BUSENUM_COMPATIBLE_IDS L"{FB219259-B48D-4E01-8E73-483FA35B7477}\\HpCompatibleCustomCap\0"
#define BUSENUM_COMPATIBLE_IDS_LENGTH sizeof(BUSENUM_COMPATIBLE_IDS)


#define FILE_DEVICE_BUSENUM         FILE_DEVICE_BUS_EXTENDER

#define BUSENUM_IOCTL(_index_) \
    CTL_CODE (FILE_DEVICE_BUSENUM, _index_, METHOD_BUFFERED, FILE_READ_DATA)

#define IOCTL_BUSENUM_PLUGIN_HARDWARE               BUSENUM_IOCTL (0x0)
#define IOCTL_BUSENUM_UNPLUG_HARDWARE               BUSENUM_IOCTL (0x1)
#define IOCTL_BUSENUM_EJECT_HARDWARE                BUSENUM_IOCTL (0x2)
#define IOCTL_HPCUSTOMCAP_DONT_DISPLAY_IN_UI_DEVICE     BUSENUM_IOCTL (0x3)

//
//  Data structure used in PlugIn and UnPlug ioctls
//

typedef struct _BUSENUM_PLUGIN_HARDWARE
{
    //
    // sizeof (struct _BUSENUM_HARDWARE)
    //
    IN ULONG Size;

    //
    // Unique serial number of the device to be enumerated.
    // Enumeration will be failed if another device on the
    // bus has the same serail number.
    //

    IN ULONG SerialNo;

    //
    // An array of (zero terminated wide character strings). The array itself
    //  also null terminated (ie, MULTI_SZ)
    //
    #pragma warning(disable:4200)  // nonstandard extension used

    IN  WCHAR   HardwareIDs[];

    #pragma warning(default:4200)

} BUSENUM_PLUGIN_HARDWARE, *PBUSENUM_PLUGIN_HARDWARE;

typedef struct _BUSENUM_UNPLUG_HARDWARE
{
    //
    // sizeof (struct _REMOVE_HARDWARE)
    //

    IN ULONG Size;

    //
    // Serial number of the device to be plugged out
    //

    ULONG   SerialNo;

    ULONG Reserved[2];

} BUSENUM_UNPLUG_HARDWARE, *PBUSENUM_UNPLUG_HARDWARE;

typedef struct _BUSENUM_EJECT_HARDWARE
{
    //
    // sizeof (struct _EJECT_HARDWARE)
    //

    IN ULONG Size;

    //
    // Serial number of the device to be ejected
    //

    ULONG   SerialNo;

    ULONG Reserved[2];

} BUSENUM_EJECT_HARDWARE, *PBUSENUM_EJECT_HARDWARE;

#endif
