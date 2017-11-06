#include "stdafx.h"

SoundEffectVolume::SoundEffectVolume()
	: m_volume( 1.0f )
{
	Reset();
}

SoundEffectVolume::~SoundEffectVolume()
{
}

void SoundEffectVolume::Release()
{
	Reset();
}

void SoundEffectVolume::Reset()
{
}

void SoundEffectVolume::ChangeVolume( float volume )
{
	m_volume = volume;
}

void SoundEffectVolume::Effect( float *pBuffer, size_t blockSize )
{
	for( ; blockSize; blockSize--, pBuffer++ ) {
		*pBuffer *= m_volume;
	}
}
