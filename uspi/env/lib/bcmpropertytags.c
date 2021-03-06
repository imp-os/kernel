//
// bcmpropertytags.c
//
// USPi - An USB driver for Raspberry Pi written in C
// Copyright (C) 2014-2017  R. Stange <rsta2@o2online.de>
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
#include "../../../uspi/env/include/uspienv/bcmpropertytags.h"

#include "../../../uspi/env/include/uspienv/bcm2835.h"
#include "../../../uspi/env/include/uspienv/synchronize.h"
#include "../../../uspi/env/include/uspienv/sysconfig.h"
#include "../../../uspi/env/include/uspienv/util.h"

#include "../../../kernel/c/common/include/mailbox.h"
#include "../../../kernel/c/common/include/stdlib.h"

typedef struct TPropertyBuffer
{
	u32	nBufferSize;			// bytes
	u32	nCode;
	#define CODE_REQUEST		0x00000000
	#define CODE_RESPONSE_SUCCESS	0x80000000
	#define CODE_RESPONSE_FAILURE	0x80000001
	u8	Tags[0];
	// end tag follows
}
TPropertyBuffer;

void BcmPropertyTags (TBcmPropertyTags *pThis)
{
	// BcmMailBox (&pThis->m_MailBox, BCM_MAILBOX_PROP_OUT);
	pThis->m_MailBox.m_nChannel = BCM_MAILBOX_PROP_OUT;
}

void _BcmPropertyTags (TBcmPropertyTags *pThis)
{
	// _BcmMailBox (&pThis->m_MailBox);
}

boolean BcmPropertyTagsGetTag (TBcmPropertyTags *pThis, u32 nTagId,
			       void *pTag, unsigned nTagSize, unsigned  nRequestParmSize)
{
	unsigned nBufferSize = sizeof (TPropertyBuffer) + nTagSize + sizeof (u32);

#if RASPPI != 3
	// cannot use malloc() here because this is used before mem_init() is called
	u8 Buffer[nBufferSize + 15];
	TPropertyBuffer *pBuffer = (TPropertyBuffer *) (((u32) Buffer + 15) & ~15);
#else
	TPropertyBuffer *pBuffer = (TPropertyBuffer *) MEM_COHERENT_REGION;
#endif
	
	pBuffer->nBufferSize = nBufferSize;
	pBuffer->nCode = CODE_REQUEST;
	_kernel_memcpy( pBuffer->Tags, pTag, nTagSize );
	
	TPropertyTag *pHeader = (TPropertyTag *) pBuffer->Tags;
	pHeader->nTagId = nTagId;
	pHeader->nValueBufSize = nTagSize - sizeof (TPropertyTag);
	pHeader->nValueLength = nRequestParmSize & ~VALUE_LENGTH_RESPONSE;

	u32 *pEndTag = (u32 *) (pBuffer->Tags + nTagSize);
	*pEndTag = PROPTAG_END;

#if RASPPI != 3
	CleanDataCache ();
	DataSyncBarrier ();
#endif

	uintptr_t nBufferAddress = BUS_ADDRESS ((uintptr_t) pBuffer);
	/*
	if (BcmMailBoxWriteRead (&pThis->m_MailBox, nBufferAddress) != nBufferAddress)
	{
		return FALSE;
	}
	*/
	// ACU - use kernel mailbox code here instead
	_kernel_mailbox_write( pThis->m_MailBox.m_nChannel, nBufferAddress );
	_kernel_mailbox_read(pThis->m_MailBox.m_nChannel );


	
#if RASPPI != 3
	InvalidateDataCache ();
	DataSyncBarrier ();
#else
	DataMemBarrier ();
#endif

	if (pBuffer->nCode != CODE_RESPONSE_SUCCESS)
	{
		return FALSE;
	}
	
	if (!(pHeader->nValueLength & VALUE_LENGTH_RESPONSE))
	{
		return FALSE;
	}
	
	pHeader->nValueLength &= ~VALUE_LENGTH_RESPONSE;
	if (pHeader->nValueLength == 0)
	{
		return FALSE;
	}

	_kernel_memcpy( pTag, pBuffer->Tags, nTagSize );

	return TRUE;
}
