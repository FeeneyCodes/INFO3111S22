#include "cSound.h"

#include "cSound_Imp.h"

#include <new>

cSound::cSound()
{
	// Create instance (don't check if it wasn't set)
	this->m_pImp = new(std::nothrow) cSound_Imp();

	// By default, we ignore any sound request, 
	//	
	this->m_bFailQuietly = true;
	return;
}


cSound::~cSound()
{
	if ( this->m_pImp != nullptr )
	{
		delete this->m_pImp;
	}
	this->m_pImp = nullptr;

	return;
}

bool cSound::Init(std::string &error)
{
	return this->m_pImp->Init( error );
}

bool cSound::IsGoodToGo(void)
{
	return this->m_pImp->IsGoodToGo();
}

void cSound::setFailQuietly( bool bFailQuietly )
{
	this->m_bFailQuietly = bFailQuietly;
	return;
}

void cSound::setFileBasePath( std::string basePath )
{
	this->m_pImp->setFileBasePath( basePath );
	return;
}


// Returns ID for sound, or 0 if there's an error
int cSound::LoadFromFile( std::string fileName, std::string friendlyName, cSound::sSourceType type, bool overwriteIfPresent /*=false*/ )
{
	if ( ! this->m_pImp->IsGoodToGo() )
	{
		if ( this->m_bFailQuietly )	
		{ 
			return cSound::m_FAILQUIETLY_INT_RETURNVALUE;	// Lie, returning OK 
		}// if ( this->m_bFailQuietly )	
	}// if ( ! this->m_pImp->IsGoodToGo() )

	return this->m_pImp->LoadFromFile( fileName, friendlyName, type, overwriteIfPresent );
}

bool cSound::Play( std::string friendlyName, bool restartIfPlaying /*=true, which is default for OpenAL*/ )
{
	if ( ! this->m_pImp->IsGoodToGo() )
	{
		if ( this->m_bFailQuietly )	
		{ 
			return cSound::m_FAILQUIETLY_BOOL_RETURNVALUE;	// Lie, returning OK 
		}// if ( this->m_bFailQuietly )	
	}// if ( ! this->m_pImp->IsGoodToGo() )

	return this->m_pImp->Play( friendlyName, restartIfPlaying );
}

bool cSound::Delete( std::string friendlyName )
{
	if ( ! this->m_pImp->IsGoodToGo() )
	{
		if ( this->m_bFailQuietly )	
		{ 
			return cSound::m_FAILQUIETLY_BOOL_RETURNVALUE;	// Lie, returning OK 
		}// if ( this->m_bFailQuietly )	
	}// if ( ! this->m_pImp->IsGoodToGo() )

	return this->m_pImp->Delete( friendlyName );
}

bool cSound::Delete( int soundID )
{
	if ( ! this->m_pImp->IsGoodToGo() )
	{
		if ( this->m_bFailQuietly )	
		{ 
			return cSound::m_FAILQUIETLY_BOOL_RETURNVALUE;	// Lie, returning OK 
		}// if ( this->m_bFailQuietly )	
	}// if ( ! this->m_pImp->IsGoodToGo() )

	return this->m_pImp->Delete( soundID );
}

bool cSound::ShutDown(void)
{
	if ( ! this->m_pImp->IsGoodToGo() )
	{
		if ( this->m_bFailQuietly )	
		{ 
			return cSound::m_FAILQUIETLY_BOOL_RETURNVALUE;	// Lie, returning OK 
		}// if ( this->m_bFailQuietly )	
	}// if ( ! this->m_pImp->IsGoodToGo() )

	return this->m_pImp->ShutDown();
}

void cSound::LoopSound( std::string friendlyName, bool bLoop )
{
	if ( this->m_pImp->IsGoodToGo() )
	{
		this->m_pImp->LoopSound( friendlyName, bLoop );
	}
	return;
}

void cSound::LoopSound( int soundID, bool bLoop )
{
	if ( this->m_pImp->IsGoodToGo() )
	{
		this->m_pImp->LoopSound( soundID, bLoop );
	}
	return;
}

float cSound::getPitch( std::string friendlyName )
{
	return this->m_pImp->getPitch( friendlyName );
}

float cSound::getPitch ( int soundID )
{
	return this->m_pImp->getPitch( soundID );
}

void cSound::setPitch( std::string friendlyName, float pitch )
{
	this->m_pImp->setPitch( friendlyName, pitch );
	return;
}

void cSound::setPitch( int soundID, float pitch )
{
	this->m_pImp->setPitch( soundID, pitch );
	return;
}

float cSound::getGain( std::string friendlyName )
{
	return this->m_pImp->getGain( friendlyName );
}

float cSound::getGain( int soundID )
{
	return this->m_pImp->getGain( soundID );
}

void cSound::setGain( std::string friendlyName, float gain )
{
	this->m_pImp->setGain( friendlyName, gain );
	return;
}

void cSound::setGain( int soundID, float gain )
{
	this->m_pImp->setGain( soundID, gain );
	return;
}

float cSound::getDopplerFactor(void)
{
	return this->m_pImp->getDopplerFactor();
}

void cSound::setDopplerFactor( float dopplerFactor /*=1.0f*/ )
{
	this->m_pImp->setDopplerFactor( dopplerFactor );
	return;
}

void cSound::resetDoppler(void)
{
	this->m_pImp->resetDoppler();
	return;
}

float cSound::getSpeedOfSound(void)
{
	return this->m_pImp->getSpeedOfSound();
}

void cSound::setSpeedOfSound( float speedOfSound /*=343.3f*/ )
{
	this->m_pImp->setSpeedOfSound( speedOfSound );
	return;
}


glm::vec3 cSound::getPosition( std::string friendlyName )
{
	return this->m_pImp->getPosition( friendlyName );
}

glm::vec3 cSound::getPosition( int soundID )
{
	return this->m_pImp->getPosition( soundID );
}

glm::vec3 cSound::getVelocity( std::string friendlyName )
{
	return this->m_pImp->getVelocity( friendlyName );
}

glm::vec3 cSound::getVelocity( int soundID )
{
	return this->m_pImp->getVelocity( soundID );
}

glm::vec3 cSound::getDirection( std::string friendlyName )
{
	return this->m_pImp->getDirection( friendlyName );
}

glm::vec3 cSound::getDirection( int soundID )
{
	return this->m_pImp->getDirection( soundID );
}

void cSound::setPosition( std::string friendlyName, const glm::vec3 &position )
{
	this->m_pImp->setPosition( friendlyName, position );
	return;
}

void cSound::setPosition( int soundID, const glm::vec3 &position )
{
	this->m_pImp->setPosition( soundID, position );
	return;
}

void cSound::setVelocity( std::string friendlyName, const glm::vec3 &velocity )
{
	this->m_pImp->setVelocity( friendlyName, velocity );
	return;
}

void cSound::setVelocity( int soundID, const glm::vec3 &velocity )
{
	this->m_pImp->setVelocity( soundID, velocity );
	return;
}

void cSound::setDirection( std::string friendlyName, const glm::vec3 &direction )
{
	this->m_pImp->setDirection( friendlyName, direction );
	return;
}

void cSound::setDirection( int soundID, const glm::vec3 &direction )
{
	this->m_pImp->setDirection( soundID, direction );
	return;
}


void cSound::resetSpeedOfSound(void)
{	// default is 343.3f
	this->m_pImp->resetSpeedOfSound();
	return;
}


std::string cSound::getSourceStateString( sSourceState sourceState )
{
	switch ( sourceState )
	{
	case STATE_INITIAL:	return "STATE_INITIAL";		break;
	case STATE_PLAYING:	return "STATE_PLAYING";		break;
	case STATE_PAUSED:	return "STATE_PAUSED";		break;
	case STATE_STOPPED:	return "STATE_STOPPED";		break;
	}
	return "UNKNOWN SOURCE STATE";
}

std::string cSound::getSourceTypeString( sSourceType sourceType )
{
	switch ( sourceType )
	{
	case TYPE_STATIC:		return "TYPE_STATIC";		break;
	case TYPE_STREAMING:	return "TYPE_STREAMING";		break;
	case TYPE_UNDETERMINED:	return "TYPE_UNDETERMINED";		break;
	}
	return "UNKNOWN SOURCE TYPE";
}
