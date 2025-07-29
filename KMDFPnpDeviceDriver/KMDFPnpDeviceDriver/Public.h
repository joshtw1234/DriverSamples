/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_KMDFPnpDeviceDriver,
    0x6bbb1340,0x07f4,0x46f7,0x8c,0xc3,0x85,0x13,0x86,0xb5,0x6b,0xe3);
// {6bbb1340-07f4-46f7-8cc3-851386b56be3}
