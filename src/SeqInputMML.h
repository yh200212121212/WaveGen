#pragma once

#define MAX_TRACK	16

class SeqInputMML : public SeqInputBase
{
public:
	typedef struct tagTRACEINFO {
		BYTE	track;
		DWORD	tick;
	}TRACEINFO;
	typedef struct tagCOMPILEDINFO {
		int		errorCode;
		DWORD	errorLine;
		DWORD	maxTickCount;
		int		traceInfoCount;
		TRACEINFO *pTraceInfo;

		tagCOMPILEDINFO(){
			errorCode = 0;
			traceInfoCount = 0;
			pTraceInfo = nullptr;
		}
	}COMPILEDINFO;

	SeqInputMML();
	virtual ~SeqInputMML();

	bool Init( SoundManager *pManager );
	COMPILEDINFO CompileMML( const wchar_t *pMML );
	static const wchar_t* GetErrorString( int errorCode );
	virtual bool Tick( DWORD dwTime );
	static float GetFreq( BYTE note );
	void Play( DWORD dwTime, void(*playFinished)(void*)=nullptr, void *pPlayFinishedParam=nullptr );
	void Stop();

private:
	typedef struct tagPARAMVIBRATO{
		float	delayTime;
		float	sPower;
		float	aPower;
		float	aTime;
		float	dTime;
		float	sTime;
		float	rTime;
		float	hz;

		void Reset() {
			delayTime	= 0;
			hz			= 0;
			aPower		= 0;
			aTime		= 0;
			dTime		= 0;
			sPower		= 0;
			sTime		= 0;
			rTime		= 0;
		}
	}PARAMVIBRATO;

	typedef struct tagSOUNDSET {
		tagSOUNDSET(){
			pSoundSet = NULL;
			pGen = nullptr;
			pADSR = nullptr;
			pVolume = nullptr;
			pVibrato = nullptr;
			isVibrato = false;
		}
		SoundEffectSet	*pSoundSet;
		EffectGen		*pGen;
		SoundEffectADSR	*pADSR;
		SoundEffectVolume *pVolume;
		SoundEffectVibrato *pVibrato;
		bool isVibrato;
		PARAMVIBRATO	vibratoParam;
		EffectGen::eTYPE programNo;
	}SOUNDSET;

	typedef struct tagGLOBALINFO {
		bool		isFcMode;
		DWORD		playSkip;
		SOUNDSET	holder[MAX_TRACK];
		void Reset();
	}GLOBALINFO;

	enum eCMD {
		 CMD_TEMPO
		,CMD_END
		,CMD_TRACEINFO
		,CMD_PLAYSKIP
		,CMD_NOTE_ON
		,CMD_NOTE_OFF
		,CMD_NOTE_REST
		,CMD_PROGRAM_CHANGE
		,CMD_DUTY_CHANGE
		,CMD_ADSR
		,CMD_VIBRATO
		,CMD_VOLUME
		,CMD_FCMODE
	};
	typedef struct tagTOKEN{
		eCMD		command;		// command
		BYTE		track;			// Target track number
		DWORD		param;			// General parameters
		union {
			struct tagADSR{			// Parameters for CMD_ADSR
				float aPower;
				float aTime;
				float dTime;
				float sPower;
				float rTime;

				void Reset() {
					aPower = 1;
					aTime = 0;
					dTime = 0;
					sPower = 1;
					rTime = 0;
				}
			}paramADSR;
			struct tagNOTEON{		// Parameters for CMD_NOTE_ON
				BYTE	note;
				BYTE	sweepNote;
				float	sweepTime;
			}paramNoteOn;
			PARAMVIBRATO paramVibrato;
		}u1;
		DWORD		gateTick;		// Gate time (tick number)
	}TOKEN;

	const wchar_t *CompilePhase1( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const;
	std::vector<TOKEN> CompilePhase2( const wchar_t *pSource, int *pErrorCode, DWORD *pErrorLine ) const;
	std::vector<TOKEN> CompilePhase3( std::vector<TOKEN> tokens ) const;
	DWORD GetNoteTick( const wchar_t *pNoteSize, DWORD defaultTick, const wchar_t **ppExit ) const;
	DWORD GetNumber( const wchar_t *pString, const wchar_t **ppExit ) const;
	bool  GetNote( const wchar_t *pString, DWORD defaultTick, char octave, char *pNote, DWORD *pGateTime, const wchar_t **ppExit ) const;
	DWORD GetTempoToTick( DWORD tempo ) const;
	std::vector<std::wstring> GetParams( const wchar_t *pSource, const wchar_t **ppExit ) const;
	DWORD PlaySeq( DWORD index );
	void  Release();

	GLOBALINFO				m_info;
	SoundManager			*m_pManager;
	std::vector<TOKEN>		m_sequence;			// Sequence data
	DWORD					m_playIndex;		// Current sequence playback index
	DWORD					m_totalTick;		// Tick value from the start of playback to the present
	DWORD					m_nextTick;			// Tick to execute next Sequence
	DWORD					m_tickParSec;		// Tick value per second
	DWORD					m_prevTime;			// Time (ms) of the previous frame
	bool					m_isStop;			// True if it is stopped

	void(*m_fncPlayFinidhed)( void* );			// Called back at the end of playback
	void					*m_playFinishedParam;
};
