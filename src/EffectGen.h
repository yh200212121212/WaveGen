﻿#pragma once

// 単調波形生成
class EffectGen : public SoundEffectBase
{
public:
	enum eTYPE{
		 SQUARE		// 矩形波
		,TRIANGLE	// 三角波
		,SAW		// のこぎり波
		,SINEWAVE	// 正弦波
		,NOISE		// ノイズ
		,FCNOISE_S	// ファミコンノイズ(短)
		,FCNOISE_L	// ファミコンノイズ(長)
		,SILENT		// 無音
	};

	EffectGen();
	EffectGen( float freq, eTYPE type );
	virtual ~EffectGen();
	void Release();
	void Reset();
	void Effect( float *pBuffer, size_t size );

	void ChangeFreq( float freq );
	void ChangeFreqSweep( float fromFreq, float toFreq, float time );
	void ChangeType( eTYPE type );
	void ChangeSquareDuty( float duty );

private:
	float EffectSquare( bool isFirst );
	float EffectTriangle( bool isFirst );
	float EffectSaw( bool isFirst );
	float EffectSinewave( bool isFirst );
	float EffectNoise( bool isFirst );
	float EffectFcNoiseS( bool isFirst );
	float EffectFcNoiseL( bool isFirst );
	float EffectSilent( bool isFirst );

	float(EffectGen::*m_fncEffect)( bool isFirst );
	eTYPE	m_type;
	float	m_tph;
	float	m_blockCount;
	float	m_noise;
	float	m_squareDuty;
	float	m_freq;
	float	m_sweepFreq;
	unsigned short m_fcr;
};
