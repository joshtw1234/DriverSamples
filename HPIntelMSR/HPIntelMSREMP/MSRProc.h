#include <wdm.h>


/** A Utility to generate millisecond time unit. */
#define	MILLISECOND(x)		((ULONG)(-10 * 1000 * (x)))
/** A Utility to generate second time unit. */
#define	SECOND(x)		((ULONG)(-10 * 1000 * 1000 * (x)))
/** A Utility to assign relative time for LARGE_INTEGER. */
#define	SET_RELATIVE_TIME(x, s)	(x.HighPart = -1, x.LowPart = (s))

#define SLEEP_TIME 2000

BOOLEAN tStart;
/*
 * declared for thread routine.
 */
KSTART_ROUTINE IntelMSRDemoThread;

BOOLEAN mSleep(IN ULONG ulValue);