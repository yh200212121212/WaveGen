#pragma once

// Vibrato effect
class SoundEffectVibrato : public SoundEffectBase
{
public:
	SoundEffectVibrato();
	virtual ~SoundEffectVibrato();
	void Release();
	void Reset();
	void Effect( float *pBuffer, size_t size );

	void SetEffectGen( EffectGen *pEffectGen );

	void ChangeParam( float baseFreq, float delayTime, float sPower, float hz, float aPower, float aTime, float dTime, float sTime, float rTime );

private:
	typedef struct tagPARAM {
		float	baseFreq, delayTime, sPower, aPower, aTime, dTime, sTime, rTime, hz;
	}PARAM;

	PARAM			m_setting;
	DWORD			m_tickCount;
	DWORD			m_delayTick;
	DWORD			m_sustainTick;
	DWORD			m_releaseTick;
	EffectGen		m_effectFreqGen;
	EffectGen*		m_pEffectGen;
	SoundEffectADSR	m_effectAdsr;
};
