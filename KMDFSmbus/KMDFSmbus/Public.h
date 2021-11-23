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

DEFINE_GUID (GUID_DEVINTERFACE_KMDFSmbus,
    0xb5276199,0x2603,0x4744,0x8e,0xbd,0x87,0x71,0x6d,0xb2,0x32,0x93);
// {b5276199-2603-4744-8ebd-87716db23293}
