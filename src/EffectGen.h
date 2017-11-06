#pragma once

// Waveform generating monotone
class EffectGen : public SoundEffectBase
{
public:
	enum eTYPE : unsigned char {
		 SQUARE		// Rectangular wave
		,TRIANGLE	// Triangular wave
		,SAW		// Sawtooth wave
		,SINEWAVE	// Sine wave
		,FCNOISE_L	// Video noise(long)
		,FCNOISE_S	// フVideo noise(short)
		,FCTRIANGLE // Triangle NES(16 stages)
		,SILENT		// Silent
	};

	EffectGen();
	EffectGen( float freq, eTYPE type );
	virtual ~EffectGen();
	void Release();
	void Reset();
	void Effect( float *pBuffer, size_t size );

	void ChangeFreq( float freq );
	void ChangeFCNoiseFreq( BYTE note );
	void ChangeFreqSweep( float fromFreq, float toFreq, float time );
	void ChangeType( eTYPE type );
	void ChangeSquareDuty( float duty );

	eTYPE GetType();

private:
	float EffectSquare( bool isFirst );
	float EffectTriangle( bool isFirst );
	float EffectFcTriangle( bool isFirst );
	float EffectSaw( bool isFirst );
	float EffectSinewave( bool isFirst );
	float EffectFcNoise( bool isFirst );
	float EffectSilent( bool isFirst );

	int GetFcRandomTick();

	float(EffectGen::*m_fncEffect)( bool isFirst );
	eTYPE	m_type;
	float	m_tph;
	float	m_blockCount;
	float	m_noise;
	float	m_squareDuty;
	float	m_freq;
	float	m_sweepFreq;

	// FC noise
	WORD	m_fcr;
	char	m_fcls;
	int		m_fcnoiseWait;
	int		m_fcnoiseWaitCount;
};
