#pragma once

class SoundOutputWaveFile : public SoundOutputBase
{
public:
	SoundOutputWaveFile();
	virtual ~SoundOutputWaveFile();

	bool Create( const wchar_t *pFileName );
	void Release();
	void Start();
	void Stop();
#ifdef _WIN64
	virtual DWORD GetBlockSize(DWORD time) const;
#else
	virtual size_t GetBlockSize( DWORD time ) const;
#endif
	virtual void Write( const void *pWaveData, size_t blockSize, DWORD time );

private:
	HANDLE	m_fp;
	DWORD	m_writeTime;
	DWORD	m_lengthDataPos;
};
