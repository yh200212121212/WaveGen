#include "stdafx.h"

SoundManager::SoundManager()
	: m_pOutputBase( nullptr )
	, m_pSeqInputBase( nullptr )
	, m_threadHandle( nullptr )
	, m_threadIsExit( true )
{
	InitializeCriticalSection( &m_cs );

	// Thread creation
	CreateManagerThread();
}

SoundManager::~SoundManager()
{
	Release();

	ReleaseManagerThread();
	DeleteCriticalSection( &m_cs );
}

void SoundManager::ChangeOutput( SoundOutputBase *pBase )
{
	CriticalBlock mb( &m_cs );

	SAFE_DELETE( m_pOutputBase );
	m_pOutputBase = pBase;
}

void SoundManager::ChangeInput( SeqInputBase *pBase )
{
	CriticalBlock mb( &m_cs );

	SAFE_DELETE( m_pSeqInputBase );
	m_pSeqInputBase = pBase;
}

DWORD WINAPI SoundManager::ManagerThreadBase( LPVOID pParam )
{
	SoundManager *pThis = (SoundManager*)pParam;
	return pThis->ManagerThread();
}

DWORD SoundManager::ManagerThread()
{
	while( !m_threadIsExit ) {
		{
			CriticalBlock mb( &m_cs );

			DWORD time = timeGetTime();
			// sequence
			if( m_pSeqInputBase ) {
				m_pSeqInputBase->Tick( time );
			}

			// in/output
			if( m_pOutputBase ) {
				size_t blockSize = m_pOutputBase->GetBlockSize( time );
				void *pWaveData = new char[ blockSize*4 ];
				Store( pWaveData, blockSize*4 );
				m_pOutputBase->Write( pWaveData, blockSize, time );
				delete pWaveData;
			}
		}
		Sleep( 1 );
	}

	return 0;
}

bool SoundManager::CreateManagerThread()
{
	if( m_threadHandle != nullptr )
		return true;

	m_threadIsExit = false;
	m_threadHandle = CreateThread( NULL, 0, ManagerThreadBase, this, 0, NULL );
	if( m_threadHandle == nullptr )
		return false;
	return true;
}

void SoundManager::ReleaseManagerThread()
{
	m_threadIsExit = true;
	if( m_threadHandle ) {
		DWORD dwExitCode;
		for(;;) {
			if( GetExitCodeThread( m_threadHandle, &dwExitCode )==0 || dwExitCode == STILL_ACTIVE ) {
				Sleep( 100 );
				continue;
			}
			break;
		}
	}
}

DWORD SoundManager::Push( SoundEffectSet *pSES )
{
	EFFECTINFO effectInfo;
	effectInfo.handle = m_effectHandle++;
	effectInfo.pBase = pSES;

	CriticalBlock mb( &m_cs );
	m_effectSet.push_back( effectInfo );
	return effectInfo.handle;
}

DWORD SoundManager::Push( SoundEffectSet *pSES, DWORD updateHandle )
{
	Remove( updateHandle );
	return Push( pSES );
}

bool SoundManager::Remove( DWORD handle )
{
	CriticalBlock mb( &m_cs );

	for( EFFECTSETLISTITR itr=m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		if( itr->handle == handle ) {
			m_effectSet.erase( itr );
			return false;
		}
	}
	return true;
}

void SoundManager::Store( void *pBuf, size_t byteSize )
{
	size_t blockSize = byteSize/4;
	if( blockSize == 0 )
		return;


	float *l = new float[blockSize];	// Work buffer (left channel)
	float *r = new float[blockSize];	// Work buffer (right channel)
	float *mixl = new float[blockSize];	// Mixing buffer (left channel)
	float *mixr = new float[blockSize];	// Mixing buffer (right channel)
	ZeroMemory( mixl, blockSize*sizeof(float) );
	ZeroMemory( mixr, blockSize*sizeof(float) );

	// Effect set loop
	for( EFFECTSETLISTITR itr=m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		ZeroMemory( l, blockSize*sizeof(float) );
		ZeroMemory( r, blockSize*sizeof(float) );

		// Acquire waveform data from effect set
		itr->pBase->GetWave( l, r, blockSize );

		// mixing
		float *prl = l, *prr = r;
		float *pwl = mixl, *pwr = mixr;
		for( size_t i=0; i<blockSize; i++ ) {
			*(pwl++) += *(prl++);
			*(pwr++) += *(prr++);
		}
	}
	delete l;
	delete r;

	// Convert the mixed data from float to short
	float v;
	short *w = (short*)pBuf;
	const float *rl = mixl;
	const float *rr = mixr;
	for( ; blockSize; blockSize-- ) {
		v = MinMax( *rl++, -1.0f, 1.0f );
		*w++ = (short)( v * 32767.0f );

		v = MinMax( *rr++, -1.0f, 1.0f );
		*w++ = (short)( v * 32767.0f );
	}
	delete mixl;
	delete mixr;
}

void SoundManager::Release()
{
	CriticalBlock mb( &m_cs );

	SAFE_DELETE( m_pOutputBase );
	SAFE_DELETE( m_pSeqInputBase );
	for( EFFECTSETLISTITR itr = m_effectSet.begin(); itr!=m_effectSet.end(); ++itr ) {
		itr->pBase->Release();
		delete itr->pBase;
	}
	m_effectSet.clear();
}
