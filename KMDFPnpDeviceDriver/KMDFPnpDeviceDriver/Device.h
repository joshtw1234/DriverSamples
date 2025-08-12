/*++

Module Name:

    device.h

Abstract:

    This file contains the device definitions.

Environment:

    Kernel-mode Driver Framework

--*/



EXTERN_C_START

//
// The device context performs the same job as
// a WDM device extension in the driver frameworks
//
typedef struct _DEVICE_CONTEXT
{
    ULONG PrivateDeviceData;  // just a placeholder

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

//
// This macro will generate an inline function called DeviceGetContext
// which will be used to get a pointer to the device context memory
// in a type safe manner.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, PnpDeviceGetContext)

//
// Function to initialize the device and its callbacks
//
NTSTATUS
KMDFPnpDeviceDriverCreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
    );
/// <summary>
/// Pnp device event for context cleanup
/// </summary>
EVT_WDF_DEVICE_CONTEXT_CLEANUP PnpDeviceEvtDeviceContextCleanup;
/// <summary>
/// Pnp Device Power Events
/// </summary>
EVT_WDF_DEVICE_D0_ENTRY PnpDeviceEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT PnpDeviceEvtDeviceD0Exit;
EXTERN_C_END
