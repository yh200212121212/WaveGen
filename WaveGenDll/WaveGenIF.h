#pragma once

#ifdef WAVEGENDLL_EXPORTS
 #define WAVEGENDLL_API extern "C" __declspec(dllexport) 
#else
 #define WAVEGENDLL_API extern "C" __declspec(dllimport)
#endif

WAVEGENDLL_API HANDLE WINAPI CreateWaveGen( HWND hWnd );
WAVEGENDLL_API void WINAPI ReleaseWaveGen( HANDLE hInterface );
WAVEGENDLL_API void WINAPI SetWaveFileName( HANDLE hInterface, LPCWSTR fileName );
WAVEGENDLL_API bool WINAPI CompileMML( HANDLE hInterface, LPCWSTR pBuffer, int *pErrorCode, DWORD *pErrorLine, bool isWave, void (*fncPlayFinished)(void*), int param );
WAVEGENDLL_API void WINAPI GetErrorString( int errorCode, LPTSTR pErrorMessage, int messageBufferMaxCount );
WAVEGENDLL_API void WINAPI Stop( HANDLE hInterface );
WAVEGENDLL_API void WINAPI PreviewNoteOn( HANDLE hInterface, unsigned char note, unsigned char velocity );
WAVEGENDLL_API void WINAPI PreviewNoteOff( HANDLE hInterface );
WAVEGENDLL_API void WINAPI PreviewGenType( HANDLE hInterface, EffectGen::eTYPE type );
WAVEGENDLL_API void WINAPI PreviewSetADSR( HANDLE hInterface, float aPower, float aTime, float dTime, float sPower, float rTime );

