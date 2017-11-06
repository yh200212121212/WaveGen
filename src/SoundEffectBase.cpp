#include "stdafx.h"

SoundEffectBase::SoundEffectBase()
{
}
SoundEffectBase::~SoundEffectBase()
{
}
float SoundEffectBase::CalcLiner( float s, float e, float t )
{
	if( s >= e || e <= t )
		return 1.0f;
	if( s >= t )
		return 0.0f;
	return (t-s)/(e-s);
}
