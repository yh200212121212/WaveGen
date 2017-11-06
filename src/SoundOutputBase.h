#pragma once

class SoundOutputBase
{
public:
	SoundOutputBase();
	virtual ~SoundOutputBase();

	virtual void Release()=0;
#ifdef _WIN64
	virtual DWORD GetBlockSize(DWORD time) const = 0;
#else
	virtual size_t GetBlockSize( DWORD time ) const=0;
#endif
	virtual void Write( const void *pWaveData, size_t blockSize, DWORD time )=0; 
};
