#include "stdafx.h"


// Handle
typedef struct tagWAVEGENIF{
	HWND			hWnd;
	SoundManager	*pManager;
	SeqInputMML		*pInputMML;
	SoundOutputBase	*pOutputBase;
	std::wstring	waveFile;
	SoundEffectSet	*pPreviewSet;
	EffectGen		*pPrevireGen;
	SoundEffectADSR	*pPreviewADSR;
	SoundEffectVolume *pPreviewVolume;
	EffectGen::eTYPE previewGenType;
}WAVEGENIF;

SoundOutputDS* CreateOutputDS( HWND hWnd )
{
	SoundOutputDS *pOutput = new SoundOutputDS();
	pOutput->Create( hWnd, 0.05f );
	return pOutput;
}

SoundOutputWaveFile* CreateOutputWaveFile( const wchar_t *pFileName )
{
	SoundOutputWaveFile *pOutput = new SoundOutputWaveFile();
	pOutput->Create( pFileName );
	return pOutput;
}

WAVEGENDLL_API HANDLE WINAPI CreateWaveGen(HWND hWnd)
{
	// Create a handler
	WAVEGENIF *pHandler = new WAVEGENIF;
	pHandler->hWnd = hWnd;

	// Create a sound Manager
	pHandler->pManager = new SoundManager();

	// Create the input device
	pHandler->pInputMML = new SeqInputMML();
	pHandler->pInputMML->Init( pHandler->pManager );

	// Create output device
	pHandler->pOutputBase = CreateOutputDS( hWnd );

	// The manager I/O registration
	pHandler->pManager->ChangeInput( pHandler->pInputMML );
	pHandler->pManager->ChangeOutput( pHandler->pOutputBase );

	// Create Preview
	pHandler->previewGenType = EffectGen::SQUARE;
	pHandler->pPreviewSet = new SoundEffectSet();
	pHandler->pPrevireGen = new EffectGen();
	pHandler->pPreviewADSR = new SoundEffectADSR();
	pHandler->pPreviewVolume = new SoundEffectVolume();
	pHandler->pPreviewVolume->ChangeVolume( 0.2f );
	pHandler->pPreviewSet->Push( pHandler->pPrevireGen );
	pHandler->pPreviewSet->Push( pHandler->pPreviewADSR );
	pHandler->pPreviewSet->Push( pHandler->pPreviewVolume );
	pHandler->pManager->Push( pHandler->pPreviewSet );
	
	return pHandler;
}

WAVEGENDLL_API void WINAPI ReleaseWaveGen( HANDLE hInterface )
{
	if( hInterface ) {
		WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
		if( pThis->pManager )
			delete pThis->pManager;
		delete pThis;
	}
	hInterface = nullptr;
}

WAVEGENDLL_API void WINAPI SetWaveFileName( HANDLE hInterface, LPCWSTR fileName )
{
	if( hInterface == nullptr )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->waveFile = fileName;
}

WAVEGENDLL_API bool WINAPI CompileMML( HANDLE hInterface, LPCWSTR pMML, int *pErrorCode, DWORD *pErrorLine, bool isWave, void(*fncPlayFinished)(void*), int param )
{
	if( hInterface == nullptr )
		return true;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;

	// WAVE save file name is not specified?
	if( isWave==true && pThis->waveFile.length()==0 )
		return true;

	// To compile MML
	SeqInputMML::COMPILEDINFO compileInfo;
	compileInfo = pThis->pInputMML->CompileMML( pMML );

	if( pErrorCode )
		*pErrorCode = compileInfo.errorCode;
	if( pErrorLine )
		*pErrorLine = compileInfo.errorLine;

	if( compileInfo.errorCode != 0 )
		return true;

	// The switching of the output device
	if( isWave == true && dynamic_cast<SoundOutputWaveFile*>(pThis->pOutputBase) == nullptr ) {
		CriticalBlock cb( &pThis->pManager->m_cs );
		SoundOutputWaveFile *pOutput = CreateOutputWaveFile( pThis->waveFile.c_str() );
		pThis->pManager->ChangeOutput( pOutput );
		pThis->pOutputBase = pOutput;
		pOutput->Start();
	}
	if( isWave == false && dynamic_cast<SoundOutputDS*>(pThis->pOutputBase) == nullptr ) {
		CriticalBlock cb( &pThis->pManager->m_cs );
		pThis->pOutputBase = CreateOutputDS( pThis->hWnd );
		pThis->pManager->ChangeOutput( pThis->pOutputBase );
	}

	// Play
	pThis->pInputMML->Play( timeGetTime(), fncPlayFinished, (void*)param );

	return false;
}

WAVEGENDLL_API void WINAPI GetErrorString( int errorCode, LPTSTR pErrorMessage, int messageBufferMaxCount )
{
	const wchar_t *pMsg = SeqInputMML::GetErrorString( errorCode );
	wcscpy_s( pErrorMessage, messageBufferMaxCount, pMsg );
}

WAVEGENDLL_API void WINAPI Stop( HANDLE hInterface )
{
	if( hInterface == NULL )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->pInputMML->Stop();

	// The output to the DS
	if( dynamic_cast<SoundOutputDS*>(pThis->pOutputBase) == nullptr ) {
		pThis->pOutputBase = CreateOutputDS( pThis->hWnd );
		pThis->pManager->ChangeOutput( pThis->pOutputBase );
	}
}

WAVEGENDLL_API void WINAPI PreviewNoteOn( HANDLE hInterface, unsigned char note, unsigned char velocity )
{
	if( hInterface == NULL )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	if( pThis->previewGenType == EffectGen::FCNOISE_L 
	 || pThis->previewGenType == EffectGen::FCNOISE_S )
		pThis->pPrevireGen->ChangeFCNoiseFreq( note );
	else
		pThis->pPrevireGen->ChangeFreq( SeqInputMML::GetFreq( note ) );
	pThis->pPreviewVolume->ChangeVolume( velocity / 127.0f * 0.2f );
	pThis->pPreviewADSR->NoteOn();
}

WAVEGENDLL_API void WINAPI PreviewNoteOff( HANDLE hInterface )
{
	if( hInterface == NULL )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->pPreviewADSR->NoteOff();
}

WAVEGENDLL_API void WINAPI PreviewGenType( HANDLE hInterface, EffectGen::eTYPE type )
{
	if( hInterface == NULL )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->previewGenType = type;
	pThis->pPrevireGen->ChangeType( type );
}

WAVEGENDLL_API void WINAPI PreviewSetADSR( HANDLE hInterface, float aPower, float aTime, float dTime, float sPower, float rTime )
{
	if( hInterface == NULL )
		return;

	WAVEGENIF *pThis = (WAVEGENIF*)hInterface;
	pThis->pPreviewADSR->ChangeParam( aPower, aTime, dTime, sPower, rTime );
}

