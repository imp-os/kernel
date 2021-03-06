//
// synchronize.c
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2015  R. Stange <rsta2@o2online.de>
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <stdint.h>
#include "../../uspi/include/uspi/synchronize.h"

#include "../../uspi/include/uspi/assert.h"
#include "../../uspi/include/uspi/types.h"

// ACU - 32/64-bit [interrupt enable instructions]
#if ISA_TYPE == 64
	// 64-bit
	#define	EnableInterrupts()	__asm volatile ("MSR			DAIFClr			, #0x7")
	#define	DisableInterrupts()	__asm volatile ("MSR			DAIFClr			, #0x0")
#else
	// 32-bit
	#define	EnableInterrupts()	__asm volatile ("cpsie i")
	#define	DisableInterrupts()	__asm volatile ("cpsid i")
#endif
extern void			_kernel_mmu_clean_and_invalidate_cache_va( uintptr_t va );
static volatile unsigned s_nCriticalLevel = 0;
static volatile boolean s_bWereEnabled;

void uspi_EnterCritical (void)
{

	u32 nFlags;
	// ACU - 32/64-bit [interrupt flag instruction]
	#if ISA_TYPE == 64
		// 64-bit
		__asm volatile ("mrs %0, daif" : "=r" (nFlags));
	#else
		// 32-bit
		__asm volatile ("mrs %0, cpsr" : "=r" (nFlags));
	#endif

	DisableInterrupts ();

	if (s_nCriticalLevel++ == 0)
	{
		s_bWereEnabled = nFlags & 0x80 ? FALSE : TRUE;
	}

	DataMemBarrier ();

}

void uspi_LeaveCritical (void)
{

	// ACU

	DataMemBarrier ();

	if (--s_nCriticalLevel == 0)
	{
		if (s_bWereEnabled)
		{
			EnableInterrupts ();
		}
	}

}
#if RASPPI == 1

//
// Cache maintenance operations for ARMv6
//
// NOTE: The following functions should hold all variables in CPU registers. Currently this will be
//	 ensured using maximum optimation (see circle/synchronize.h).
//
//	 The following numbers can be determined (dynamically) using CTR.
//	 As long we use the ARM1176JZF-S implementation in the BCM2835 these static values will work:
//

#define DATA_CACHE_LINE_LENGTH		32

void uspi_CleanAndInvalidateDataCacheRange (u32 nAddress, u32 nLength)
{

	// ACU
	return;

	/*
	nLength += DATA_CACHE_LINE_LENGTH;

	while (1)
	{
		asm volatile ("mcr p15, 0, %0, c7, c14,  1" : : "r" (nAddress) : "memory");

		if (nLength < DATA_CACHE_LINE_LENGTH)
		{
			break;
		}

		nAddress += DATA_CACHE_LINE_LENGTH;
		nLength  -= DATA_CACHE_LINE_LENGTH;
	}
	*/

}

#else

//
// Cache maintenance operations for ARMv7-A
//
// See: ARMv7-A Architecture Reference Manual, Section B4.2.1
//
// NOTE: The following functions should hold all variables in CPU registers. Currently this will be
//	 ensured using the register keyword and maximum optimation (see uspi/synchronize.h).
//
//	 The following numbers can be determined (dynamically) using CTR, CSSELR, CCSIDR and CLIDR.
//	 As long we use the Cortex-A7 implementation in the BCM2836 or the Cortex-A53 implementation
//	 in the BCM2837 these static values will work:
//

#define L1_DATA_CACHE_LINE_LENGTH	64
#define L2_CACHE_LINE_LENGTH		64
#define DATA_CACHE_LINE_LENGTH_MIN	64		// min(L1_DATA_CACHE_LINE_LENGTH, L2_CACHE_LINE_LENGTH)

void uspi_CleanAndInvalidateDataCacheRange (uintptr_t nAddress, u32 nLength)
{

	// ACU

	nLength += DATA_CACHE_LINE_LENGTH_MIN;

	while (1)
	{

		// call kernel's one
		_kernel_mmu_clean_and_invalidate_cache_va( nAddress );
		// __asm volatile ("mcr p15, 0, %0, c7, c14,  1" : : "r" (nAddress) : "memory");	// DCCIMVAC

		if (nLength < DATA_CACHE_LINE_LENGTH_MIN)
		{
			break;
		}

		nAddress += DATA_CACHE_LINE_LENGTH_MIN;
		nLength  -= DATA_CACHE_LINE_LENGTH_MIN;
	}

}

#endif
