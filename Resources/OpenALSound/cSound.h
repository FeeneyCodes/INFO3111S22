#ifndef _cSound_HG_
#define _cSound_HG_

// Header class for PIMPL implementation

#include <string>
#include "../include/glm/vec3.hpp"


class cSound_Imp;

class cSound
{
public:
	cSound();
	~cSound();

	bool Init(std::string &error );

	bool ShutDown(void);

	enum sSourceState	// AL_SOURCE_STATE
	{	// These are straight duplicates of the OpenAL source states
		STATE_INITIAL	= 0x1011,	// #define AL_INITIAL 0x1011
		STATE_PLAYING	= 0x1012, 
		STATE_PAUSED	= 0x1013, 
		STATE_STOPPED	= 0x1014,
		STATE_UNKNOWN_OR_INVALID = 0
	};

	enum sSourceType	// AL_SOURCE_TYPE
	{	// These are straight duplicates of the OpenAL source types
		TYPE_STATIC			= 0x1028,	// #define AL_STATIC 0x1028
		TYPE_STREAMING		= 0x1029,
		TYPE_UNDETERMINED	= 0x1030,
		TYPE_UNKNOWN_OR_INVALID = 0
	};

	std::string getSourceStateString( sSourceState sourceState );
	std::string getSourceTypeString( sSourceType sourceType );

	// Returns ID for sound, or 0 if there's an error
	int LoadFromFile( std::string fileName, std::string friendlyName, cSound::sSourceType type, bool overwriteIfPresent = false );

	// File name is DIRECTLY appended, so add final slash
	void setFileBasePath( std::string basePath );

	bool Play( std::string friendlyName, bool restartIfPlaying = true /*default for OpenAL*/ );

	bool Delete( std::string friendlyName );
	bool Delete( int soundID );

	void LoopSound( std::string friendlyName, bool bLoop );
	void LoopSound( int soundID, bool bLoop );

	float getPitch( std::string friendlyName );
	float getPitch ( int soundID );
	void setPitch( std::string friendlyName, float pitch );
	void setPitch( int soundID, float pitch );

	float getGain( std::string friendlyName );
	float getGain( int soundID );
	void setGain( std::string friendlyName, float gain );
	void setGain( int soundID, float gain );

	glm::vec3 getPosition( std::string friendlyName );
	glm::vec3 getPosition( int soundID );
	glm::vec3 getVelocity( std::string friendlyName );
	glm::vec3 getVelocity( int soundID );
	glm::vec3 getDirection( std::string friendlyName );
	glm::vec3 getDirection( int soundID );

	void setPosition( std::string friendlyName, const glm::vec3 &position );
	void setPosition( int soundID, const glm::vec3 &position );
	void setVelocity( std::string friendlyName, const glm::vec3 &velocity );
	void setVelocity( int soundID, const glm::vec3 &velocity );
	void setDirection( std::string friendlyName, const glm::vec3 &direction );
	void setDirection( int soundID, const glm::vec3 &direction );


	float getDopplerFactor(void);
	void setDopplerFactor( float dopplerFactor = 1.0f );
	void resetDoppler(void);	// default is 1.0f
	float getSpeedOfSound(void);
	void setSpeedOfSound( float speedOfSound = 343.3f );
	void resetSpeedOfSound(void);	// default is 343.3f

	bool IsGoodToGo(void);

	// Does NOT return an error if sound system ISN'T initialized. 
	void setFailQuietly( bool bFailQuietly );

private:
	cSound_Imp* m_pImp;

	bool m_bFailQuietly;

	const bool m_FAILQUIETLY_BOOL_RETURNVALUE = true;
	const int  m_FAILQUIETLY_INT_RETURNVALUE = 1;
};

#endif
