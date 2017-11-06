#include "stdafx.h"

#define TICKCOUNT	3840		//Resolution of one measure 3840 = 256 Minute note length triplet possible
#define ERR(x){	errorCode=(x); goto err; }

SeqInputMML::SeqInputMML()
	: m_pManager( nullptr )
	, m_isStop( true )
	, m_fncPlayFinidhed( nullptr )
{
}

SeqInputMML::~SeqInputMML()
{
	Release();
}

void SeqInputMML::Release()
{
	m_sequence.clear();
}

bool SeqInputMML::Init( SoundManager *pManager )
{
	if( pManager == nullptr )
		return true;

	m_pManager = pManager;

	// Make the registration set sound
	for( int i=0; i<MAX_TRACK; i++ ) {
		SOUNDSET ss;
		ss.pSoundSet = new SoundEffectSet();
		ss.pGen		 = new EffectGen( 220, EffectGen::SILENT );
		ss.pVibrato  = new SoundEffectVibrato();
		ss.pVibrato->SetEffectGen( ss.pGen );
		ss.pSoundSet->Push( ss.pVibrato );
		ss.pADSR	 = new SoundEffectADSR();						// ADSR effector coupled
		ss.pSoundSet->Push( ss.pADSR );									//... It may be set to sound
		ss.pVolume	 = new SoundEffectVolume();						// Volume effector coupled
		ss.pSoundSet->Push( ss.pVolume );								// ... It may be set to sound
		m_pManager->Push( ss.pSoundSet );
		m_info.holder[i] = ss;
	}

	m_playIndex = 0;

	return false;
}

//The note designated frequency number
float SeqInputMML::GetFreq( BYTE note )
{
	double freq = 28160;
	const double equalTempera = pow( 2, 1./12. );
	for( int i=129-(note&0x7f); i; i-- )
		freq /= equalTempera;
	return (float)freq;
}

DWORD SeqInputMML::PlaySeq( DWORD index )
{
	CriticalBlock cb( &m_pManager->m_cs );

	if( index >= m_sequence.size() )
		return 0;

	TOKEN token = m_sequence[ index ];
	if( token.track >= MAX_TRACK )
		return 0;

	SOUNDSET *pSS = &m_info.holder[ token.track ];

	switch( token.command )
	{
		case CMD_VIBRATO:{
					pSS->isVibrato = true;
					pSS->vibratoParam = token.u1.paramVibrato;
				}break;
		case CMD_NOTE_ON:{
					if( pSS->programNo == EffectGen::FCNOISE_S || pSS->programNo == EffectGen::FCNOISE_L ) {
						pSS->pGen->ChangeFCNoiseFreq( token.u1.paramNoteOn.note );
					}else{
						float freq = GetFreq( token.u1.paramNoteOn.note );
						if( token.u1.paramNoteOn.sweepTime > 0 ){
							pSS->pGen->ChangeFreqSweep( freq, GetFreq( token.u1.paramNoteOn.sweepNote ), token.u1.paramNoteOn.sweepTime );
						}else if( pSS->isVibrato ){
							pSS->pVibrato->ChangeParam( freq, pSS->vibratoParam.delayTime	, pSS->vibratoParam.sPower
															, pSS->vibratoParam.hz			, pSS->vibratoParam.aPower
															, pSS->vibratoParam.aTime		, pSS->vibratoParam.dTime
															, pSS->vibratoParam.sTime		, pSS->vibratoParam.rTime );
						}else{
							pSS->pGen->ChangeFreq( freq );
						}
					}
					pSS->pADSR->NoteOn();
				}break;
		case CMD_NOTE_OFF:
					pSS->pADSR->NoteOff();
				break;
		case CMD_PROGRAM_CHANGE: {
					EffectGen::eTYPE pg;
					switch( token.param ) {
						default:
						case 0:	pg = EffectGen::SQUARE;		break;
						case 1:	if( m_info.isFcMode ) {
									pg = EffectGen::FCTRIANGLE;
									pSS->pADSR->ChangeParam(  1, 0, 0, 1, 0  );
								}else{
									pg = EffectGen::TRIANGLE;
								}
							break;
						case 2:	pg = EffectGen::SAW;		break;
						case 3:	pg = EffectGen::SINEWAVE;	break;
						case 4:	pg = EffectGen::FCNOISE_L;	break;
						case 5:	pg = EffectGen::FCNOISE_S;	break;
					}
					pSS->pGen->ChangeType( pg );
					pSS->programNo = pg;
				}break;
		case CMD_DUTY_CHANGE: {
					pSS->pGen->ChangeSquareDuty( token.param / 1000.0f );
				}break;
		case CMD_TEMPO:
					m_tickParSec = token.param;
				break;
		case CMD_ADSR:
					if( m_info.isFcMode && pSS->pGen->GetType() == EffectGen::FCTRIANGLE )
						break;	// Based on the ADSR banned in FC mode
					pSS->pADSR->ChangeParam(  token.u1.paramADSR.aPower
											, token.u1.paramADSR.aTime
											, token.u1.paramADSR.dTime
											, token.u1.paramADSR.sPower
											, token.u1.paramADSR.rTime );
				break;
		case CMD_VOLUME:
					pSS->pVolume->ChangeVolume( token.param/381.0f );
				break;
		case CMD_FCMODE:
					m_info.isFcMode = token.param ? true : false;
				break;
	}

	return token.gateTick;
}

bool SeqInputMML::Tick( DWORD dwTime )
{
	if( m_isStop )
		return true;
	if( m_pManager == nullptr )
		return true;
	if( m_sequence.size() <= m_playIndex )
		return true;
	DWORD deltaTime = dwTime - m_prevTime;
	if( deltaTime <= 10 )
		return true;

	DWORD deltaTick = deltaTime * m_tickParSec / 1000;
	m_totalTick += deltaTick;
	m_prevTime = dwTime;
	while( m_nextTick <= m_totalTick ) {
		if( m_sequence.size() <= m_playIndex ) {
			m_isStop = true;
			if( m_fncPlayFinidhed )
				m_fncPlayFinidhed( m_playFinishedParam );
			return true;
		}

		if( m_nextTick > m_totalTick )
			return false;

		m_nextTick += PlaySeq( m_playIndex++ );
	}
	return false;
}

// Play
void SeqInputMML::Play( DWORD dwTime, void(*playFinished)(void*), void *pPlayFinishedParam )
{
	m_playFinishedParam = pPlayFinishedParam;
	m_fncPlayFinidhed = playFinished;

	m_prevTime = dwTime;
	m_totalTick = m_info.playSkip;
	m_tickParSec = 0;
	m_playIndex = 0;
	m_nextTick = 0;
	m_isStop = false;

	Tick( dwTime );
}

// Stop
void SeqInputMML::Stop()
{
	m_isStop = true;

	// Note off
	CriticalBlock cb( &m_pManager->m_cs );
	for( int i=0; i<MAX_TRACK; i++ ) {
		m_info.holder[i].pADSR->NoteOff();
	}
}

void SeqInputMML::GLOBALINFO::Reset()
{
	isFcMode = false;
	playSkip = 0;
}

// To compile MML
SeqInputMML::COMPILEDINFO SeqInputMML::CompileMML( const wchar_t *pMML )
{
	COMPILEDINFO compiledInfo;
	m_info.Reset();

	// Phase one
	const wchar_t *pPreCompiled = CompilePhase1( pMML, &compiledInfo.errorCode, &compiledInfo.errorLine );
	if( compiledInfo.errorCode != 0 )
		return std::move( compiledInfo );

	// Phase two
	m_sequence = CompilePhase2( pPreCompiled, &compiledInfo.errorCode, &compiledInfo.errorLine );
	delete pPreCompiled;
	if( compiledInfo.errorCode != 0 )
		return std::move( compiledInfo );

	// Phase three
	m_sequence = CompilePhase3( std::move( m_sequence ) );

	// Add the number of Tick function algorithm
	struct AlgTotalTick {
		DWORD operator()(DWORD sum, TOKEN token){ return sum + token.gateTick; }
	};

	// Count the number of Tick
	compiledInfo.maxTickCount = accumulate( m_sequence.begin(), m_sequence.end(), 0, AlgTotalTick() );

	// Process a specific command
	std::vector<std::vector<TOKEN>::iterator> debugInfos;
	for( std::vector<TOKEN>::iterator itr=m_sequence.begin(); itr!=m_sequence.end(); ++itr ) {
		// Trace information
		if( itr->command == CMD_TRACEINFO )
			debugInfos.push_back( itr );
		// Fast forward
		if( itr->command == CMD_PLAYSKIP )
			m_info.playSkip = accumulate( m_sequence.begin(), itr, 0, AlgTotalTick() );
	}

	// Since the forward end position is bad
	if( m_info.playSkip >= compiledInfo.maxTickCount )
		m_info.playSkip = 0;

/*
	// Formation of the position that the trace information
	if( (compiledInfo.traceInfoCount = debugInfos.size()) > 0 ) {
		TRACEINFO *pTI = compiledInfo.pTraceInfo = new TRACEINFO[compiledInfo.traceInfoCount];
		for( std::vector<std::vector<TOKEN>::iterator>::iterator itr=debugInfos.begin(); itr!=debugInfos.end(); ++itr ) {
			pTI->track = (*itr)->track;
			pTI->tick = accumulate( m_sequence.begin(), *itr, 0, AlgTotalTick() );
		}
	}
*/

	return std::move( compiledInfo );
}

// Returns the numeric
DWORD SeqInputMML::GetNumber( const wchar_t *pString, const wchar_t **ppExit ) const
{
	DWORD num = 0;
	while( *pString>='0' && *pString<='9' ) {
		num *= 10;
		num += *pString - '0';
		pString++;
	}
	if( ppExit )
		*ppExit = pString;
	return num;
}

// Take a rest. RET: syntax error is true
bool SeqInputMML::GetNote( const wchar_t *pString, DWORD defaultTick, char octave, char *pNote, DWORD *pGateTime, const wchar_t **ppExit ) const
{
	char note = -1;
	bool isRest = false;
	if( *pString>='a' && *pString<='g' ) {
		static const char tbl[7] = {9,11,0,2,4,5,7};
		note = tbl[*pString-'a'];
		pString++;
		if( *pString == '+' ) {
			note++;
			pString++;
		} else if( *pString == '-' ) {
			note--;
			pString++;
		}
	}else if( *pString == 'r' ) {
		isRest = true;
		pString++;
	}else{
		return true;
	}

	// Tone acquisition
	DWORD gateTime = GetNoteTick( pString, defaultTick, &pString );

	if( !isRest )
		note += octave * 12;
	if( pNote )
		*pNote = (char)note;
	if( pGateTime )
		*pGateTime = gateTime;
	if( ppExit )
		*ppExit = pString;

	return false;
}

// Converting point value corresponding to the TICK * from character
DWORD SeqInputMML::GetNoteTick( const wchar_t *pNoteSize, DWORD defaultTick, const wchar_t **ppExit ) const
{
	DWORD noteBase = GetNumber( pNoteSize, &pNoteSize );
	if( noteBase != 0 )
		defaultTick = TICKCOUNT / noteBase;
	noteBase = defaultTick;
	while( *pNoteSize=='.' ) {
		defaultTick /= 2;
		noteBase += defaultTick;
		pNoteSize++;
	}

	if( ppExit )
		*ppExit = pNoteSize;

	return noteBase;
}

// The value of one second TICK conversion from the tempo
DWORD SeqInputMML::GetTempoToTick( DWORD tempo ) const
{
	return TICKCOUNT * tempo / 240;
}

// Phase 1: character compilation
const wchar_t *SeqInputMML::CompilePhase1( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const
{
	bool isLineComment = false;
	bool isBlockComment = false;;
	int loopCount = 0;
	int parenCount = 0;
	wchar_t *pBuffer = new wchar_t[ wcslen(pSource)+1 ];
	wchar_t *w = pBuffer;
	int errorCode;
	DWORD dwLineCount = 1;


	// Counting the number of lines, to unify the newline in \n
	while( *pSource ) {
		if( *pSource=='\r' ) {
			pSource++;
			if( *pSource=='\n' )
				pSource++;
			dwLineCount++;
			*(w++)='\n';
		}else if( *pSource=='\n' ) {
			pSource++;
			if( *pSource=='\r' )
				pSource++;
			dwLineCount++;
			*(w++)='\n';
		}else{
			*(w++) = *(pSource++);
		}
	}
	*w = '\0';

	wchar_t *pWork = new wchar_t[ wcslen(pBuffer)+sizeof(DWORD)*dwLineCount+1 ];
	wmemcpy( pWork, pBuffer, wcslen(pBuffer)+sizeof(wchar_t) );

	pSource = pBuffer;
	w = pWork;
	dwLineCount = 1;
	while( *pSource ) {
		// Line?
		if( *pSource=='\n' ) {
			isLineComment = false;
			*(w++) = *(pSource++);
			*(DWORD*)w = ++dwLineCount;
			w += 2;
			continue;
		}

		// Line comment?
		if( wcsncmp( pSource, L"//", 2 ) == 0 ) {
			pSource += 2;
			isLineComment = true;
			continue;
		}

		// Comments must be in line with
		if( isLineComment==false ) {
			// Block comment end?
			if(  wcsncmp( pSource, L"*/", 2 ) == 0 ) {
				pSource += 2;
				if( isBlockComment )	isBlockComment = false;
				else					ERR(15);
				continue;
			}
			// Block comment?
			if( wcsncmp( pSource, L"/*", 2 ) == 0 ) {
				pSource += 2;
				isBlockComment = true;
				continue;
			}
		}

		// The following shall not in any comments
		if( isBlockComment || isLineComment ) {
			pSource++;
			continue;
		}

		// Fill the blank
		if( *pSource == ' ' || *pSource == '\t' ) {
			pSource++;
			continue;
		}
		// It's S-JIS EM space.
		if( pSource[0]==129 && pSource[1]==64 ) {
			pSource+=2;
			continue;
		}
		// Checking the number of loop blocks
		if( *pSource == '[' )
			loopCount++;
		if( *pSource == ']' ) {
			// There are more ends than starting
			if( loopCount == 0 )
				ERR(16);
			loopCount--;
		}
		// Check number of parentheses
		if( *pSource == '(' )
			parenCount++;
		if( *pSource == ')' ) {
			// There are more ends than starting
			if( parenCount == 0 )
				ERR(17);
			parenCount--;
		}

		// UPPERCASE LOWERCASE
		if( *pSource>='A' && *pSource<='Z' )
			*(w++) = *(pSource++) | 0x20;
		else			
			*(w++) = *(pSource++);
	}
	// Loop not closed
	if( loopCount != 0 )
		ERR(14);
	// The parentheses are not closed
	if( parenCount != 0 )
		ERR(18);

	*w = '\0';
	delete pBuffer;

	return pWork;

err:
	*pErrorCode = errorCode;
	*pErrorLine = dwLineCount;
	delete pWork;
	delete pBuffer;
	return nullptr;
}

// Get a character string up to closing parenthesis
std::vector<std::wstring> SeqInputMML::GetParams( const wchar_t *pSource, const wchar_t **ppExit ) const
{
	std::vector<std::wstring> params;
	const wchar_t *pEnd = pSource;
	while( *pEnd != ')' ) {
		if( *pEnd==',' ) {
			params.push_back( std::wstring( pSource, pEnd ) );
			pEnd++;
			pSource = pEnd;
		}else{
			pEnd++;
		}
	}
	if( pSource != pEnd ) {
		params.push_back( std::wstring( pSource, pEnd ) );
	}

	if( ppExit )
		*ppExit = pEnd+1;

	return std::move( params );
}

// Compilation Phase 2: Token generation
std::vector<SeqInputMML::TOKEN> SeqInputMML::CompilePhase2( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const
{
	// Work structure required during compilation
	typedef struct tagTOKENWORK : TOKEN {
		DWORD		startTick;
		DWORD		index;
	}TOKENWORK;

	// Loop block information
	typedef struct tagLOOPINFO {
		const wchar_t *pStart;
		const wchar_t *pEscape;
		const wchar_t *pEnd;
		DWORD loopCount;
	}LOOPINFO;
	std::stack<LOOPINFO> loopStack;

	// Track information
	typedef struct tagTRACKWORK {
		std::vector<TOKENWORK> tokens;
	}TRACKWORK;

	TOKENWORK token;
	std::vector<TOKENWORK> resultTokens, *pTokens;
	std::vector<TOKEN> result;
	TRACKWORK trackWork[MAX_TRACK];
	char	onceOctave = 0;
	bool	isSweep = false;
	DWORD	dwLineCount = 1;
	int		errorCode = 0;
	size_t	emptyTrackCount = 0;

	// Set all optional values
	int		currentOctave = 5;
	int		currentTrack = 0;
	DWORD	defaultTick = TICKCOUNT / 4;
	int		noteOnGate = 80;

	// Sound source reset
	for( int i=0; i<MAX_TRACK; i++ ) {
		//Sound
		token.command = CMD_PROGRAM_CHANGE;
		token.param = 0;
		token.gateTick = 0;
		trackWork[i].tokens.push_back( token );

		// volume
		token.command = CMD_VOLUME;
		token.param = 100;
		token.gateTick = 0;
		trackWork[i].tokens.push_back( token );

		// ADSR invalid
		token.command = CMD_ADSR;
		token.gateTick = 0;
		token.u1.paramADSR.Reset();
		trackWork[i].tokens.push_back( token );

		// Vib invalid
		token.command = CMD_VIBRATO;
		token.gateTick = 0;
		token.u1.paramVibrato.Reset();
		trackWork[i].tokens.push_back( token );
	}
	emptyTrackCount = trackWork[0].tokens.size();

	// Tempo is...
	pTokens = &trackWork[ currentTrack ].tokens;
	token.command = CMD_TEMPO;
	token.param = GetTempoToTick( 128 );
	token.gateTick = 0;
	pTokens->push_back( token );

	while( *pSource ) {
		token.gateTick = 0;

		// Line feed code
		if( *pSource == '\n' ) {
			pSource++;
			dwLineCount = *(DWORD*)pSource;
			pSource += 2;
			continue;
		}

		// ADSR effect designation?
		if( wcsncmp( pSource, L"adsr(", 5 ) == 0 ) {
			std::vector<std::wstring> params = GetParams( pSource+5, &pSource );
			if( params.size() != 5 )
				ERR(1)	// Invalid number of ADSR arguments
			token.command = CMD_ADSR;
			token.gateTick = 0;
			token.u1.paramADSR.aPower = (float)_wtof( params[0].c_str() );
			token.u1.paramADSR.aTime  = (float)_wtof( params[1].c_str() );
			token.u1.paramADSR.dTime  = (float)_wtof( params[2].c_str() );
			token.u1.paramADSR.sPower = (float)_wtof( params[3].c_str() );
			token.u1.paramADSR.rTime  = (float)_wtof( params[4].c_str() );
			pTokens->push_back( token );
			continue;
		}

		// Definition?
		if( wcsncmp( pSource, L"#fc(", 4 ) == 0 ) {
			std::vector<std::wstring> params = GetParams( pSource+4, &pSource );
			if( params.size() != 1 )
				ERR(21)	// Invalid number of FC arguments
			token.command = CMD_FCMODE;
			token.gateTick = 0;
			token.param = (int)_wtoi( params[0].c_str() );
			if( token.param & ~1 )
				ERR(21)
			pTokens->push_back( token );
			continue;
		}

		// Vibrato effect designation?
		if( wcsncmp( pSource, L"vibrato(", 8 ) == 0 ) {
			std::vector<std::wstring> params = GetParams( pSource+8, &pSource );
			if( params.size() != 8 ) 
					ERR(20)	// VIBRATO number of arguments is invalid
			token.command = CMD_VIBRATO;
			token.gateTick = 0;
			token.u1.paramVibrato.delayTime	= (float)_wtof( params[0].c_str() );
			token.u1.paramVibrato.hz		= (float)_wtof( params[1].c_str() );
			token.u1.paramVibrato.aPower	= (float)_wtof( params[2].c_str() );
			token.u1.paramVibrato.aTime		= (float)_wtof( params[3].c_str() );
			token.u1.paramVibrato.dTime		= (float)_wtof( params[4].c_str() );
			token.u1.paramVibrato.sPower	= (float)_wtof( params[5].c_str() );
			token.u1.paramVibrato.sTime		= (float)_wtof( params[6].c_str() );
			token.u1.paramVibrato.rTime		= (float)_wtof( params[7].c_str() );

			// If hz is 0 or Time is all 0, it is released
			if( token.u1.paramVibrato.hz == 0 || (token.u1.paramVibrato.aTime == 0 && token.u1.paramVibrato.dTime == 0 && token.u1.paramVibrato.sTime == 0 && token.u1.paramVibrato.rTime == 0 ) )
				token.u1.paramVibrato.Reset();

			pTokens->push_back( token );
			continue;
		}

		// トラックチェンジ？
		if( wcsncmp( pSource, L"track(", 6 ) == 0 ) {
			if( !loopStack.empty() )
				ERR(2)
			if( isSweep )
				ERR(3)
			std::vector<std::wstring> params = GetParams( pSource+6, &pSource );
			if( params.size() != 1 )
				ERR(4)
			currentTrack = _wtoi( params[0].c_str() ) % MAX_TRACK;
			pTokens = &trackWork[ currentTrack ].tokens;
			continue;
		}

		switch( *pSource ) {
			case '!':	token.command = CMD_TRACEINFO;
						token.param = GetNumber( pSource+1, &pSource );
						pTokens->push_back( token );
					break;
			case 'o':{	const wchar_t *pStart = pSource+1;
						currentOctave = GetNumber( pStart, &pSource );
						if( pStart == pSource || currentOctave>8 )
							ERR(5)
					}break;
			case 'l':{	const wchar_t *pStart = pSource+1;
						defaultTick = GetNoteTick( pStart, defaultTick, &pSource );
						if( pStart == pSource )
							ERR(6)
					}break;
			case 't':	token.command = CMD_TEMPO;
						token.param = GetTempoToTick( GetNumber( pSource+1, &pSource ) );
						if( token.param == 0 )
							ERR(7)
						pTokens->push_back( token );
					break;
			case '<':	currentOctave--;
						if( currentOctave < 0 )
							currentOctave = 0;
						pSource++;
					break;
			case '>':	currentOctave++;
						if( currentOctave > 8 )
							currentOctave = 8;
						pSource++;
					break;
			case 'q':	noteOnGate = GetNumber( pSource+1, &pSource );
						if( noteOnGate < 0 )		noteOnGate = 0;
						else if( noteOnGate > 100 )	noteOnGate = 100;
					break;
			case '@':	if( pSource[1] == 'w' ) {
							token.command = CMD_DUTY_CHANGE;
							token.param = GetNumber( pSource+2, &pSource );
						}else if( pSource[1] == '@' ) {
							token.command = CMD_PLAYSKIP;
							token.param = GetNumber( pSource+2, &pSource );
							pTokens->push_back( token );
						}else{
							token.command = CMD_PROGRAM_CHANGE;
							token.param = GetNumber( pSource+1, &pSource );
						}
						pTokens->push_back( token );
					break;
			case 'v':	token.command = CMD_VOLUME;
						token.param = GetNumber( pSource+1, &pSource );
						pTokens->push_back( token );
					break;
			case 'c':	case 'd':	case 'e':	case 'f':
			case 'g':	case 'a':	case 'b':	case 'r':{
						char note;
						DWORD gateTime;
						if( GetNote( pSource, defaultTick, (char)(onceOctave+currentOctave), &note, &gateTime, &pSource ) )
							ERR(8)

						if( note >= 0 ) {
							if( isSweep ) {
								// Sweep
								TOKENWORK *pOff, *pOn = &(*pTokens)[pTokens->size()-1];
								size_t i;
								for( i=pTokens->size(); i; --i, --pOn ) {
									if( pOn->command == CMD_NOTE_ON )
										break;
								}
								if( i ) {
									pOff = pOn+1;
									if( pOff->command != CMD_NOTE_OFF )
										ERR(9)

									// Mixing length
									gateTime = pOn->gateTick + gateTime + pOff->gateTick;
									pOn->gateTick  = gateTime * noteOnGate / 100;
									pOn->u1.paramNoteOn.sweepTime = pOn->gateTick / (float)TICKCOUNT;
									pOff->gateTick = gateTime - pOn->gateTick;

									if( pOn->u1.paramNoteOn.note != note ) {
										// Slur
										pOn->u1.paramNoteOn.sweepNote = note;
										pOn->u1.paramNoteOn.sweepTime = pOn->gateTick / (float)TICKCOUNT;
										if( pOn->u1.paramNoteOn.sweepNote == pOn->u1.paramNoteOn.note )
											pOn->u1.paramNoteOn.sweepTime = 0;	// If the origin and scale in the same thailand
									}else{
										pOn->u1.paramNoteOn.sweepTime = 0;
									}
								}
							}else{
								token.command = CMD_NOTE_ON;
								token.u1.paramNoteOn.note = note;
								token.gateTick = gateTime * noteOnGate / 100;
								token.u1.paramNoteOn.sweepTime = 0;
								pTokens->push_back( token );

								token.command = CMD_NOTE_OFF;
								token.gateTick = gateTime - token.gateTick;
								pTokens->push_back( token );
							}

							if( *pSource == '&' ) {
								isSweep = true;
								pSource++;
							}else{
								isSweep = false;
							}
						}else{
							if( isSweep )
								ERR(10)
							token.command = CMD_NOTE_REST;
							token.param = 1000;
							token.gateTick = gateTime;
							pTokens->push_back( token );
						}
					}break;
			case '`':	onceOctave = 1;
						pSource++;
					continue;
			case '"':	onceOctave = -1;
						pSource++;
					continue;
			case '[':{	LOOPINFO loopInfo;
						loopInfo.loopCount = GetNumber( pSource+1, &pSource );
						if( loopInfo.loopCount < 2 )
							loopInfo.loopCount = 2;	// At least twice
						loopInfo.pStart = pSource;
						loopInfo.pEscape = nullptr;
						loopInfo.pEnd = nullptr;
						loopStack.push( loopInfo );
					}break;
			case ':':{	if( loopStack.empty() )
							ERR(11)
						LOOPINFO loopInfo = loopStack.top();
						if( loopInfo.loopCount == 1 ) {
							loopStack.pop();
							pSource = loopInfo.pEnd;
						}else{
							pSource++;
							if( loopInfo.pEscape == nullptr ) {
								loopStack.pop();
								loopInfo.pEscape = pSource;
								loopStack.push( loopInfo );
							}else if( loopInfo.pEscape != pSource )
								ERR(12)
						}
					}break;
			case ']':{	if( loopStack.empty() )
							ERR(13)
						LOOPINFO loopInfo = loopStack.top();
						loopStack.pop();
						loopInfo.loopCount--;
						if( loopInfo.loopCount == 0 ) {
							// End loop
							pSource++;
						}else{
							loopInfo.pEnd = pSource+1;
							pSource = loopInfo.pStart;
							loopStack.push( loopInfo );
						}
					}break;
			default:	ERR(19)
		}
		onceOctave = 0;
	}
	if( !loopStack.empty() )
		ERR(14)

	// The reproduction start time is arranged for each index and the track Tick to one write track information merge
	DWORD tokenIndex = 0;
	for( BYTE i=0; i<MAX_TRACK; i++ ) {
		DWORD dwTotalTime = 0;
		if( trackWork[i].tokens.size() > emptyTrackCount ) {
			for( std::vector<tagTOKENWORK>::iterator itr=trackWork[i].tokens.begin(); itr!=trackWork[i].tokens.end(); ++itr ) {
				itr->startTick = dwTotalTime;
				itr->track = i;
				itr->index = tokenIndex++;
				dwTotalTime += itr->gateTick;
				resultTokens.push_back( *itr );
			}
		}
	}

	// In the reproduction start time from the track to the merge sort
	struct LessToken {
		bool operator()(const tagTOKENWORK& riLeft, const tagTOKENWORK& riRight) const {
			if( riLeft.startTick == riRight.startTick )	// If the same time quickly defined
				return riLeft.index < riRight.index;
			return riLeft.startTick < riRight.startTick;
		}
	};
	std::sort( resultTokens.begin(), resultTokens.end(), LessToken() );

	// Between the time before and after the read command is recomputed, unnecessary to remove the work put in again
	size_t count = resultTokens.size()-1;
	for( size_t i=0; i<count; i++ ) {
		resultTokens[i].gateTick = resultTokens[i+1].startTick - resultTokens[i].startTick;
		result.push_back( resultTokens[i] );
	}
	result.push_back( resultTokens[resultTokens.size()-1] );

	// In the end marker
	token.command = CMD_END;
	token.gateTick = 0;
	result.push_back( token );

	*pErrorCode = 0;
	*pErrorLine = 0;
	return std::move( result );

err:
	*pErrorCode = errorCode;
	*pErrorLine = dwLineCount;

	result.clear();
	return std::move( result );
}

// Phase 3: optimizing compilation
std::vector<SeqInputMML::TOKEN> SeqInputMML::CompilePhase3( std::vector<TOKEN> tokens ) const
{
	{
		TOKEN dummyToken;
		dummyToken.command = (eCMD)-1;
		tokens.push_back( dummyToken );
	}
	std::vector<TOKEN> result;
	TOKEN *pToken = &tokens[0], *pPrevToken = nullptr;
	for( size_t i=tokens.size(); i>0; i--, pToken++ ) {
		if( pPrevToken ) {
			// Mix the rest at the same time, if the gate around the track
			if( pPrevToken->track == pToken->track
			 && pToken->command == CMD_NOTE_OFF
			 && pPrevToken->command == CMD_NOTE_OFF )
			{
				pPrevToken->gateTick += pToken->gateTick;
			}else{
				result.push_back( *pPrevToken );
				pPrevToken = pToken;
			}
		}else{
			pPrevToken = pToken;
		}
	}

	return result;
}

const wchar_t* SeqInputMML::GetErrorString( int errorCode )
{
	switch( errorCode )
	{
		case 0:	return L"No error";
		case 1:	return L"ADSR argument is invalid";
		case 2:	return L"The loop block ([) is changing the track without being closed";
		case 3: return L"I am changing the track during sweep (&)";
		case 4: return L"Invalid argument of track change";
		case 5: return L"Octave (o) Invalid argument 0 - 8";
		case 6: return L"Default tone specification (l) is invalid";
		case 7: return L"Tempo specification (t) is invalid 0 - 8";
		case 8: return L"Scale acquisition error";
		case 9: return L"Sweep analysis error";
		case 10:return L"A rest (r) is specified after the sweep (&)";
		case 11:return L"There is a loop escape mark (:) outside the loop block";
		case 12:return L"There are multiple escape marks (:) in the loop block ([)";
		case 13:return L"There is a loop end mark (]) without a loop block ([)";
		case 14:return L"Loop block ([) is not closed";
		case 15:return L"There is an endless comment block termination (* /)";
		case 16:return L"There is an unexpected loop block termination (])";
		case 17:return L"There is an unexpected parameter block termination ())";
		case 18:return L"Parameter block (() is not closed";
		case 19:return L"I have an unknown order";
		case 20:return L"VIBRATO argument is invalid";
		case 21:return L"#FC argument is invalid 0 or 1 is specified"; 
	}
	return L"Unknown error";
}
