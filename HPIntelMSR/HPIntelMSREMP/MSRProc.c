#include "MSRProc.h"



VOID IntelMSRDemoThread(__in PVOID StartContext)
{
	tStart = FALSE;
	while (1)
	{
		if (tStart)
		{
			KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: IntelMSRDemoThread STOP\n"));
			break;
		}
		KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_INFO_LEVEL, "KmdfHelloWorld: IntelMSRDemoThread\n"));
		mSleep(SLEEP_TIME);
	}

	PsTerminateSystemThread(0);
}

/**
 * @brief Millisecond sleep.
 *
 * @param ulValue The delay value.
 */
BOOLEAN
mSleep(IN ULONG ulValue)
{
	NTSTATUS Status;
	LARGE_INTEGER DelayTime;

	if (ulValue == 0) {
		return (STATUS_SUCCESS);
	}

	SET_RELATIVE_TIME(DelayTime, MILLISECOND(ulValue));
	Status = KeDelayExecutionThread(KernelMode, FALSE, &DelayTime);

	return (NT_SUCCESS(Status));
}