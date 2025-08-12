/*++

Module Name:

    ControlDevice.h

Abstract:

    This file contains the Control device definitions.

Environment:

    Kernel-mode Driver Framework

--*/

EXTERN_C_START
typedef struct _CONTROL_DEVICE_EXTENSION {

    PVOID   ControlData; // Store your control data here

} CONTROL_DEVICE_EXTENSION, * PCONTROL_DEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(CONTROL_DEVICE_EXTENSION, ControlDeviceGetContext)

//
// Windows Restart event
//
EVT_WDF_DEVICE_SHUTDOWN_NOTIFICATION ControlDeviceDriverShutDown;

/// <summary>
/// The Create Control Device
/// </summary>
_Must_inspect_result_
_Success_(return == STATUS_SUCCESS)
_IRQL_requires_max_(PASSIVE_LEVEL)
NTSTATUS
KMDFPnpDeviceDriverCreateControlDevice(
    _In_ WDFDEVICE Device
);
/// <summary>
/// The Delete Control Device
/// </summary>
_IRQL_requires_max_(PASSIVE_LEVEL)
VOID
KMDFPnpDeviceDriverDeleteControlDevice(
    _In_ WDFDEVICE Device
);

EXTERN_C_END

