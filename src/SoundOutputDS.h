#pragma once

class SoundOutputDS : public SoundOutputBase
{
public:
	SoundOutputDS();
	virtual ~SoundOutputDS();

	bool Create( HWND hWnd, float bufferSec );
	void Release();
	void Mute();
#ifdef _WIN64
	virtual DWORD GetBlockSize(DWORD) const;
#else
	virtual size_t GetBlockSize(DWORD) const;
#endif
	virtual void Write( const void *pWaveData, size_t blockSize, DWORD time );

private:
	DWORD GetBufferByte() const;

	float m_bufferSec;
	LPDIRECTSOUND8	m_pDSDev;
	LPDIRECTSOUNDBUFFER	m_pDSB;
	WAVEFORMATEX m_wfx;

	DWORD m_writePos;
};
