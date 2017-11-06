#include "stdafx.h"

SoundOutputWaveFile::SoundOutputWaveFile()
	: m_fp( nullptr )
	, m_writeTime( 0 )
{
}

SoundOutputWaveFile::~SoundOutputWaveFile()
{
	Release();
}

bool SoundOutputWaveFile::Create( const wchar_t *pFileName )
{
	m_fp = CreateFile( pFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS , FILE_ATTRIBUTE_NORMAL , nullptr );
	if( m_fp == INVALID_HANDLE_VALUE ) {
		m_fp = nullptr;
		return true;
	}

	DWORD dwSize;
	WriteFile( m_fp, "RIFF", 4, &dwSize, nullptr );

	DWORD cbSize = sizeof(PCMWAVEFORMAT) + 12;
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, nullptr );

	WriteFile( m_fp, "WAVEfmt ", 8, &dwSize, nullptr );
	cbSize = sizeof(PCMWAVEFORMAT);
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, nullptr );

	PCMWAVEFORMAT pcmwf;
	pcmwf.wf.wFormatTag		= WAVE_FORMAT_PCM;
	pcmwf.wf.nChannels		= 2;
	pcmwf.wf.nSamplesPerSec	= BASE_FREQ;
	pcmwf.wf.nBlockAlign	= 4;
	pcmwf.wf.nAvgBytesPerSec= pcmwf.wf.nSamplesPerSec * pcmwf.wf.nBlockAlign;
	pcmwf.wBitsPerSample	= 16;
	WriteFile( m_fp, &pcmwf, sizeof(pcmwf), &dwSize, nullptr );

	WriteFile( m_fp, "data", 4, &dwSize, nullptr );
	m_lengthDataPos = SetFilePointer( m_fp, 0, nullptr, FILE_CURRENT );
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, nullptr );

	cbSize = 0;

	return false;
}

void SoundOutputWaveFile::Release()
{
	if( m_fp == nullptr )
		return;

	DWORD dwSize;
	DWORD pos = SetFilePointer( m_fp, 0, nullptr, FILE_CURRENT );
	DWORD cbSize = pos - m_lengthDataPos - 4;
	SetFilePointer( m_fp, m_lengthDataPos, nullptr, FILE_BEGIN );
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, nullptr );

	cbSize = pos - 8;
	SetFilePointer( m_fp, 4, nullptr, FILE_BEGIN );
	WriteFile( m_fp, &cbSize, sizeof(cbSize), &dwSize, nullptr );

	CloseHandle( m_fp );
	m_fp = nullptr;
}
#ifdef _WIN64
DWORD SoundOutputWaveFile::GetBlockSize(DWORD time) const
#else
size_t SoundOutputWaveFile::GetBlockSize(DWORD time) const
#endif
{
	return (time - m_writeTime) * BASE_FREQ / 1000;
}

void SoundOutputWaveFile::Write( const void *pWaveData, size_t blockSize, DWORD time )
{
	if( m_fp == nullptr )
		return;
	if( m_writeTime == 0 )
		return;
	m_writeTime = time;

	DWORD writeSize, dwSize;
#ifdef _WIN64
#pragma warning(disable:4267)
	writeSize = blockSize * 4;
#else
	writeSize = blockSize * 4;
#endif
	WriteFile( m_fp, pWaveData, writeSize, &dwSize, nullptr );
}

void SoundOutputWaveFile::Start()
{
	m_writeTime = timeGetTime();
}

void SoundOutputWaveFile::Stop()
{
	m_writeTime = 0;
}

