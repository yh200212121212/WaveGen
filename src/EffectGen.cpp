#include "stdafx.h"

#define	FC_NOISE_SFT	10
#define FC_NOISE_SEC	(1789773 << FC_NOISE_SFT)
#define FC_NOISE_DLT	(FC_NOISE_SEC/BASE_FREQ)

EffectGen::EffectGen()
{
	Reset();
	ChangeFreq( 440 );
	ChangeType( SQUARE );
	ChangeSquareDuty( 0.5f );
}

EffectGen::EffectGen( float freq, eTYPE type )
{
	Reset();
	ChangeFreq( freq );
	ChangeType( type );
	ChangeSquareDuty( 0.5f );
}

EffectGen::~EffectGen()
{
}
void EffectGen::ChangeFreq( float freq )
{
	if( freq <= 0 )
		freq = FLT_MIN;
	m_freq = freq;
	m_tph = ((float)BASE_FREQ) / freq;
	while( m_blockCount >= m_tph )
		m_blockCount -= m_tph;
	m_sweepFreq = 0;
}

void EffectGen::ChangeFreqSweep( float fromFreq, float toFreq, float time )
{
	if( time <= 0 ) {
		ChangeFreq( toFreq );
		m_sweepFreq = 0;
	}else{
		ChangeFreq( fromFreq );
		m_sweepFreq = (toFreq - fromFreq) / (BASE_FREQ * time);
	}
}

void EffectGen::ChangeType( eTYPE type )
{
	m_type = type;
	switch( type )
	{
		case SQUARE:
			m_fncEffect = &EffectGen::EffectSquare;
			break;
		case TRIANGLE:
			m_fncEffect = &EffectGen::EffectTriangle;
			break;
		case SAW:
			m_fncEffect = &EffectGen::EffectSaw;
			break;
		case SINEWAVE:
			m_fncEffect = &EffectGen::EffectSinewave;
			break;
		case FCNOISE_L:
			m_fncEffect = &EffectGen::EffectFcNoise;
			m_fcls = 1;
			break;
		case FCNOISE_S:
			m_fncEffect = &EffectGen::EffectFcNoise;
			m_fcls = 6;
			break;
		case FCTRIANGLE:
			m_fncEffect = &EffectGen::EffectFcTriangle;
			break;
		case SILENT:
		default:
			m_fncEffect = &EffectGen::EffectSilent;
			break;
	}
}

// FCノイズ用周波数切り替え
void EffectGen::ChangeFCNoiseFreq( BYTE note )
{
	static const int tbl[16]={
		 0x004, 0x008, 0x010, 0x020, 0x040, 0x060, 0x080, 0x0a0
		,0x0ca, 0x0fe, 0x17c, 0x1fc, 0x2fa, 0x3f8, 0x7f2, 0xfe4
	};
	m_fcnoiseWaitCount = m_fcnoiseWait = tbl[ 0x0f-(note&0x0f) ] << FC_NOISE_SFT;
}


void EffectGen::ChangeSquareDuty( float duty )
{
	m_squareDuty = MinMax( duty, FLT_MIN, 1.0f-FLT_EPSILON );
}

EffectGen::eTYPE EffectGen::GetType()
{
	return m_type;
}

void EffectGen::Release()
{
}
void EffectGen::Reset()
{
	m_sweepFreq = 0;
	m_blockCount = 0;
	m_fcr = 0x8000;
	m_fcls = 6;
	m_noise = 0;
	ChangeFCNoiseFreq( 0x08 );
}

void EffectGen::Effect( float *pBuffer, size_t bloackSize )
{
	for( ; bloackSize; bloackSize--, pBuffer++ ) {
		bool isFirst = false;
		m_blockCount++;
		while( m_tph <= m_blockCount ) {
			m_blockCount -= m_tph;
			isFirst = true;
		}
		*pBuffer = (this->*m_fncEffect)( isFirst );
		if( m_sweepFreq ) {
			m_freq += m_sweepFreq;
			m_tph = BASE_FREQ / m_freq;
			if( m_tph < 0 )
				m_tph = -m_tph;
		}
	}
}

// 矩形波
float EffectGen::EffectSquare( bool )
{
	return ( ( m_tph * m_squareDuty < m_blockCount ) ? 1.0f : -1.0f );
}

// 三角波
float EffectGen::EffectTriangle( bool )
{
	float v = CalcLiner( 0, m_tph, m_blockCount ) * 4.0f;

	if( v >= 2.0f )	return 3.0f-v;
	else			return v-1.0f;
}

// ファミコン三角波
float EffectGen::EffectFcTriangle( bool )
{
	float v = CalcLiner( 0, m_tph, m_blockCount ) * 4.0f;

	if( v >= 2.0f )	v = 3.0f-v;
	else			v = v-1.0f;

	return floor(v*8) / 8;
}

// のこぎり波
float EffectGen::EffectSaw( bool )
{
	return 1.0f - CalcLiner( 0, m_tph, m_blockCount ) * 2.0f;
}

// 正弦波
float EffectGen::EffectSinewave( bool )
{
	return sin( CalcLiner( 0, m_tph, m_blockCount ) * 3.141592f * 2.0f );
}

int EffectGen::GetFcRandomTick()
{
    m_fcr >>= 1;
	m_fcr |= ((m_fcr^(m_fcr>>m_fcls))&1)<<15;
	return (m_fcr&1)?1:-1;
}

// FCノイズ
float EffectGen::EffectFcNoise( bool )
{
	int total = 0;
	int count = 0;
	int delta = FC_NOISE_DLT;
    while( delta >= m_fcnoiseWaitCount ) {
        delta -= m_fcnoiseWaitCount;
        m_fcnoiseWait = 0;
		total += GetFcRandomTick();
        count++;
    }
    if (count > 0)
        m_noise = total/(float)count;
    m_fcnoiseWait += delta;
    if (m_fcnoiseWait >= m_fcnoiseWaitCount) {
        m_fcnoiseWait -= m_fcnoiseWaitCount;
		m_noise = (float)GetFcRandomTick();
    }

    return m_noise;
}

// 無音
float EffectGen::EffectSilent( bool )
{
	return 0;
}
