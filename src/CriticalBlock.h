#pragma once

class CriticalBlock {
	public:
		CriticalBlock( const LPCRITICAL_SECTION hCS );
		~CriticalBlock();
	private:
		LPCRITICAL_SECTION	m_hCS;
};
