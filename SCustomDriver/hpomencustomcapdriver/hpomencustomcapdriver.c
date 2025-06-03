/*
   Copyright (c) 2017, HP Development Company, L.P. All rights reserved.
   This software contains confidential and proprietary information of HP.
   The user of this software agrees not to disclose, disseminate or copy
   such Confidential Information and shall use the software only in accordance
   with the terms of the license agreement the user entered into with HP.
*/

#include "hpomencustomcapdriver.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, HPCustCapEvtDeviceAdd)
#pragma alloc_text (PAGE, HPCustCapEvtIoRead)
#pragma alloc_text (PAGE, HPCustCapEvtIoWrite)
#pragma alloc_text (PAGE, HPCustCapEvtIoDeviceControl)
#endif


NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
/*++

Routine Description:
    DriverEntry initializes the driver and is the first routine called by the
    system after the driver is loaded. DriverEntry configures and creates a WDF driver
    object.
    .
Parameters Description:

    DriverObject - represents the instance of the function driver that is loaded
    into memory. DriverObject is allocated by the system before the
    driver is loaded, and it is released by the system after the system unloads
    the function driver from memory.

    RegistryPath - represents the driver specific path in the Registry.
    The function driver can use the path to store driver related data between
    reboots. The path does not store hardware instance specific data.

Return Value:

    STATUS_SUCCESS if successful,
    STATUS_UNSUCCESSFUL otherwise.

--*/
{
    NTSTATUS            status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG   config;

    KdPrint(("Josh HP Custom Capability - Driver Entry.\n"));

    //
    // Initiialize driver config to control the attributes that
    // are global to the driver. Note that framework by default
    // provides a driver unload routine. If DriverEntry creates any resources
    // that require clean-up in driver unload,
    // you can manually override the default by supplying a pointer to the EvtDriverUnload
    // callback in the config structure. In general xxx_CONFIG_INIT macros are provided to
    // initialize most commonly used members.
    //

    WDF_DRIVER_CONFIG_INIT(
        &config,
        HPCustCapEvtDeviceAdd
        );


    //
    // Create a framework driver object to represent our driver.
    //
    status = WdfDriverCreate(
        DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES, // Driver Attributes
        &config,          // Driver Config Info
        WDF_NO_HANDLE
        );

    if (!NT_SUCCESS(status)) {
        KdPrint( ("WdfDriverCreate failed with status 0x%x\n", status));
    }

    return status;
}


NTSTATUS
HPCustCapEvtDeviceAdd(
    IN WDFDRIVER       Driver,
    IN PWDFDEVICE_INIT DeviceInit
    )
/*++
Routine Description:

    HPCustCapEvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a WDF device object to
    represent a new instance of hpomencustomcapdriver device.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    NTSTATUS               status = STATUS_SUCCESS;
    PFDO_DATA              fdoData;
    WDF_IO_QUEUE_CONFIG    queueConfig;
    WDF_OBJECT_ATTRIBUTES  fdoAttributes;
    WDFDEVICE              hDevice;
    WDFQUEUE               queue;
    WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks = { 0 };
    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    KdPrint(("Josh HPCustCapEvtDeviceAdd called\n"));

    KdPrint(("Josh 1 Start To Power call back"));
    /*
     * Initialize PnP-power callbacks, attributes and a context area
     * for the device object.
     */
    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
    pnpPowerCallbacks.EvtDevicePrepareHardware = HPDriverEvtDevicePrepareHardware;
    pnpPowerCallbacks.EvtDeviceReleaseHardware = HPDriverEvtDeviceReleaseHardware;
    pnpPowerCallbacks.EvtDeviceSelfManagedIoSuspend = HPDriver_EvtDeviceSelfManagedIoSuspend;
    pnpPowerCallbacks.EvtDeviceD0Entry = HPDriverEvtDeviceD0Entry;
    pnpPowerCallbacks.EvtDeviceD0Exit = HPDriverEvtDeviceD0Exit;
    /*
     * Register the PnP Callbacks.
     */
    WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);
    KdPrint(("Josh End To Power call back"));

    //
    // Initialize attributes and a context area for the device object.
    //
    //
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&fdoAttributes, FDO_DATA);

    //
    // Create a framework device object.This call will in turn create
    // a WDM device object, attach to the lower stack, and set the
    // appropriate flags and attributes.
    //
    status = WdfDeviceCreate(&DeviceInit, &fdoAttributes, &hDevice);
    if (!NT_SUCCESS(status)) {
        KdPrint( ("WdfDeviceCreate failed with status code 0x%x\n", status));
        return status;
    }

    //
    // Get the device context by using the accessor function specified in
    // the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro for FDO_DATA.
    //
    fdoData = HPCustCapFdoGetData(hDevice);

    //
    // Tell the Framework that this device will need an interface
    //
    status = WdfDeviceCreateDeviceInterface(
                 hDevice,
                 (LPGUID) &GUID_DEVINTERFACE_HPCUSTOMCAP,
                 NULL // ReferenceString
             );

    if (!NT_SUCCESS (status)) {
        KdPrint( ("WdfDeviceCreateDeviceInterface failed 0x%x\n", status));
        return status;
    }

    //
    // Register I/O callbacks to tell the framework that you are interested
    // in handling IRP_MJ_READ, IRP_MJ_WRITE, and IRP_MJ_DEVICE_CONTROL requests.
    // If a specific callback function is not specified for one ofthese,
    // the request will be dispatched to the EvtIoDefault handler, if any.
    // If there is no EvtIoDefault handler, the request will be failed with
    // STATUS_INVALID_DEVICE_REQUEST.
    // WdfIoQueueDispatchParallel means that we are capable of handling
    // all the I/O requests simultaneously and we are responsible for protecting
    // data that could be accessed by these callbacks simultaneously.
    // A default queue gets all the requests that are not
    // configured for forwarding using WdfDeviceConfigureRequestDispatching.
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig,  WdfIoQueueDispatchParallel);

    queueConfig.EvtIoRead = HPCustCapEvtIoRead;
    queueConfig.EvtIoWrite = HPCustCapEvtIoWrite;
    queueConfig.EvtIoDeviceControl = HPCustCapEvtIoDeviceControl;

    //
    // By default, Static Driver Verifier (SDV) displays a warning if it 
    // doesn't find the EvtIoStop callback on a power-managed queue. 
    // The 'assume' below causes SDV to suppress this warning. If the driver 
    // has not explicitly set PowerManaged to WdfFalse, the framework creates
    // power-managed queues when the device is not a filter driver.  Normally 
    // the EvtIoStop is required for power-managed queues, but for this driver
    // it is not needed b/c the driver doesn't hold on to the requests or 
    // forward them to other drivers. This driver completes the requests 
    // directly in the queue's handlers. If the EvtIoStop callback is not 
    // implemented, the framework waits for all driver-owned requests to be
    // done before moving in the Dx/sleep states or before removing the 
    // device, which is the correct behavior for this type of driver.
    // If the requests were taking an indeterminate amount of time to complete,
    // or if the driver forwarded the requests to a lower driver/another stack,
    // the queue should have an EvtIoStop/EvtIoResume.
    //
    __analysis_assume(queueConfig.EvtIoStop != 0);
    status = WdfIoQueueCreate(
        hDevice,
        &queueConfig,
        WDF_NO_OBJECT_ATTRIBUTES,
        &queue
        );
    __analysis_assume(queueConfig.EvtIoStop == 0);

    if (!NT_SUCCESS (status)) {

        KdPrint( ("WdfIoQueueCreate failed 0x%x\n", status));
        return status;
    }

    return status;
}
/**
 * @brief Routine Description:
 * Performs whatever initialization is needed to setup the device, setting up
 * a DMA channel or mapping any I/O port resources.  This will only be called
 * as a device starts or restarts, not every time the device moves into the D0
 * state.  Consequently, most hardware initialization belongs elsewhere.
 *
 * @param Device - A handle to the WDFDEVICE
 * @param Resources - The raw PnP resources associated with the device.
 * Most of the time, these aren't useful for a PCI device.
 * @param ResourcesTranslated - The translated PnP resources associated with
 * the device.  This is what is important to a PCI device.
 *
 * @return NT status code - failure will result in the device stack being
 * torn down
 */
NTSTATUS
HPDriverEvtDevicePrepareHardware(WDFDEVICE Device, WDFCMRESLIST Resources,
    WDFCMRESLIST ResourcesTranslated)
{
    UNREFERENCED_PARAMETER(Resources);
    UNREFERENCED_PARAMETER(ResourcesTranslated);
    PAGED_CODE();
    KdPrint(("Josh HPDriverEvtDevicePrepareHardware... \n"));
    return STATUS_SUCCESS;
}
/**
 * @brief Routine Description:
 * Unmap the resources that were mapped in PLxEvtDevicePrepareHardware.
 * This will only be called when the device stopped for resource rebalance,
 * surprise-removed or query-removed.
 *
 * @param Device - A handle to the WDFDEVICE
 * @param ResourcesTranslated - The translated PnP resources associated with the
 * device.  This is what is important to a PCI device.
 *
 * @Return Value:
 * NT status code - failure will result in the device stack being torn down
 *
 */
NTSTATUS
HPDriverEvtDeviceReleaseHardware(IN WDFDEVICE Device,
    IN WDFCMRESLIST ResourcesTranslated)
{
    UNREFERENCED_PARAMETER(ResourcesTranslated);
    PAGED_CODE();
    KdPrint(("Josh HPDriverEvtDeviceReleaseHardware... \n"));
    return STATUS_SUCCESS;
}
NTSTATUS HPDriver_EvtDeviceSelfManagedIoSuspend(IN WDFDEVICE Device)
{
    UNREFERENCED_PARAMETER(Device);
    KdPrint(("Josh HPDriver_EvtDeviceSelfManagedIoSuspend... \n"));
    return STATUS_SUCCESS;
}
/**
 * @brief Used to configure device on D0 entry
 *
 * @param dev WDF device
 * @param previousState previous power state
 *
 * @return On success, the function returns STATUS_SUCCESS
 * On failure it passes the relevant error code to the caller.
 */
NTSTATUS
HPDriverEvtDeviceD0Entry(IN WDFDEVICE Device,
    IN WDF_POWER_DEVICE_STATE PreviousState)
{
    KdPrint(("Josh HPDriverEvtDeviceD0Entry... %d\n", PreviousState));
    return STATUS_SUCCESS;
}
/**
 * @brief Used to configure device on D0 exit
 *
 * @param Device WDF device
 * @param TargetState Target power state
 *
 * @return On success, the function returns STATUS_SUCCESS
 * On failure it passes the relevant error code to the caller.
 */
NTSTATUS
HPDriverEvtDeviceD0Exit(IN WDFDEVICE Device,
    IN WDF_POWER_DEVICE_STATE TargetState)
{
    PAGED_CODE();
    KdPrint(("Josh HPDriverEvtDeviceD0Exit... %d \n", TargetState));
    return STATUS_SUCCESS;
}


VOID
HPCustCapEvtIoRead (
    WDFQUEUE      Queue,
    WDFREQUEST    Request,
    size_t        Length
    )
/*++

Routine Description:

    Performs read from the hpomencustomcapdriver device. This event is called when the
    framework receives IRP_MJ_READ requests.

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
             I/O request.
    Request - Handle to a framework request object.

    Lenght - Length of the data buffer associated with the request.
                 By default, the queue does not dispatch
                 zero length read & write requests to the driver and instead to
                 complete such requests with status success. So we will never get
                 a zero length request.

Return Value:

  None.

--*/
{
    NTSTATUS    status;
    ULONG_PTR bytesCopied =0;
    WDFMEMORY memory;

    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Length);

    PAGED_CODE();

    KdPrint(( "HPCustCapEvtIoRead: Request: 0x%p, Queue: 0x%p\n",
                                    Request, Queue));

    //
    // Get the request memory and perform read operation here
    //
    status = WdfRequestRetrieveOutputMemory(Request, &memory);
    if(NT_SUCCESS(status) ) {
        //
        // Copy data into the memory buffer using WdfMemoryCopyFromBuffer
        //
    }

    WdfRequestCompleteWithInformation(Request, status, bytesCopied);
}

VOID
HPCustCapEvtIoWrite (
    WDFQUEUE      Queue,
    WDFREQUEST    Request,
    size_t        Length
    )
/*++

Routine Description:

    Performs write to the hpomencustomcapdriver device. This event is called when the
    framework receives IRP_MJ_WRITE requests.

Arguments:

    Queue -  Handle to the framework queue object that is associated with the
            I/O request.
    Request - Handle to a framework request object.

    Lenght - Length of the data buffer associated with the request.
                 The default property of the queue is to not dispatch
                 zero lenght read & write requests to the driver and
                 complete is with status success. So we will never get
                 a zero length request.

Return Value:

   None
--*/

{
    NTSTATUS    status;
    ULONG_PTR bytesWritten =0;
    WDFMEMORY memory;

    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Length);

    KdPrint(("HPCustCapEvtIoWrite. Request: 0x%p, Queue: 0x%p\n",
                                Request, Queue));

    PAGED_CODE();

    //
    // Get the request buffer and perform write operation here
    //
    status = WdfRequestRetrieveInputMemory(Request, &memory);
    if(NT_SUCCESS(status) ) {
        //
        // 1) Use WdfMemoryCopyToBuffer to copy data from the request
        // to driver buffer.
        // 2) Or get the buffer pointer from the request by calling
        // WdfRequestRetrieveInputBuffer
        // 3) Or you can get the buffer pointer from the memory handle
        // by calling WdfMemoryGetBuffer.
        //
        bytesWritten = Length;
    }

    WdfRequestCompleteWithInformation(Request, status, bytesWritten);

}


VOID
HPCustCapEvtIoDeviceControl(
    IN WDFQUEUE     Queue,
    IN WDFREQUEST   Request,
    IN size_t       OutputBufferLength,
    IN size_t       InputBufferLength,
    IN ULONG        IoControlCode
    )
/*++
Routine Description:

    This event is called when the framework receives IRP_MJ_DEVICE_CONTROL
    requests from the system.

Arguments:

    Queue - Handle to the framework queue object that is associated
            with the I/O request.
    Request - Handle to a framework request object.

    OutputBufferLength - length of the request's output buffer,
                        if an output buffer is available.
    InputBufferLength - length of the request's input buffer,
                        if an input buffer is available.

    IoControlCode - the driver-defined or system-defined I/O control code
                    (IOCTL) that is associated with the request.

Return Value:

   VOID

--*/
{
    NTSTATUS  status= STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);

    KdPrint(("HPCustCapEvtIoDeviceControl called\n"));

    PAGED_CODE();

    //
    // Use WdfRequestRetrieveInputBuffer and WdfRequestRetrieveOutputBuffer
    // to get the request buffers.
    //

    switch (IoControlCode) {

    default:
        status = STATUS_INVALID_DEVICE_REQUEST;
    }

    //
    // Complete the Request.
    //
    WdfRequestCompleteWithInformation(Request, status, (ULONG_PTR) 0);
}
