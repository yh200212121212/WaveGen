#include "stdafx.h"

#define SAFE_RELEASE(x)	{if(x){(x)->Release();(x)=nullptr;}}

SoundOutputBase::SoundOutputBase()
{
}

SoundOutputBase::~SoundOutputBase()
{
}
