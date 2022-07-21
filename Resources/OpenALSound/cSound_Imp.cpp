#include "cSound_Imp.h"

// This is from the OpenAL example code
#include "Framework.h"
// This includes the following files:
//		aldlist.cpp
//		aldlist.h
//		CWaves.cpp
//		CWaves.h
//		Framework.cpp
//		Framework.h
//		LoadOAL.cpp
//		LoadOAL.h

#include <iostream>

// This was forward declared in cSound_Imp because
//	of a windows.h include issue that I'm annoyed with.
// This struct has OpenAL things, so includes framework.h, 
//	which includes Windows.h. Windows.h had "PlaySound" 
//	which was having issues with a class method PlaySound
// Solution: PlaySound is now SoundSourcePlay, and 
//	this class is defined in the cpp file, instead.
// Not ideal, but it is what it is.
class cSound_Imp::sSoundInfo
{
public:
	sSoundInfo()	
	{
		uiBuffer = 0;	// Unknown
		uiSource = 0;	// Unknown
		iState = AL_INITIAL;
		iType = AL_UNDETERMINED;
		this->m_UniqueID = sSoundInfo::m_NextUniqueID;
		sSoundInfo::m_NextUniqueID++;
		return;
	};
	std::string name;
	std::string FileName;
	ALuint      uiBuffer;
	ALuint      uiSource;  
	ALint       iState;		// AL_SOURCE_STATE: AL_INITIAL, AL_PLAYING, AL_PAUSED, AL_STOPPED
	ALint		iType;		// AL_SOURCE_TYPE: AL_STATIC, AL_STREAMING, AL_UNDETERMINED
	std::string getStateString(void);
	std::string getTypeString(void);
	cSound::sSourceState getSourceState(void);
	cSound::sSourceType  getSourceType(void);
	
	int getUniqueID(void)	{ return this->m_UniqueID; }
private:
	int m_UniqueID;
	static int m_NextUniqueID;
};

//static
int cSound_Imp::sSoundInfo::m_NextUniqueID = 1000;	// Sounds start at 1000 (0 is an error)

cSound_Imp::cSound_Imp()
{

	return;
}

cSound_Imp::~cSound_Imp()
{
	return;
}

bool cSound_Imp::Init(std::string &error)
{
	//ALuint      uiBuffer;
	//ALuint      uiSource;  
	//ALint       iState;

	std::cout << "cSound: about the init OpenAL (assuming it's even there??)..." << std::endl;

	// Initialize Framework
	ALFWInit();

//	ALFWprintf("PlayStatic Test Application\n");

	if (!ALFWInitOpenAL())
	{
		error = "Failed to initialize OpenAL\n";
		ALFWprintf( error.c_str() );
		ALFWShutdown();
		return false;
	}

	this->m_bGoodToGo = true;

	return true;
}

bool cSound_Imp::ShutDown(void)
{
	// Clean up by deleting Source(s) and Buffer(s)
	//	alSourceStop(uiSource);
	//  alDeleteSources(1, &uiSource);
	//	alDeleteBuffers(1, &uiBuffer);

	for ( std::map<int, sSoundInfo*>::iterator itSound = this->m_map_pSoundsByID.begin(); 
		  itSound != this->m_map_pSoundsByID.end();	itSound++ )
	{
		sSoundInfo* pTheSound = itSound->second;

		alSourceStop( pTheSound->uiSource );
		alDeleteSources( 1, &(pTheSound->uiSource) );
		alDeleteBuffers( 1, &(pTheSound->uiBuffer) );

		delete pTheSound;
		
	}// for (std::map...

	// Clear the maps
	this->m_map_pSoundsByID.clear();
	this->m_map_SoundIDByName.clear();

	ALFWShutdownOpenAL();

	ALFWShutdown();

	this->m_bGoodToGo = false;

	return true;
}

bool cSound_Imp::IsGoodToGo(void)
{
	return this->m_bGoodToGo;
}

void cSound_Imp::setFileBasePath( std::string basePath )
{
	this->m_fileBasePath = basePath;
	return;
}


// Returns ID for sound, or 0 if there's an error
int cSound_Imp::LoadFromFile( std::string fileName, std::string friendlyName, cSound::sSourceType sourceType, bool overwriteIfPresent /*=false*/ )
{
	int soundID = this->FindSoundID(friendlyName);
	
	if ( soundID != 0) 
	{	// There's a sound there. Are we to overwrite it?
		if ( overwriteIfPresent)
		{	// Yes, so delete it
			this->Delete(soundID);
		}
		else
		{	// Don't overwrite, so return an error
			return 0;
		}//if ( overwriteIfPresent)
	}//if ( soundID != 0) 

	// At this point, there is no sound with that name, so add it

	fileName = this->m_fileBasePath + fileName;

	sSoundInfo* pNewSound = new sSoundInfo();
	pNewSound->FileName = fileName;
	pNewSound->name = friendlyName;
	pNewSound->iType = sourceType;
	
	alGenBuffers( 1, &(pNewSound->uiBuffer) );

	// Load Wave file into OpenAL Buffer
	//if (!ALFWLoadWaveToBuffer((char*)ALFWaddMediaPath(fileName.c_str()), uiBuffer))
	if (!ALFWLoadWaveToBuffer((char*)fileName.c_str(), pNewSound->uiBuffer))
	{	// Error
		alDeleteBuffers( 1, &(pNewSound->uiBuffer) );
		delete pNewSound;
		return 0;
	}//if (!ALFWLoadWaveToBuffer(...

	// Generate a Source to playback the buffer
	alGenSources( 1, &(pNewSound->uiSource) );

	// Attach Source to buffer
	alSourcei( pNewSound->uiSource, AL_BUFFER, pNewSound->uiBuffer );

	// Add the sound to the maps
	this->m_map_pSoundsByID[pNewSound->getUniqueID()] = pNewSound;
	this->m_map_SoundIDByName[pNewSound->name] = pNewSound->getUniqueID();

	return pNewSound->getUniqueID();
}


bool cSound_Imp::Play( std::string friendlyName, bool restartIfPlaying /*=true, which is default for OpenAL*/ )
{
	// Find sound in map
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID == 0 )
	{	// No sound of that name
		return false;	// Error;
	}

	return this->Play( soundID, restartIfPlaying );
}

bool cSound_Imp::Play( int soundUniqueID, bool restartIfPlaying /*=true, which is default for OpenAL*/ )
{
	cSound_Imp::sSoundInfo* pSound = this->FindSound(soundUniqueID);
	if ( pSound == nullptr )
	{	// There's no sound with that ID
		return false;	// Error
	}


	// OpenAL default is to restart the sound if it's already playing
	// So if we AREN'T doing this, then check state of sound
	if ( ! restartIfPlaying )
	{
		alGetSourcei( pSound->uiSource, AL_SOURCE_STATE, &(pSound->iState) );
		if ( pSound->iState == AL_PLAYING )
		{
			return true;	// Is already playing
		}
	}// if ( ! restartIfPlaying )

	// Either the sound isn't currently playing, or we want it restarted
	alSourcePlay( pSound->uiSource );

	return true;
}

bool cSound_Imp::Delete( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID == 0 )
	{	// Didn't find it
		return false;	// Error
	}

	return this->Delete( soundID );
}

bool cSound_Imp::Delete( int soundUniqueID )
{
	cSound_Imp::sSoundInfo* pSound = this->FindSound( soundUniqueID );
	if ( pSound == nullptr )
	{	// Didn't find it
		return false;	// Error
	}

	// Delete the things from the maps
	this->m_map_SoundIDByName.erase( pSound->name );
	this->m_map_pSoundsByID.erase( pSound->getUniqueID() );

	// Clean up by deleting Source(s) and Buffer(s)
	alSourceStop( pSound->uiSource );
	// In the example code, these happen immediately, so I assume we
	//	don't have to spin, waiting for the sound to stop or anything
    alDeleteSources(1, &(pSound->uiSource) );
	alDeleteBuffers(1, &(pSound->uiBuffer) );

	delete pSound;

	return true;
}

void cSound_Imp::LoopSound( std::string friendlyName, bool bLoop )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		this->LoopSound( soundID, bLoop );
	}
	return;
}

void cSound_Imp::LoopSound( int soundID, bool bLoop )
{
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alSourcei( pTheSound->uiSource, AL_LOOPING, ( bLoop ? AL_TRUE : AL_FALSE ) );
	}
	return;
}


float cSound_Imp::getPitch( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		return this->getPitch( soundID );
	}
	return 0.0f;	// Error condition (or the sound is actually at 0.0f pitch, I suppose
}

float cSound_Imp::getPitch ( int soundID )
{
	float pitch = 0.0f;
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alGetSourcef( pTheSound->uiSource, AL_PITCH, &pitch );
	}
	return pitch;
}

void cSound_Imp::setPitch( std::string friendlyName, float pitch )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		this->setPitch( soundID, pitch );
	}
	return;
}

void cSound_Imp::setPitch( int soundID, float pitch )
{
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alSourcef( pTheSound->uiSource, AL_PITCH, pitch );
	}
	return;
}

float cSound_Imp::getGain( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		return this->getGain( soundID );
	}
	return 0.0f;	// Error condition (or the sound is actually at 0.0f gain, I suppose
}

float cSound_Imp::getGain( int soundID )
{
	float gain = 0.0f;
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alGetSourcef( pTheSound->uiSource, AL_GAIN, &gain );
	}
	return gain;
}

void cSound_Imp::setGain( std::string friendlyName, float gain )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		this->setGain( soundID, gain );
	}
	return;
}

void cSound_Imp::setGain( int soundID, float gain )
{
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alSourcef( pTheSound->uiSource, AL_GAIN, gain );
	}
	return;
}

glm::vec3 cSound_Imp::getPosition( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		return this->getPosition( soundID );
	}
	return glm::vec3(0.0f);
}

glm::vec3 cSound_Imp::getPosition( int soundID )
{
	glm::vec3 position(0.0f);
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alGetSource3f( pTheSound->uiSource, AL_POSITION, &(position.x), &(position.y), &(position.z) );
	}
	return position;
}

glm::vec3 cSound_Imp::getVelocity( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		return this->getVelocity( soundID );
	}
	return glm::vec3(0.0f);
}

glm::vec3 cSound_Imp::getVelocity( int soundID )
{
	glm::vec3 velocity(0.0f);
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alGetSource3f( pTheSound->uiSource, AL_VELOCITY, &(velocity.x), &(velocity.y), &(velocity.z) );
	}
	return velocity;
}

glm::vec3 cSound_Imp::getDirection( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		return this->getDirection( soundID );
	}
	return glm::vec3(0.0f);
}

glm::vec3 cSound_Imp::getDirection( int soundID )
{
	glm::vec3 direction(0.0f);
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alGetSource3f( pTheSound->uiSource, AL_DIRECTION, &(direction.x), &(direction.y), &(direction.z) );
	}
	return direction;
}

void cSound_Imp::setPosition( std::string friendlyName, const glm::vec3 &position )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		this->setPosition( soundID, position );
	}
	return;
}

void cSound_Imp::setPosition( int soundID, const glm::vec3 &position )
{
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alSource3f( pTheSound->uiSource, AL_DIRECTION, position.x, position.y, position.z );
	}
	return;
}

void cSound_Imp::setVelocity( std::string friendlyName, const glm::vec3 &velocity )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		this->setVelocity( soundID, velocity );
	}
	return;
}

void cSound_Imp::setVelocity( int soundID, const glm::vec3 &velocity )
{
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alSource3f( pTheSound->uiSource, AL_VELOCITY, velocity.x, velocity.y, velocity.z );
	}
	return;
}

void cSound_Imp::setDirection( std::string friendlyName, const glm::vec3 &direction )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID != 0 )
	{
		this->setDirection( soundID, direction );
	}
	return;
}

void cSound_Imp::setDirection( int soundID, const glm::vec3 &direction )
{
	cSound_Imp::sSoundInfo* pTheSound = this->FindSound( soundID );
	if ( pTheSound != nullptr )
	{
		alSource3f( pTheSound->uiSource, AL_DIRECTION, direction.x, direction.y, direction.z );
	}
	return;
}









// Returns zero if there's no sound
int cSound_Imp::FindSoundID( std::string friendlyName )
{
	std::map<std::string, int>::iterator itSound =this->m_map_SoundIDByName.find( friendlyName );
	if ( itSound == this->m_map_SoundIDByName.end() )
	{	// Nope, didn't find it
		return 0;
	}
	// Found ID
	return itSound->second;
}


float cSound_Imp::getDopplerFactor(void)
{
	return alGetFloat( AL_DOPPLER_FACTOR );
}

void cSound_Imp::setDopplerFactor( float dopplerFactor )
{
	alDopplerFactor( dopplerFactor );
	return;
}

void cSound_Imp::resetDoppler(void)
{	// default is 1.0f
	this->setDopplerFactor( 1.0f );
	return;
}

float cSound_Imp::getSpeedOfSound(void)
{
	return alGetFloat( AL_SPEED_OF_SOUND );
}

void cSound_Imp::setSpeedOfSound( float speedOfSound )
{
	alSpeedOfSound( speedOfSound );
	return;
}

void cSound_Imp::resetSpeedOfSound(void)
{	// default is 343.3f
	this->setSpeedOfSound( 343.3f );
	return;
}


// Returns nullptr or zero if there's no sound
cSound_Imp::sSoundInfo* cSound_Imp::FindSound( std::string friendlyName )
{
	int soundID = this->FindSoundID( friendlyName );
	if ( soundID == 0 ) 
	{	// Didn't find it
		return nullptr;
	}
	
	std::map<int, sSoundInfo*>::iterator itSound = this->m_map_pSoundsByID.find(soundID);
	if ( itSound == this->m_map_pSoundsByID.end() )
	{	// Didn't find it
		return nullptr;
	}
	// Found it
	return itSound->second;
}

cSound_Imp::sSoundInfo* cSound_Imp::FindSound( int soundUniqueID )
{
	std::map<int, sSoundInfo*>::iterator itSound = this->m_map_pSoundsByID.find(soundUniqueID);
	if ( itSound == this->m_map_pSoundsByID.end() )
	{	// Didn't find it
		return nullptr;
	}
	// Found it
	return itSound->second;
}

std::string cSound_Imp::getLastError(bool clearAfterRead)
{
	std::string errorText = this->m_lastError;
	if (clearAfterRead)
	{
		this->m_lastError = "";
	}
	return errorText;
}


cSound::sSourceState  cSound_Imp::sSoundInfo::getSourceState(void)
{	// AL_SOURCE_STATE

	switch (this->iState)
	{
	case AL_INITIAL:	return cSound::sSourceState::STATE_INITIAL;		break;
	case AL_PLAYING:	return cSound::sSourceState::STATE_PLAYING;		break;
	case AL_PAUSED:		return cSound::sSourceState::STATE_PAUSED;		break;
	case AL_STOPPED:	return cSound::sSourceState::STATE_STOPPED;		break;
	}
	return cSound::sSourceState::STATE_UNKNOWN_OR_INVALID;
}

std::string cSound_Imp::sSoundInfo::getStateString(void)
{	// AL_SOURCE_STATE
	switch (this->iState)
	{
	case AL_INITIAL:	return "STATE_INITIAL";		break;
	case AL_PLAYING:	return "STATE_PLAYING";		break;
	case AL_PAUSED:		return "STATE_PAUSED";		break;
	case AL_STOPPED:	return "STATE_STOPPED";		break;
	}
	return "STATE_UNKNOWN_OR_INVALID";
}

cSound::sSourceType cSound_Imp::sSoundInfo::getSourceType(void)
{
	// AL_SOURCE_TYPE
	switch (this->iType)
	{
	case AL_STATIC:		return cSound::sSourceType::TYPE_STATIC;	break;
	case AL_STREAMING:	return cSound::sSourceType::TYPE_STREAMING;	break;
	case AL_UNDETERMINED:	return cSound::sSourceType::TYPE_UNDETERMINED;	break;
	}
	return cSound::sSourceType::TYPE_UNDETERMINED;
}

std::string cSound_Imp::sSoundInfo::getTypeString(void)
{
	// AL_SOURCE_TYPE
	switch (this->iType)
	{
	case AL_STATIC:			"TYPE_STATIC";			break;
	case AL_STREAMING:		"TYPE_STREAMING";		break;
	case AL_UNDETERMINED:	"TYPE_UNDETERMINED";	break;
	}
	return "TYPE_UNDETERMINED";
}
