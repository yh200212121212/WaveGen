#pragma once

#include "targetver.h"

#include <windows.h>
#include <WinBase.h>
#include <mmsystem.h>
#pragma comment ( lib, "winmm.lib" )
#include <dsound.h> 
#pragma comment ( lib, "dsound.lib" )

#pragma warning(disable:4480)

#include <vector>
#include <stack>
#include <string>
#include <cctype>
#include <algorithm>
#include <numeric>
#include <fstream>

#define SAFE_RELEASE(x)	{if(x){(x)->Release();(x)=nullptr;}}
#define SAFE_DELETE(x)	{if(x){delete(x);(x)=nullptr;}}

#define BASE_FREQ	44100

#include "CriticalBlock.h"
#include "SoundOutputBase.h"
#include "SoundOutputDS.h"
#include "SoundOutputWaveFile.h"
#include "SeqInputBase.h"
#include "SoundManager.h"
#include "SoundEffectSet.h"
#include "SoundEffectBase.h"
#include "EffectGen.h"
#include "SoundEffectADSR.h"
#include "SoundEffectVolume.h"
#include "SoundEffectVibrato.h"
#include "SeqInputMML.h"

#include "WinBase.h"

template<typename T> inline T MinMax( T v, T min, T max ) {
	if( v > max )	return max;
	if( v < min )	return min;
	return v;
}
template<typename T> inline T Min( T v, T min ) {
	if( v > min )	return min;
	return v;
}
template<typename T> inline T Max( T v, T max ) {
	if( v < max )	return max;
	return v;
}
