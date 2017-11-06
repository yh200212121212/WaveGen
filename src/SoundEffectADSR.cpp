#include "stdafx.h"

SoundEffectADSR::SoundEffectADSR()
	: m_fncEffect( &SoundEffectADSR::EffectMute )
{
	Reset();
}

SoundEffectADSR::~SoundEffectADSR()
{
}

void SoundEffectADSR::ChangeParam( float aPower, float aTime, float dTime, float sPower, float rTime )
{
	m_aPower = Max( aPower, 0.0f );
	m_sPower = Max( sPower, 0.0f );
	m_aTime = Max( (DWORD)(BASE_FREQ * aTime), 0ul );
	m_dTime = Max( (DWORD)(BASE_FREQ * dTime), 0ul );
	m_rTime = Max( (DWORD)(BASE_FREQ * rTime), 0ul );
}

void SoundEffectADSR::Release()
{
	Reset();
}

void SoundEffectADSR::Reset()
{
	// Default setting same as without effect
	ChangeParam( 0, 0, 0, 1, 0 );
	m_fncEffect = &SoundEffectADSR::EffectMute;
}

void SoundEffectADSR::NoteOn()
{
	m_startTime = 0;
	m_currentPower = 0;
	m_fncEffect = &SoundEffectADSR::EffectAttack;
}

void SoundEffectADSR::NoteOff()
{
	m_startTime = 0;
	// Release it if it has not stopped or decayed already
	if( m_fncEffect != &SoundEffectADSR::EffectMute && m_fncEffect != &SoundEffectADSR::EffectRelease )
		m_fncEffect = &SoundEffectADSR::EffectRelease;
}

void SoundEffectADSR::Effect( float *pBuffer, size_t bloackSize )
{
	for( ; bloackSize; bloackSize--, pBuffer++ ) {
		*pBuffer *= (this->*m_fncEffect)();
		m_startTime++;
	}
}

float SoundEffectADSR::EffectAttack()
{
	if( m_startTime >= m_aTime ) {
		m_startTime -= m_aTime;
		m_fncEffect = &SoundEffectADSR::EffectDecay;
		m_currentPower = m_aPower;
		return EffectDecay();
	}

	return CalcLiner( 0, (float)m_aTime, (float)m_startTime ) * m_aPower;
}

float SoundEffectADSR::EffectDecay()
{
	if( m_startTime >= m_dTime ) {
		m_fncEffect = &SoundEffectADSR::EffectSustain;
		m_currentPower = m_sPower;
		return EffectSustain();
	}

	return m_aPower - CalcLiner( 0, (float)m_aTime, (float)m_startTime ) * (m_aPower-m_sPower);
}

float SoundEffectADSR::EffectSustain()
{
	return m_sPower;
}

float SoundEffectADSR::EffectRelease()
{
	if( m_startTime >= m_rTime ) {
		m_startTime -= m_rTime;
		m_fncEffect = &SoundEffectADSR::EffectMute;
		m_currentPower = 0;
		return EffectMute();
	}

	return (1.0f-CalcLiner( 0, (float)m_rTime, (float)m_startTime )) * m_currentPower;
}

float SoundEffectADSR::EffectMute()
{
	return 0;
}

