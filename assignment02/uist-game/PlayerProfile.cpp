#include "PlayerProfile.h"

#include "NetworkServerSession.h"

////////////////////////////////////////////////////////////////////////////////
//
// PlayerProfile
//
////////////////////////////////////////////////////////////////////////////////

void PlayerProfile::setPlayerID(PlayerID playerID)
{
	m_playerID = playerID;
}

////////////////////////////////////////////////////////////////////////////////

PlayerID PlayerProfile::playerID() const
{
	return m_playerID;
}

////////////////////////////////////////////////////////////////////////////////

void PlayerProfile::setSession(NetworkServerSession *session)
{
	m_session = session;
}

////////////////////////////////////////////////////////////////////////////////

NetworkServerSession *PlayerProfile::session() const
{
	return m_session;
}
