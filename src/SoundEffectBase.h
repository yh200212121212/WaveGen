#pragma once

class SoundEffectBase
{
public:
	SoundEffectBase();
	virtual ~SoundEffectBase();
	virtual void Release()=0;
	virtual void Reset()=0;
	virtual void Effect( float *pBuffer, size_t size )=0;

	const SoundManager *m_pSoundManager;

	static float CalcLiner( float s, float e, float t );
};
