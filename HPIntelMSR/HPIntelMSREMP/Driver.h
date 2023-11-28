#include <ntddk.h>
#include <wdf.h>
#include "MSRProc.h"


DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD KmdfHelloWorldEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP HPIntelMSREvtDriverContextCleanup;

