/*++

Module Name:

    ControlDevice.c

Abstract:

    This file contains the control device entry points and callbacks.

Environment:

    Kernel-mode Driver Framework

--*/

#include "driver.h"
#include "ControlDevice.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, KMDFPnpDeviceDriverCreateControlDevice)
#pragma alloc_text (PAGE, KMDFPnpDeviceDriverDeleteControlDevice)
#pragma alloc_text (PAGE, ControlDeviceDriverShutDown)
#endif

#pragma warning(push)
#pragma warning(disable:28118) // this callback will run at IRQL=PASSIVE_LEVEL
_Use_decl_annotations_
NTSTATUS 
KMDFPnpDeviceDriverCreateControlDevice(
    WDFDEVICE Device
)
{
    NTSTATUS                    status;
    PWDFDEVICE_INIT             pInit = NULL;
    WDFDEVICE                   controlDevice = NULL;
    WDF_OBJECT_ATTRIBUTES       controlAttributes;
    BOOLEAN                     bCreate = FALSE;
    PCONTROL_DEVICE_EXTENSION   pCtrlDeviceContext;

    DECLARE_CONST_UNICODE_STRING(ntDeviceName, NTDEVICE_NAME_STRING);
    DECLARE_CONST_UNICODE_STRING(symbolicLinkName, SYMBOLIC_NAME_STRING);

    PAGED_CODE();
    //
    // First find out whether any ControlDevice has been created. If the
    // collection has more than one device then we know somebody has already
    // created or in the process of creating the device.
    // 
    WdfWaitLockAcquire(DriverDeviceCollectionLock, NULL);

    if (WdfCollectionGetCount(DriverDeviceCollection) == 1) {
        bCreate = TRUE;
    }

    WdfWaitLockRelease(DriverDeviceCollectionLock);

    if (!bCreate) {
        //
        // Control device is already created. So return success.
        //
        return STATUS_SUCCESS;
    }
    //
    //
    // In order to create a control device, we first need to allocate a
    // WDFDEVICE_INIT structure and set all properties.
    //
    pInit = WdfControlDeviceInitAllocate(WdfDeviceGetDriver(Device),
                                         &SDDL_DEVOBJ_SYS_ALL_ADM_RWX_WORLD_RW_RES_R
    );

    if (pInit == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        goto Error;
    }
    //
    // Set exclusive to false so that more than one app can talk to the
    // control device simultaneously.
    //
    WdfDeviceInitSetExclusive(pInit, FALSE);

    status = WdfDeviceInitAssignName(pInit, &ntDeviceName);

    if (!NT_SUCCESS(status)) {
        goto Error;
    }
    /*
    * Register Restart event.
    */
    WdfControlDeviceInitSetShutdownNotification(pInit,
                                                ControlDeviceDriverShutDown,
                                                WdfDeviceShutdown);

    //
    // Specify the size of device context
    //
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&controlAttributes, CONTROL_DEVICE_EXTENSION);
    status = WdfDeviceCreate(&pInit,
                             &controlAttributes,
                             &controlDevice);
    if (!NT_SUCCESS(status)) {
        goto Error;
    }
    pCtrlDeviceContext = ControlDeviceGetContext(controlDevice);
    pCtrlDeviceContext->ControlData = NULL; // Initialize control data
    //
    // Create a symbolic link for the control object so that usermode can open
    // the device.
    //
    status = WdfDeviceCreateSymbolicLink(controlDevice, &symbolicLinkName);

    if (!NT_SUCCESS(status)) {
        goto Error;
    }
    //
    // Control devices must notify WDF when they are done initializing.   I/O is
    // rejected until this call is made.
    //
    WdfControlFinishInitializing(controlDevice);

    ControlDevice = controlDevice;
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%ws %!FUNC! Exit %!STATUS!", LOG_TITLE, status);
    KdPrint(("%ws HPBaseDriverCreateControlDevice...Exit 0x%x\n", LOG_TITLE, status));
    return STATUS_SUCCESS;
Error:

    if (pInit != NULL) {
        WdfDeviceInitFree(pInit);
    }

    if (controlDevice != NULL) {
        //
        // Release the reference on the newly created object, since
        // we couldn't initialize it.
        //
        WdfObjectDelete(controlDevice);
        controlDevice = NULL;
    }
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%ws %!FUNC! Error Exit %!STATUS!", LOG_TITLE, status);
    KdPrint(("%ws KMDFPnpDeviceDriverCreateControlDevice...Error Exit 0x%x\n", LOG_TITLE, status));

    return status;
}

VOID KMDFPnpDeviceDriverDeleteControlDevice(
    WDFDEVICE Device
)
/*++

Routine Description:

    This routine deletes the control by doing a simple dereference.

Arguments:

    Device - Handle to a framework pnp device object.

Return Value:

    WDF status code

--*/
{
    UNREFERENCED_PARAMETER(Device);

    PAGED_CODE();

    if (ControlDevice) {
        WdfObjectDelete(ControlDevice);
        ControlDevice = NULL;
        TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%ws %!FUNC! Exit", LOG_TITLE);
        KdPrint(("%ws KMDFPnpDeviceDriverDeleteControlDevice...Exit \n", LOG_TITLE));
    }
}
VOID
ControlDeviceDriverShutDown(
    WDFDEVICE Device
)
/*++

Routine Description:
    Callback invoked when the machine is shutting down.  If you register for
    a last chance shutdown notification you cannot do the following:
    o Call any pageable routines
    o Access pageable memory
    o Perform any file I/O operations

    If you register for a normal shutdown notification, all of these are
    available to you.

    This function implementation does nothing, but if you had any outstanding
    file handles open, this is where you would close them.

Arguments:
    Device - The device which registered the notification during init

Return Value:
    None

  --*/
{
    UNREFERENCED_PARAMETER(Device);
    TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DRIVER, "%!FUNC! Driver Shutdown");
    KdPrint(("ControlDeviceDriverShutDown... \n"));
    //SetWatchdogRegistry();
    return;
}
#pragma warning(pop) // enable 28118 again
