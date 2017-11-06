#pragma once

class SeqInputBase
{
public:
	SeqInputBase();
	virtual ~SeqInputBase();

	virtual bool Tick( DWORD dwTime )=0;
};
