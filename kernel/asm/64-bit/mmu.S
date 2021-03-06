/**
 * Copyright 2019 AbbeyCatUK
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



.global							_kernel_mmu_tcr_read
.global							_kernel_mmu_tcr_write
.global							_kernel_mmu_configure_mair
.global							_kernel_mmu_clear_tlb_and_btc
.global							_kernel_mmu_configure_ttbr0
.global							_kernel_mmu_configure_enable
.global							_kernel_mmu_configure_disable
.global							_kernel_mmu_invalidate_tlb
.global							_kernel_mmu_invalidate_btc
.global							_kernel_mmu_clean_and_invalidate_cache_va



							//
							// _kernel_mmu_tcr_read
							//
							// Reads the TCR (Translation Control Register)
							//
							// To access the TCR_EL1:
							//     MRS <Xt>, TCR_EL1 ; Read TCR_EL1 into Xt
							//     MSR TCR_EL1, <Xt> ; Write Xt to TCR_EL1
							//
							// On entry:
							// -
							//
							// On exit:
							// x0 - contains the TCR value
							//
_kernel_mmu_tcr_read:
							MRS			x0			, TCR_EL1
							RET



							//
							// _kernel_mmu_tcr_write
							//
							// Configures the TCR (Translation Control Register)
							//
							// To access the TCR_EL1:
							//     MRS <Xt>, TCR_EL1 ; Read TCR_EL1 into Xt
							//     MSR TCR_EL1, <Xt> ; Write Xt to TCR_EL1
							//
							// On entry:
							// x0 - contains the TCR value
							//
							// On exit:
							// -
							//
_kernel_mmu_tcr_write:
							MSR			TCR_EL1			, x0
							RET



_kernel_mmu_configure_mair:

							// MAIR needs setting up as well I believe
							// [D7-2064] - 0b0100 0b0100 = 0x44, implies outer non-cacheable, inner non-cacheable, 0x00 implies device memory, nGnRnE (strongly ordered IIRC)
							// see https://github.com/bztsrc/raspi3-tutorial/blob/master/10_virtualmemory/mmu.c
							MSR			MAIR_EL1		, x0
							RET



							//
							// _kernel_mmu_clear_tlb_and_btc
							//
							// Clears both the TLB and BTC
							// @todo AArch64 does these both - I/cache invalidation clears branch predictor as well, it seems
							//
							// On entry:
							// -
							//
							// On exit:
							// -
							//
_kernel_mmu_clear_tlb_and_btc:
							STR			lr			, [sp, #-16]!
							DSB			SY
							BL			_kernel_mmu_invalidate_tlb
							BL			_kernel_mmu_invalidate_btc
							DSB			SY
							ISB
							LDR			lr			, [sp]		, #16
							RET
				


							//
							// _kernel_mmu_configure_ttbr0
							//
							// Configures the TTBR0 (Translation Table Base Register 0)
							//
							// DDI0487A - D7-2130
							//
							// To access the TTBR0_EL1:
							// MRS <Xt>, TTBR0_EL1 ; Read TTBR0_EL1 into Xt
							// MSR TTBR0_EL1, <Xt> ; Write Xt to TTBR0_EL1
							//
							// On entry:
							// x0 - contains the TTBR0 value
							//
							// On exit:
							// -
							//
_kernel_mmu_configure_ttbr0:
							MSR			TTBR0_EL1		, x0
							ISB
							RET



							//
							// _kernel_mmu_configure_enable
							//
							// Configures the MMU, enabling required flags
							//
							// Ref: Bare-Metal Boot Process for ARM v8a (DAI0527A)
							//
							// On entry:
							// -
							//
							// On exit:
							// -
							//
_kernel_mmu_configure_enable:

							// http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0500j/index.html
							// The CPUECTLR_EL1 is write accessible in EL1 if:
							// ACTLR_EL3.CPUECTLR is 1 and ACTLR_EL2.CPUECTLR is 1, or
							// ACTLR_EL3.CPUECTLR is 1 and SCR.NS is 0.
							//
							// CPUECTLR_EL1 = CPU Extended Control Register (EL1)
							// ACTLR_EL3    = Auxiliary Control Register (EL3), with CPUECTLR

							MRS			x0			, S3_1_C15_C2_1						// Implemented in the CPUECTLR register
							ORR			x0			, x0		, #(0x1 <<  6)				// SMP bit
							MSR			S3_1_C15_C2_1		, x0

							
							MRS			x0			, SCTLR_EL1						// Enable caches and the MMU:
							ORR			x0			, x0		, #(0x1 <<  2)				// C bit (data cache)
							ORR			x0			, x0		, #(0x1 << 12)				// I bit (instruction cache)
							ORR			x0			, x0		, #(0x1 <<  0)				// M bit (MMU)
							MSR			SCTLR_EL1		, x0
							
							DSB			SY
							ISB
							RET



							//
							// _kernel_mmu_configure_disable
							//
							// Configures the MMU, enabling required flags
							//
							// Ref: Bare-Metal Boot Process for ARM v8a (DAI0527A)
							//
							// On entry:
							// -
							//
							// On exit:
							// -
							//
_kernel_mmu_configure_disable:
							MRS			x0			, S3_1_C15_C2_1						// Implemented in the CPUECTLR register
							BIC			x0			, x0		, #(0x1 <<  6)				// SMPEN bit
							MSR			S3_1_C15_C2_1		, x0

							MRS			x0			, SCTLR_EL1						// Enable caches and the MMU:
							BIC			x0			, x0		, #(0x1 <<  2)				// C bit (data cache)
							BIC			x0			, x0		, #(0x1 << 12)				// I bit (instruction cache)
							BIC			x0			, x0		, #(0x1 <<  0)				// M bit (MMU)
							MSR			SCTLR_EL1		, x0
							
							DSB			SY
							ISB
							RET



							//
							// _kernel_mmu_invalidate_tlb
							//
							// Invalidates ("flushes") the TLB (Translation Lookaside Buffer)
							//
							// Ref: ARM DDI 0487A.f [C5-393] "A64 system instructions for TLB maintenance"
							// Ref: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0488d/CIHGGBGB.html
							//
							// On entry:
							// -
							//
							// On exit:
							// r0 = corrupted
							//
_kernel_mmu_invalidate_tlb:
							TLBI			VMALLE1IS
							RET



							//
							// _kernel_mmu_invalidate_btc
							//
							// Invalidates ("flushes") the BTC (Branch Target Cache)
							//
							// Ref: ARM DDI 0487A.f [G6-4320] "BPIMVA, Branch Predictor Invalidate by VA"
							// It seems Branch Target Cache is - in AArch64 - referred to as "Branch Predictor"
							//
							// IC IALLU => Instruction Cache Invalidate All to PoU
							//
							// On entry:
							// -
							//
							// On exit:
							// r0 = corrupted
							//
_kernel_mmu_invalidate_btc:
							IC			IALLU										// [G6.2.73] also invalidates the branch predictor
							RET



							//
							// On entry:
							// x0 = virtual address
							//
							// Ref: ARM DDI 0487A.f [C5-318] "DC CIVAC, Data or unified Cache line Clean and Invalidate by VA to PoC"
							//
_kernel_mmu_clean_and_invalidate_cache_va:

							DC CIVAC, x0	// [C5-318]
							RET


