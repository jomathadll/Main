#include "StdAfx.h"
#include "ProtocolSend.h"
#include "Hunting.h"
#include <sstream>
#include <filesystem>

Hunting gHunting;

Hunting::Hunting()
{
	avarageExp = 0;
	expPerMinute = 0;
	etaLevelUp = 0;
	lastExp = 0;
}

Hunting::~Hunting()
{
}

void Hunting::ReceiveHunting(PMSG_CHARACTER_HUNTING_INFO_RECV * lpMsg)
{
	level = lpMsg->level;
	avarageExp = lpMsg->avarageExp;
	expPerMinute = lpMsg->expPerMinute;
	etaLevelUp = lpMsg->etaLevelUp;
	lastExp = lpMsg->lastExp;
	zenPerMinute = lpMsg->zenPerMinute;
	lastZen = lpMsg->lastZen;
}