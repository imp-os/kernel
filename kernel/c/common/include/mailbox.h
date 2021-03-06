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



#include "define.h"
#include <stdint.h>



#ifndef MAILBOX_H
#define MAILBOX_H



	#define		ARM_MAILBOX_REGISTERS							( ARM_PERIPHERAL_BASE_ADDRESS + 0xb880 )
	#define 	ARM_MAILBOX_READ_REGISTER 						( ARM_MAILBOX_REGISTERS + 0x00 )
	#define 	ARM_MAILBOX_STATUS_REGISTER 					( ARM_MAILBOX_REGISTERS + 0x18 )
	#define 	ARM_MAILBOX_WRITE_REGISTER 						( ARM_MAILBOX_REGISTERS + 0x20 )
	#define 	ARM_MAILBOX_STATUS_FULL 						0x80000000
	#define 	ARM_MAILBOX_STATUS_EMPTY						0x40000000

	#define		ARM_MAILBOX_TAG_TERMINATOR						0x00000000
	#define		ARM_MAILBOX_TAG_FIRMWARE_REVISION				0x00000001
	#define		ARM_MAILBOX_TAG_BOARD_MODEL						0x00010001
	#define		ARM_MAILBOX_TAG_BOARD_REVISION					0x00010002
	#define		ARM_MAILBOX_TAG_BOARD_MAC_ADDRESS				0x00010003
	#define		ARM_MAILBOX_TAG_BOARD_SERIAL					0x00010004
	#define		ARM_MAILBOX_TAG_ARM_MEMORY						0x00010005
	#define		ARM_MAILBOX_TAG_VIDEOCORE_MEMORY				0x00010006
	#define		ARM_MAILBOX_TAG_CLOCKS							0x00010007
	#define		ARM_MAILBOX_TAG_COMMAND_LINE					0x00050001
	#define		ARM_MAILBOX_TAG_DMA_CHANNELS					0x00060001
	#define		ARM_MAILBOX_TAG_POWER_STATE						0x00020001
	#define		ARM_MAILBOX_TAG_TIMING							0x00020002
	#define		ARM_MAILBOX_TAG_SET_POWER_STATE					0x00028001
	#define		ARM_MAILBOX_TAG_CLOCK_STATE						0x00030001
	#define		ARM_MAILBOX_TAG_SET_CLOCK_STATE					0x00038001
	#define		ARM_MAILBOX_TAG_CLOCK_RATE						0x00030002
	#define		ARM_MAILBOX_TAG_SET_CLOCK_RATE					0x00038002
	#define		ARM_MAILBOX_TAG_MAX_CLOCK_RATE					0x00030004
	#define		ARM_MAILBOX_TAG_MIN_CLOCK_RATE					0x00030007
	#define		ARM_MAILBOX_TAG_TURBO							0x00030009
	#define		ARM_MAILBOX_TAG_SET_TURBO						0x00038009
	#define		ARM_MAILBOX_TAG_VOLTAGE							0x00030003
	#define		ARM_MAILBOX_TAG_SET_VOLTAGE						0x00038003
	#define		ARM_MAILBOX_TAG_MAX_VOLTAGE						0x00030005
	#define		ARM_MAILBOX_TAG_MIN_VOLTAGE						0x00030008
	#define		ARM_MAILBOX_TAG_TEMPERATURE						0x00030006
	#define		ARM_MAILBOX_TAG_MAX_TEMPERATURE					0x0003000a
	#define		ARM_MAILBOX_TAG_ALLOCATE_MEMORY					0x0003000c
	#define		ARM_MAILBOX_TAG_LOCK_MEMORY						0x0003000d
	#define		ARM_MAILBOX_TAG_UNLOCK_MEMORY					0x0003000e
	#define		ARM_MAILBOX_TAG_RELEASE_MEMORY					0x0003000f
	#define		ARM_MAILBOX_TAG_EXECUTE_CODE					0x00030010
	#define		ARM_MAILBOX_TAG_DISPMANX_RESOURCE_MEM_HANDLE	0x00030014
	#define		ARM_MAILBOX_TAG_EDID_BLOCK						0x00030020
	#define		ARM_MAILBOX_TAG_ALLOCATE_BUFFER					0x00040001
	#define		ARM_MAILBOX_TAG_RELEASE_BUFFER					0x00048001
	#define		ARM_MAILBOX_TAG_BLANK_SCREEN					0x00040002
	#define		ARM_MAILBOX_TAG_PHYSICAL_WIDTH_HEIGHT			0x00040003
	#define		ARM_MAILBOX_TAG_TEST_PHYSICAL_WIDTH_HEIGHT		0x00044003
	#define		ARM_MAILBOX_TAG_SET_PHYSICAL_WIDTH_HEIGHT		0x00048003
	#define		ARM_MAILBOX_TAG_VIRTUAL_WIDTH_HEIGHT			0x00040004
	#define		ARM_MAILBOX_TAG_TEST_VIRTUAL_WIDTH_HEIGHT		0x00044004
	#define		ARM_MAILBOX_TAG_SET_VIRTUAL_WIDTH_HEIGHT		0x00048004
	#define		ARM_MAILBOX_TAG_DEPTH							0x00040005
	#define		ARM_MAILBOX_TAG_TEST_DEPTH						0x00044005
	#define		ARM_MAILBOX_TAG_SET_DEPTH						0x00048005
	#define		ARM_MAILBOX_TAG_PIXEL_ORDER						0x00040006
	#define		ARM_MAILBOX_TAG_TEST_PIXEL_ORDER				0x00044006
	#define		ARM_MAILBOX_TAG_SET_PIXEL_ORDER					0x00048006
	#define		ARM_MAILBOX_TAG_ALPHA_MODE						0x00040007
	#define		ARM_MAILBOX_TAG_TEST_ALPHA_MODE					0x00044007
	#define		ARM_MAILBOX_TAG_SET_ALPHA_MODE					0x00048007
	#define		ARM_MAILBOX_TAG_PITCH							0x00040008
	#define		ARM_MAILBOX_TAG_VIRTUAL_OFFSET					0x00040009
	#define		ARM_MAILBOX_TAG_TEST_VIRTUAL_OFFSET				0x00044009
	#define		ARM_MAILBOX_TAG_SET_VIRTUAL_OFFSET				0x00048009
	#define		ARM_MAILBOX_TAG_OVERSCAN						0x0004000a
	#define		ARM_MAILBOX_TAG_TEST_OVERSCAN					0x0004400a
	#define		ARM_MAILBOX_TAG_SET_OVERSCAN					0x0004800a
	#define		ARM_MAILBOX_TAG_PALETTE							0x0004000b
	#define		ARM_MAILBOX_TAG_TEST_PALETTE					0x0004400b
	#define		ARM_MAILBOX_TAG_SET_PALETTE						0x0004800b
	#define		ARM_MAILBOX_TAG_SET_CURSOR_INFO					0x00008010
	#define		ARM_MAILBOX_TAG_SET_CURSOR_STATE				0x00008011



	unsigned int _kernel_mailbox_write( unsigned int channel, uintptr_t data );
	unsigned int _kernel_mailbox_read( unsigned int channel );



#endif /*MAILBOX_H*/
