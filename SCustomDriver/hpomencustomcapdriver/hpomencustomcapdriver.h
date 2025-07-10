/*
   Copyright (c) 2017, HP Development Company, L.P. All rights reserved.
   This software contains confidential and proprietary information of HP.
   The user of this software agrees not to disclose, disseminate or copy
   such Confidential Information and shall use the software only in accordance
   with the terms of the license agreement the user entered into with HP.
*/


#if !defined(_HPCUSTCAP_H_)
#define _HPCUSTCAP_H_

#include <ntddk.h>
#include <wdf.h>

#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#include "wmilib.h"
#include <initguid.h>
#include "driver.h"
#include "public.h"

// For Featured driver only
#ifdef HPCUSTCAP_FUNC_FEATURED
#include <wpprecorder.h>
#endif // HPCUSTCAP_FUNC_FEATURED

#define HPCUSTCAP_POOL_TAG (ULONG) 'ucpH'

#define MOFRESOURCENAME L"HPCustCapWMI"

#define HPCUSTCAP_FUNC_DEVICE_LOG_ID "HPCustCapDevice"
//
// The device extension for the device object
//
typedef struct _FDO_DATA
{

    WDFWMIINSTANCE WmiDeviceArrivalEvent;

    BOOLEAN     WmiPowerDeviceEnableRegistered;

    HPCUSTOMCAP_INTERFACE_STANDARD BusInterface;

// For Featured driver only
#ifdef HPCUSTCAP_FUNC_FEATURED
    RECORDER_LOG    WppRecorderLog;
#endif // HPCUSTCAP_FUNC_FEATURED

}  FDO_DATA, *PFDO_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FDO_DATA, HPCustCapFdoGetData)


//
// Connector Types
//

#define HPCUSTCAP_WMI_STD_I8042 0
#define HPCUSTCAP_WMI_STD_SERIAL 1
#define HPCUSTCAP_WMI_STD_PARALEL 2
#define HPCUSTCAP_WMI_STD_USB 3

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_UNLOAD HPCustCapEvtDriverUnload;

EVT_WDF_DRIVER_DEVICE_ADD HPCustCapEvtDeviceAdd;

EVT_WDF_DEVICE_CONTEXT_CLEANUP HPCustCapEvtDeviceContextCleanup;
EVT_WDF_DEVICE_D0_ENTRY HPCustCapEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT HPCustCapEvtDeviceD0Exit;
EVT_WDF_DEVICE_PREPARE_HARDWARE HPCustCapEvtDevicePrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE HPCustCapEvtDeviceReleaseHardware;

VOID TestEditRegistry();

EVT_WDF_DEVICE_SELF_MANAGED_IO_INIT HPCustCapEvtDeviceSelfManagedIoInit;

//
// Io events callbacks.
//
EVT_WDF_IO_QUEUE_IO_READ HPCustCapEvtIoRead;
EVT_WDF_IO_QUEUE_IO_WRITE HPCustCapEvtIoWrite;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL HPCustCapEvtIoDeviceControl;
EVT_WDF_DEVICE_FILE_CREATE HPCustCapEvtDeviceFileCreate;
EVT_WDF_FILE_CLOSE HPCustCapEvtFileClose;

NTSTATUS
HPCustCapWmiRegistration(
    _In_ WDFDEVICE Device
    );

//
// Windows Power Events callbacks
//
EVT_WDF_DEVICE_D0_ENTRY HPDriverEvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT HPDriverEvtDeviceD0Exit;
EVT_WDF_DEVICE_SELF_MANAGED_IO_SUSPEND HPDriver_EvtDeviceSelfManagedIoSuspend;
EVT_WDF_DEVICE_PREPARE_HARDWARE HPDriverEvtDevicePrepareHardware;
EVT_WDF_DEVICE_RELEASE_HARDWARE HPDriverEvtDeviceReleaseHardware;

//
// Power events callbacks
//
EVT_WDF_DEVICE_ARM_WAKE_FROM_S0 HPCustCapEvtDeviceArmWakeFromS0;
EVT_WDF_DEVICE_ARM_WAKE_FROM_SX HPCustCapEvtDeviceArmWakeFromSx;
EVT_WDF_DEVICE_DISARM_WAKE_FROM_S0 HPCustCapEvtDeviceDisarmWakeFromS0;
EVT_WDF_DEVICE_DISARM_WAKE_FROM_SX HPCustCapEvtDeviceDisarmWakeFromSx;
EVT_WDF_DEVICE_WAKE_FROM_S0_TRIGGERED HPCustCapEvtDeviceWakeFromS0Triggered;
EVT_WDF_DEVICE_WAKE_FROM_SX_TRIGGERED HPCustCapEvtDeviceWakeFromSxTriggered;

PCHAR
DbgDevicePowerString(
    IN WDF_POWER_DEVICE_STATE Type
    );

//
// WMI event callbacks
//
EVT_WDF_WMI_INSTANCE_QUERY_INSTANCE EvtWmiInstanceStdDeviceDataQueryInstance;
EVT_WDF_WMI_INSTANCE_QUERY_INSTANCE EvtWmiInstanceHPCustCapControlQueryInstance;
EVT_WDF_WMI_INSTANCE_SET_INSTANCE EvtWmiInstanceStdDeviceDataSetInstance;
EVT_WDF_WMI_INSTANCE_SET_INSTANCE EvtWmiInstanceHPCustCapControlSetInstance;
EVT_WDF_WMI_INSTANCE_SET_ITEM EvtWmiInstanceHPCustCapControlSetItem;
EVT_WDF_WMI_INSTANCE_SET_ITEM EvtWmiInstanceStdDeviceDataSetItem;
EVT_WDF_WMI_INSTANCE_EXECUTE_METHOD EvtWmiInstanceHPCustCapControlExecuteMethod;

NTSTATUS
HPCustCapFireArrivalEvent(
    _In_ WDFDEVICE  Device
    );

#endif  // _HPCUSTCAP_H_
