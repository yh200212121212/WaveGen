#include "stdafx.h"

SoundEffectSet::SoundEffectSet( float pan /* -1(L) 0 +1(R) */ )
	: m_panpot( pan )
{
}
SoundEffectSet::~SoundEffectSet()
{
	Release();
}
void SoundEffectSet::GetWave( float *pLeft, float *pRight, size_t blockSize )
{
	for( EFFECTLISTITR itr = m_effecters.begin(); itr!=m_effecters.end(); ++itr ) {
		(*itr)->Effect( pLeft, blockSize );
	}

	// todo: pan
	memcpy( pRight, pLeft, blockSize * sizeof(float) );
}
void SoundEffectSet::Push( SoundEffectBase *pSEB )
{
	pSEB->Reset();
	m_effecters.push_back( pSEB );
}
void SoundEffectSet::Release()
{
	for( EFFECTLISTITR itr = m_effecters.begin(); itr!=m_effecters.end(); ++itr ) {
		(*itr)->Release();
		delete (*itr);
	}
	m_effecters.clear();
}
