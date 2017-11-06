#include "stdafx.h"

CriticalBlock::CriticalBlock( const LPCRITICAL_SECTION hCS )
{
	EnterCriticalSection( m_hCS = hCS );
}

CriticalBlock::~CriticalBlock()
{
	LeaveCriticalSection( m_hCS );
}
