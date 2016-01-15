// Copyright (C) 2015,2016 Anton Lauridsen
//
// This file is part of libPerfCounter.
//
// libPerfCounter is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// libPerfCounter is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public
// License along with libPerfCounter. If not, see <http://www.gnu.org/licenses/>.

//------------------------------------------------------------------
//                   !!! WARNING !!! 
//
// This file is auto generated by ctrpp.exe utility from performance
// counters manifest:
//
// ..\BuildTools\CounterGen\Counters.man
//
// It should be regenerated every time the code is built.
// Do not check it in.
//-------------------------------------------------------------------

#pragma once

#include <winperf.h>
#include <perflib.h>


EXTERN_C DECLSPEC_SELECTANY GUID YardmasterGuid = { 0xab8e1320, 0x965a, 0x4cf9, 0x9c, 0x7, 0xfe, 0x25, 0x37, 0x8c, 0x2a, 0x23 };

EXTERN_C DECLSPEC_SELECTANY GUID PerformanceGuid = { 0xdd36a036, 0xc923, 0x4794, 0xb6, 0x96, 0x70, 0x57, 0x76, 0x30, 0xb5, 0xcf };


EXTERN_C DECLSPEC_SELECTANY HANDLE Yardmaster = NULL;

EXTERN_C DECLSPEC_SELECTANY struct {
    PERF_COUNTERSET_INFO CounterSet;
    PERF_COUNTER_INFO Counter0;
    PERF_COUNTER_INFO Counter1;
} PerformanceInfo = {
    { { 0xdd36a036, 0xc923, 0x4794, 0xb6, 0x96, 0x70, 0x57, 0x76, 0x30, 0xb5, 0xcf }, { 0xab8e1320, 0x965a, 0x4cf9, 0x9c, 0x7, 0xfe, 0x25, 0x37, 0x8c, 0x2a, 0x23 }, 2, PERF_COUNTERSET_SINGLE_INSTANCE },
    { 1, PERF_COUNTER_RAWCOUNT, PERF_ATTRIB_BY_REFERENCE, sizeof(ULONG), PERF_DETAIL_NOVICE, 0, 0 },
    { 2, PERF_RAW_BASE, PERF_ATTRIB_BY_REFERENCE, sizeof(ULONG), PERF_DETAIL_NOVICE, 0, 0 },
};

EXTERN_C FORCEINLINE
VOID
CounterCleanup(
    VOID
    )
{
    if (Yardmaster != NULL) {
        PerfStopProvider(Yardmaster);
        Yardmaster = NULL;
    }
}

EXTERN_C FORCEINLINE
ULONG
CounterInitialize(
    __in_opt PERFLIBREQUEST NotificationCallback,
    __in_opt PERF_MEM_ALLOC MemoryAllocationFunction,
    __in_opt PERF_MEM_FREE MemoryFreeFunction,
    __inout_opt PVOID MemoryFunctionsContext
    )
{
    ULONG Status;
    PERF_PROVIDER_CONTEXT ProviderContext;

    ZeroMemory(&ProviderContext, sizeof(PERF_PROVIDER_CONTEXT));
    ProviderContext.ContextSize = sizeof(PERF_PROVIDER_CONTEXT);
    ProviderContext.ControlCallback = NotificationCallback;
    ProviderContext.MemAllocRoutine = MemoryAllocationFunction;
    ProviderContext.MemFreeRoutine = MemoryFreeFunction;
    ProviderContext.pMemContext = MemoryFunctionsContext;

    Status = PerfStartProviderEx(&YardmasterGuid,
                                 &ProviderContext,
                                 &Yardmaster);
    if (Status != ERROR_SUCCESS) {
        Yardmaster = NULL;
        return Status;
    }

    Status = PerfSetCounterSetInfo(Yardmaster,
                                   &PerformanceInfo.CounterSet,
                                   sizeof PerformanceInfo);
    if (Status != ERROR_SUCCESS) {
        CounterCleanup();
        return Status;
    }
    return ERROR_SUCCESS;
}
