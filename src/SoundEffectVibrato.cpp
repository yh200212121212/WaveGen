#include "stdafx.h"

SoundEffectVibrato::SoundEffectVibrato()
{
	m_setting.hz = m_setting.aTime = m_setting.dTime = m_setting.sTime = m_setting.rTime = m_setting.delayTime = m_setting.aPower = m_setting.sPower = m_setting.baseFreq = 0;
	Reset();
	m_effectFreqGen.ChangeType( EffectGen::TRIANGLE );	// Ideal is SINEWAVE
}

SoundEffectVibrato::~SoundEffectVibrato()
{
	SAFE_DELETE( m_pEffectGen );
}

void SoundEffectVibrato::Release()
{
	Reset();
}

void SoundEffectVibrato::SetEffectGen( EffectGen *pEffectGen )
{
	m_pEffectGen = pEffectGen;
}

void SoundEffectVibrato::Reset()
{
	m_tickCount = 0;
	m_delayTick = (DWORD)(BASE_FREQ * m_setting.delayTime);
	m_sustainTick = (DWORD)( BASE_FREQ * ( m_setting.delayTime + m_setting.aTime + m_setting.dTime + m_setting.sTime ) );
	m_releaseTick = (DWORD)( BASE_FREQ * ( m_setting.delayTime + m_setting.aTime + m_setting.dTime + m_setting.sTime + m_setting.rTime ) );
	m_effectFreqGen.ChangeFreq( m_setting.hz );
}

void SoundEffectVibrato::ChangeParam( float baseFreq, float delayTime, float sPower, float hz, float aPower, float aTime, float dTime, float sTime, float rTime )
{
	m_setting.baseFreq = baseFreq;
	m_setting.delayTime = delayTime;
	m_setting.aPower = (aPower==0.f) ? 0.f : (baseFreq-(baseFreq/pow(2.f,1.f/(12.f/aPower))))/ 2.f;
	m_setting.aTime = aTime;
	m_setting.dTime = dTime;
	m_setting.sTime = sTime;
	m_setting.sPower = (sPower==0.f) ? 0.f : (baseFreq-(baseFreq/pow(2.f,1.f/(12.f/sPower))))/ 2.f;
	m_setting.rTime = rTime;
	m_setting.hz = hz;
	if( m_pEffectGen )
		m_pEffectGen->ChangeFreq( baseFreq );

	Reset();
}

void SoundEffectVibrato::Effect( float *pBuffer, size_t blockSize )
{
	if( m_pEffectGen == nullptr )
		return;

	for( ; blockSize; blockSize--, pBuffer++ ) {
		if( m_setting.hz > 0 ) {
			bool isAdsr = false;
			if( m_tickCount++ <= m_delayTick )  {
				if( m_tickCount >= m_delayTick ) {
					m_effectAdsr.ChangeParam( m_setting.aPower, m_setting.aTime, m_setting.dTime, m_setting.sPower, m_setting.rTime );
					m_effectAdsr.NoteOn();
					isAdsr = true;
				}
			}else if( m_tickCount <= m_sustainTick )  {
				if( m_tickCount == m_sustainTick )
					m_effectAdsr.NoteOff();
				isAdsr = true;
			}else if( m_tickCount <= m_releaseTick )  {
				isAdsr = true;
			}

			if( isAdsr ) {
				float freqMod = 1.0f;
				m_effectFreqGen.Effect( &freqMod, 1 );
				m_effectAdsr.Effect( &freqMod, 1 );
				m_pEffectGen->ChangeFreq( m_setting.baseFreq + freqMod );
			}
		}
		m_pEffectGen->Effect( pBuffer, 1 );
	}
}
