#pragma once

struct PMSG_CHARACTER_HUNTING_INFO_RECV
{
	NEW_PSBMSG_HEAD header;	// C1:0B
	WORD level;
	DWORD avarageExp;
	DWORD expPerMinute;
	DWORD etaLevelUp;
	DWORD lastExp;
	DWORD zenPerMinute;
	DWORD lastZen;
};

class Hunting {
public:
	Hunting();
	~Hunting();

	void ReceiveHunting(PMSG_CHARACTER_HUNTING_INFO_RECV* lpMsg);

	WORD level;

	DWORD avarageExp;
	DWORD expPerMinute;
	DWORD etaLevelUp;
	DWORD lastExp;
	DWORD zenPerMinute;
	DWORD lastZen;

};

extern Hunting gHunting;