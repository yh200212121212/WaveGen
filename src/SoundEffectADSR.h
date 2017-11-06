#pragma once
class SoundEffectADSR : public SoundEffectBase
{
public:
	SoundEffectADSR();
	virtual ~SoundEffectADSR();
	void Release();
	void Reset();
	void Effect( float *pBuffer, size_t size );

	void ChangeParam( float aPower, float aTime, float dTime, float sPower, float rTime );
	void NoteOn();
	void NoteOff();

private:
	float EffectAttack();
	float EffectDecay();
	float EffectSustain();
	float EffectRelease();
	float EffectMute();

	float(SoundEffectADSR::*m_fncEffect)();

	DWORD	m_startTime;
	float	m_currentPower;
	float	m_aPower, m_sPower;
	DWORD	m_aTime, m_dTime, m_rTime;
};
