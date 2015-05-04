#include "MoveRequest.h"

#include <boost/bind.hpp>

#include "MessageTypes.h"

////////////////////////////////////////////////////////////////////////////////
//
// MoveRequest
//
////////////////////////////////////////////////////////////////////////////////

MoveRequest::MoveRequest(GameNetworkInterface *gameNetworkInterface)
	: Message(gameNetworkInterface)
{
	m_networkData.angle = 0;
	m_networkData.strength = 0;

	// Set up for network transmission via messages
	registerMessageType(MESSAGE_MOVE_REQUEST, &m_networkData,
		sizeof(NetworkData), UPDATE_FREQUENCY_ONCE);

	setMessageID(MESSAGE_ID_EVENT);
}

////////////////////////////////////////////////////////////////////////////////

void MoveRequest::setUnitIndex(uint8_t unitIndex)
{
	m_networkData.unitIndex = unitIndex;
}

////////////////////////////////////////////////////////////////////////////////

uint8_t MoveRequest::unitIndex()
{
	return m_networkData.unitIndex;
}

////////////////////////////////////////////////////////////////////////////////

void MoveRequest::setAngle(float angle)
{
	m_networkData.angle = angle;
}

////////////////////////////////////////////////////////////////////////////////

float MoveRequest::angle()
{
	return m_networkData.angle;
}

////////////////////////////////////////////////////////////////////////////////

void MoveRequest::setStrength(float strength)
{
	m_networkData.strength = strength;
}

////////////////////////////////////////////////////////////////////////////////

float MoveRequest::strength()
{
	return m_networkData.strength;
}
