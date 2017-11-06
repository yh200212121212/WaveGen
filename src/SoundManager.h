#pragma once

class SoundEffectBase;
class SoundEffectSet;

class SoundManager
{
public:
	SoundManager();
	virtual ~SoundManager();
	void Release();

	DWORD Push( SoundEffectSet *pSES );
	DWORD Push( SoundEffectSet *pSES, DWORD updateHandle );
	void ChangeOutput( SoundOutputBase *pBase );
	void ChangeInput( SeqInputBase *pBase );
	bool Remove( DWORD handle );

private:
	void Store( void *pBuf, size_t byteSize );
	bool CreateManagerThread();
	static DWORD WINAPI ManagerThreadBase( LPVOID pParam );
	DWORD ManagerThread();
	void ReleaseManagerThread();

public:
	CRITICAL_SECTION m_cs;

private:
	typedef struct tagEFFECTINFO{
		DWORD			handle;
		SoundEffectSet *pBase;
	}EFFECTINFO;

	SoundOutputBase *m_pOutputBase;
	SeqInputBase	*m_pSeqInputBase;

	typedef std::vector<EFFECTINFO>		EFFECTSETLIST;
	typedef EFFECTSETLIST::iterator		EFFECTSETLISTITR;
	EFFECTSETLIST m_effectSet;
	DWORD m_effectHandle;


	HANDLE			m_threadHandle;
	volatile bool	m_threadIsExit;
};

