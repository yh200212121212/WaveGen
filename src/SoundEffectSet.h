#pragma once

class SoundEffectSet
{
public:
	SoundEffectSet( float pan = 0.0f /* -1(L) 0 +1(R) */ );
	virtual ~SoundEffectSet();

	void GetWave( float *pLeft, float *pRight, size_t blockSize );
	void Push( SoundEffectBase *pSEB );
	void Release();

	typedef std::vector<SoundEffectBase*>	EFFECTLIST;
	typedef EFFECTLIST::iterator			EFFECTLISTITR;
	EFFECTLIST m_effecters;

private:
	float m_panpot;		// -1(L) 0(C) +1(R)
};
