#pragma once
#define SET_NUMBERHB(x) ((BYTE)((DWORD)(x)>>(DWORD)8))
#define SET_NUMBERLB(x) ((BYTE)((DWORD)(x)&0xFF))
#define SET_NUMBERHW(x) ((WORD)((DWORD)(x)>>(DWORD)16))
#define SET_NUMBERLW(x) ((WORD)((DWORD)(x)&0xFFFF))
#define SET_NUMBERHDW(x) ((DWORD)((QWORD)(x)>>(QWORD)32))
#define SET_NUMBERLDW(x) ((DWORD)((QWORD)(x)&0xFFFFFFFF))

#define MAKE_NUMBERW(x,y) ((WORD)(((BYTE)((y)&0xFF))|((BYTE)((x)&0xFF)<<8)))
#define MAKE_NUMBERDW(x,y) ((DWORD)(((WORD)((y)&0xFFFF))|((WORD)((x)&0xFFFF)<<16)))
#define MAKE_NUMBERQW(x,y) ((QWORD)(((DWORD)((y)&0xFFFFFFFF))|((DWORD)((x)&0xFFFFFFFF)<<32)))


struct NEW_PBMSG_HEAD
{
	void set(BYTE head, BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
	}

	void setE(BYTE head, BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
	}

	BYTE type;
	BYTE size;
	BYTE head;
};

struct NEW_PSBMSG_HEAD
{
	void set(BYTE head, BYTE subh, BYTE size) // OK
	{
		this->type = 0xC1;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head, BYTE subh, BYTE size) // OK
	{
		this->type = 0xC3;
		this->size = size;
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size;
	BYTE head;
	BYTE subh;
};

struct NEW_PWMSG_HEAD
{
	void set(BYTE head, WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	void setE(BYTE head, WORD size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
};

struct NEW_PSWMSG_HEAD
{
	void set(BYTE head, BYTE subh, WORD size) // OK
	{
		this->type = 0xC2;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	void setE(BYTE head, BYTE subh, WORD size) // OK
	{
		this->type = 0xC4;
		this->size[0] = SET_NUMBERHB(size);
		this->size[1] = SET_NUMBERLB(size);
		this->head = head;
		this->subh = subh;
	}

	BYTE type;
	BYTE size[2];
	BYTE head;
	BYTE subh;
};

struct PMSG_CUSTOM_PREVIEW_CHAR_LIST_RECV
{
	NEW_PSWMSG_HEAD header; // C1:F3:00
	BYTE count;
};

struct PMSG_CUSTOM_PREVIEW_CHAR_LIST
{
	char Name[10];
	WORD PetIndex;
	WORD WingIndex;
};

struct PMSG_CUSTOM_PREVIEW_CHARSET_RECV
{
	NEW_PSWMSG_HEAD header; // C1:F3:00
	BYTE count;
	BYTE Viewport;
};

struct PMSG_CUSTOM_PREVIEW_CHARSET_LIST
{
	char Name[11];
	WORD PetIndex;
	WORD SecondPetIndex;
	WORD WingIndex;
	WORD index;
	WORD ElementSlot[2];
};

struct PMSG_ANTI_CLIENT_KEY_SEND
{
	NEW_PSBMSG_HEAD header;
	char m_MacAddress[40];
	BYTE lang;
};

//**********************************************//
//*********** GameServer -> Client  ************//
//**********************************************//
struct SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_RECV
{
	NEW_PSBMSG_HEAD header; //
	int state;
	int selectedKey;
	int selectedComb;
};

struct PMSG_CHAOS_BOX_GENESIS_LIST_RECV
{
	NEW_PSWMSG_HEAD header; //
	BYTE count;
};

struct PMSG_CHAOS_BOX_GENESIS_ITEM_INFO
{
	BYTE slot;
	BYTE ItemInfo[12];
};

struct PMSG_CHAOS_BOX_GENESIS_STATE_RECV
{
	NEW_PSBMSG_HEAD header; //
	int state;
};

struct PMSG_ITEM_COUNTER_RECV
{
	NEW_PSWMSG_HEAD h;
	BYTE count;
	BYTE Type;
};

struct PMSG_ITEM_COUNTER_INFO
{
	WORD ItemIndex;
	BYTE Slot;
	BYTE Top;
	DWORD kills;
};

//**********************************************//
//*********** Client -> GameServer  ************//
//**********************************************//
struct SDHP_CHAOS_BOX_GENESIS_SET_INFO_SEND
{
	NEW_PSBMSG_HEAD header; //
	int state;
	int selectedKey;
	int selectedComb;
};

struct SDHP_CHAOS_BOX_GENESIS_MIX_SEND
{
	NEW_PSBMSG_HEAD header; //
	int state;
};


void GCCustomPreviewCharListRecv(PMSG_CUSTOM_PREVIEW_CHAR_LIST_RECV* lpMsg);
void GCCustomPreviewCharRecv(PMSG_CUSTOM_PREVIEW_CHARSET_RECV* lpMsg);