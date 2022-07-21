#ifndef _cSound_Imp_HG_
#define _cSound_Imp_HG_

// Implementation class for PIMPL

#include "cSound.h"

#include <string>
#include <map>

//// This is from the OpenAL example code
//#include "Framework.h"
//// This includes the following files:
////		aldlist.cpp
////		aldlist.h
////		CWaves.cpp
////		CWaves.h
////		Framework.cpp
////		Framework.h
////		LoadOAL.cpp
////		LoadOAL.h
//
//class sSoundInfo
//{
//public:
//	sSoundInfo()	
//	{
//		uiSource = 0;	// Unknown
//		iState = AL_INITIAL;
//		iType = AL_UNDETERMINED;
//	};
//	std::string name;
//	std::string FileName;
//	ALuint      uiBuffer;
//	ALuint      uiSource;  
//	ALint       iState;		// AL_SOURCE_STATE: AL_INITIAL, AL_PLAYING, AL_PAUSED, AL_STOPPED
//	ALint		iType;		// AL_SOURCE_TYPE: AL_STATIC, AL_STREAMING, AL_UNDETERMINED
//	std::string getStateString(void);
//	std::string getTypeString(void);
//	cSound::sSourceState getSourceState(void);
//	cSound::sSourceType  getSourceType(void);
//};

class cSound_Imp
{
public:

	cSound_Imp();
	~cSound_Imp();

	// Forward declare because of windows.h include
	// This struct has OpenAL things, so includes framework.h, 
	//	which includes Windows.h. Windows.h had "PlaySound" 
	//	which was having issues with a class method PlaySound
	// Solution: PlaySound is now SoundSourcePlay, and 
	//	this class is defined in the cpp file, instead.
	// Not ideal, but it is what it is.
	class sSoundInfo;		

	bool Init(std::string &error);
	bool ShutDown(void);

	// Returns ID for sound, or 0 if there's an error
	int LoadFromFile( std::string fileName, std::string friendlyName, cSound::sSourceType type, bool overwriteIfPresent /*=false*/ );

	void setFileBasePath( std::string basePath );


	bool Play( std::string friendlyName, bool restartIfPlaying /*=true, which is default for OpenAL*/ );
	bool Play( int soundUniqueID, bool restartIfPlaying /*=true, which is default for OpenAL*/ );

	bool Delete( std::string friendlyName );
	bool Delete( int soundUniqueID );

	// Returns zero if there's no sound
	int FindSoundID( std::string friendlyName );
	// Returns nullptr or zero if there's no sound
	sSoundInfo* FindSound( std::string friendlyName );
	sSoundInfo* FindSound( int soundUniqueID );

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



	// Doppler and speed of sound
	float getDopplerFactor(void);
	void setDopplerFactor( float dopplerFactor );
	void resetDoppler(void);	// default is 1
	float getSpeedOfSound(void);
	void setSpeedOfSound( float speedOfSound );
	void resetSpeedOfSound(void);	// default is 343.3f


	std::string getLastError(bool clearAfterRead);

	bool IsGoodToGo(void);
private:

	std::map<std::string, int> m_map_SoundIDByName;
	std::map<int, sSoundInfo*> m_map_pSoundsByID;

	std::string m_lastError;

	std::string m_fileBasePath;

	// Indicates sound system is OK.
	bool m_bGoodToGo;
};

#endif
