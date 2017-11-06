#include "stdafx.h"

SoundOutputDS::SoundOutputDS()
	: m_pDSDev( NULL )
	, m_pDSB( NULL )
	, m_writePos( 0 )
	, m_bufferSec( 0 )
{
}

SoundOutputDS::~SoundOutputDS()
{
	Release();
}
void SoundOutputDS::Release()
{
	SAFE_RELEASE( m_pDSB );
	SAFE_RELEASE( m_pDSDev );
}

bool SoundOutputDS::Create( HWND hWnd, float bufferSec )
{
	HRESULT ret;
	ret = DirectSoundCreate8( NULL, &m_pDSDev, NULL );
	if( FAILED(ret) )
		return true;
	m_pDSDev->SetCooperativeLevel( hWnd, DSSCL_NORMAL );

 	// Create a sound buffer
	m_bufferSec = bufferSec;
	ZeroMemory(&m_wfx, sizeof(m_wfx));
	m_wfx.wFormatTag = WAVE_FORMAT_PCM;
	m_wfx.nChannels = 2;
	m_wfx.nSamplesPerSec = BASE_FREQ;
	m_wfx.wBitsPerSample = 16;
	m_wfx.nBlockAlign = (WORD)(m_wfx.wBitsPerSample * m_wfx.nChannels / 8);
	m_wfx.nAvgBytesPerSec = (DWORD)(m_wfx.nSamplesPerSec * m_wfx.nBlockAlign);

	DSBUFFERDESC dsdesc;
	ZeroMemory( &dsdesc, sizeof(dsdesc) );
	dsdesc.dwSize = sizeof(dsdesc);
	dsdesc.dwFlags = DSBCAPS_GETCURRENTPOSITION2 | DSBCAPS_STATIC | DSBCAPS_LOCDEFER | DSBCAPS_GLOBALFOCUS;
	dsdesc.dwBufferBytes = GetBufferByte();
	dsdesc.lpwfxFormat = &m_wfx;
	dsdesc.guid3DAlgorithm = DS3DALG_DEFAULT;
	ret = m_pDSDev->CreateSoundBuffer( &dsdesc, &m_pDSB, nullptr );
	if( FAILED(ret) )
		return true;

	// Buffer clear
	Mute();

	// Play
	m_pDSB->Play( 0, 0, DSBPLAY_LOOPING );
	return false;
}

DWORD SoundOutputDS::GetBufferByte() const
{
	return (DWORD)(m_wfx.nSamplesPerSec * m_wfx.nBlockAlign * m_bufferSec);
}
#ifdef _WIN64
DWORD SoundOutputDS::GetBlockSize(DWORD) const
#else
size_t SoundOutputDS::GetBlockSize( DWORD ) const
#endif
{
	DWORD writeSize, playCursor;
	m_pDSB->GetCurrentPosition( &playCursor, nullptr );
	if( playCursor < m_writePos )	writeSize = GetBufferByte() - m_writePos + playCursor;
	else							writeSize = playCursor - m_writePos;
	return writeSize / 4;
}

void SoundOutputDS::Write( const void *pWaveData, size_t blockSize, DWORD time )
{
	if( m_pDSB == nullptr )
		return;

	LPVOID lpvWrite1, lpvWrite2;
	DWORD dwLength1, dwLength2;
	DWORD writeSize = GetBlockSize( time );
	if( writeSize > blockSize )
#ifdef _WIN64
#pragma warning(disable:4267)
		writeSize = blockSize;
#else
		writeSize = blockSize;
#endif
	if( writeSize == 0 )
		return;

	if( DS_OK == m_pDSB->Lock( m_writePos, writeSize*4, &lpvWrite1, &dwLength1, &lpvWrite2, &dwLength2, 0 ) ) {
		memcpy( lpvWrite1, pWaveData, dwLength1 );
		memcpy( lpvWrite2, ((char*)pWaveData) + dwLength1, dwLength2 );
		m_pDSB->Unlock( lpvWrite1, dwLength1, lpvWrite2, dwLength2 );
		m_writePos += dwLength1 + dwLength2;
		const DWORD bufSize = GetBufferByte();
		if( m_writePos >= bufSize )
			m_writePos -= bufSize;
	}else{
		dwLength1 = dwLength2 = 0;
	}
}

void SoundOutputDS::Mute()
{
	if( m_pDSB == nullptr )
		return;

	LPVOID lpvWrite = 0;
	DWORD dwLength = 0;
	if( DS_OK == m_pDSB->Lock( 0, 0, &lpvWrite, &dwLength, nullptr, nullptr, DSBLOCK_ENTIREBUFFER ) ) {
		ZeroMemory( lpvWrite, dwLength );
		m_pDSB->Unlock( lpvWrite, dwLength, nullptr, 0);
	}
}
