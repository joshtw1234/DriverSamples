/*
   Copyright (c) 2017, HP Development Company, L.P. All rights reserved.
   This software contains confidential and proprietary information of HP.
   The user of this software agrees not to disclose, disseminate or copy
   such Confidential Information and shall use the software only in accordance
   with the terms of the license agreement the user entered into with HP.
*/

//
// Define an Interface Guid to access the proprietary hpcustomcapdriver interface.
// This guid is used to identify a specific interface in IRP_MN_QUERY_INTERFACE
// handler.
//

DEFINE_GUID(GUID_HPCUSTOMCAP_INTERFACE_STANDARD,
	0x141f0b1b, 0x6ad5, 0x404d, 0x9a, 0x8c, 0x79, 0x54, 0xba, 0x3d, 0x9a, 0x7a);
// {141F0B1B-6AD5-404D-9A8C-7954BA3D9A7A} //Generated new

//
// GUID definition are required to be outside of header inclusion pragma to avoid
// error during precompiled headers.
//

#ifndef __DRIVER_H
#define __DRIVER_H

//
// Define Interface reference/dereference routines for
//  Interfaces exported by IRP_MN_QUERY_INTERFACE
//

typedef VOID (*PINTERFACE_REFERENCE)(PVOID Context);
typedef VOID (*PINTERFACE_DEREFERENCE)(PVOID Context);

typedef
BOOLEAN
(*PHPCUSTOMCAP_GET_CRISPINESS_LEVEL)(
                           IN   PVOID Context,
                           OUT  PUCHAR Level
                               );

typedef
BOOLEAN
(*PHPCUSTOMCAP_SET_CRISPINESS_LEVEL)(
                           IN   PVOID Context,
                           OUT  UCHAR Level
                               );

typedef
BOOLEAN
(*PHPCUSTOMCAP_IS_CHILD_PROTECTED)(
                             IN PVOID Context
                             );

//
// Interface for getting and setting power level etc.,
//
typedef struct _HPCUSTOMCAP_INTERFACE_STANDARD {
    INTERFACE                        InterfaceHeader;
    PHPCUSTOMCAP_GET_CRISPINESS_LEVEL    GetCrispinessLevel;
    PHPCUSTOMCAP_SET_CRISPINESS_LEVEL    SetCrispinessLevel;
    PHPCUSTOMCAP_IS_CHILD_PROTECTED      IsSafetyLockEnabled; //):
} HPCUSTOMCAP_INTERFACE_STANDARD, *PHPCUSTOMCAP_INTERFACE_STANDARD;


#endif
