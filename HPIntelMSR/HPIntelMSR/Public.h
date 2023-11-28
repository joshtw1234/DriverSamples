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

DEFINE_GUID (GUID_DEVINTERFACE_HPIntelMSR,
    0x3f0b2a41,0x8b08,0x4a65,0x90,0x14,0xc5,0xf2,0x3f,0xa3,0xda,0x6b);
// {3f0b2a41-8b08-4a65-9014-c5f23fa3da6b}
