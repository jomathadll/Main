#pragma once

#include "ProtocolSend.h"
#include "Lua.h"
#include "LuaBMD.h"
#include "LuaGlobal.h"

struct PMSG_VIEWPORT_RECV
{
	NEW_PSWMSG_HEAD header; // C2:[12:13:1F:20:45:65:68]
	BYTE count;
};

struct PMSG_VIEWPORT_PATENT
{
	BYTE index[2];
	DWORD patent;
	BYTE type;
};

class PATENTE_PLAYER_INFO
{
public:
	PATENTE_PLAYER_INFO()
	{
		this->aIndex = -1;
		this->Patente = 0;
		this->Type = 0;
	};
	~PATENTE_PLAYER_INFO()
	{

	};

	WORD aIndex;
	DWORD Patente;
	BYTE Type;
};

class PATENT_MODEL_INFO
{
public:
	PATENT_MODEL_INFO()
	{
		this->Index = 0;
		this->ModelIndex = 0;
		this->Bone = 0;
		this->Type = 0;
		this->Position[0] = 0;
		this->Position[1] = 0;
		this->Position[2] = 0;
		this->Angle[0] = 0;
		this->Angle[1] = 0;
		this->Angle[2] = 0;
		this->BillBoard = true;
		memset(this->Dir, 0, sizeof(this->Dir));
		memset(this->ModelName, 0, sizeof(this->ModelName));
	};

	~PATENT_MODEL_INFO()
	{

	};

	int Index;
	int ModelIndex;
	char ModelName[32];
	char Dir[256];
	int Bone;
	float Position[3];
	float Angle[3];
	bool Type;
	bool BillBoard;
};

class PATENT_IMAGE_INFO
{
public:
	PATENT_IMAGE_INFO()
	{
		this->Index = 0;
		this->ImageIndex = 0;
		this->ImageType = 0;
		this->Transparent = 0;
		this->Bone = 0;
		this->Position[0] = 0;
		this->Position[1] = 0;
		this->Position[2] = 0;
		this->Width = 0;
		this->Height = 0;
		memset(this->Dir, 0, sizeof(this->Dir));
	}

	~PATENT_IMAGE_INFO()
	{

	};

	int Index;
	int ImageIndex;
	int ImageType;
	int Transparent;
	int Bone;
	float Position[3];
	char Dir[256];
	float Width;
	float Height;
};

class PATENT_IMAGE_ANIMATION_INFO
{
public:
	PATENT_IMAGE_ANIMATION_INFO()
	{
		this->Index = 0;
		this->ImageIndex = -1;
		this->ImageIndexChange = -1;
		this->ImageType = 0;
		this->Transparent = 0;
		this->Bone = 0;
		this->Position[0] = 0;
		this->Position[1] = 0;
		this->Position[2] = 0;
		this->Width = 0;
		this->Height = 0;
		this->FramesH = 0;
		this->FramesV = 0;
		this->WaitTime = 0;
		this->FrameSpeed = 0;
		this->ImageChangeCount = 0;
		this->Reverse = false;
		memset(this->Dir, 0, sizeof(this->Dir));
		memset(this->DirChange, 0, sizeof(this->DirChange));
	}

	~PATENT_IMAGE_ANIMATION_INFO()
	{

	};

	int Index;
	int ImageIndex;
	int ImageIndexChange;
	int ImageType;
	int Transparent;
	int Bone;
	float Position[3];
	char Dir[256];
	char DirChange[256];
	float Width;
	float Height;
	int FramesH;
	int FramesV;
	int WaitTime;
	float FrameSpeed;
	int ImageChangeCount;
	bool Reverse;
};


class CPatente {
public:
	CPatente();
	~CPatente();

	void Init();
	void Clear();
	void RestartLua();
	void LoadImages();
	void GCPatentePlayerRecv(PMSG_VIEWPORT_RECV* lpMsg);
	bool IsPlayer(int aindex, int patent, BYTE type);
	void DeletePlayer(int aIndex);
	int GetPatentIndex(int ModelIndex);
	PATENTE_PLAYER_INFO* GetPlayer(int aIndex);
	PATENT_MODEL_INFO* GetPatentModelInfo(int index);
	PATENT_IMAGE_INFO* GetPatentImageInfo(int index);
	PATENT_IMAGE_INFO* GetPatentImageInfoByImageIndex(int index);
	PATENT_IMAGE_ANIMATION_INFO* GetPatentImageAnimationInfo(int index);
	PATENT_IMAGE_ANIMATION_INFO* GetPatentImageAnimationInfoByImageIndex(int index);
	void PatentModelRender(DWORD BMDStruct, DWORD ObjectStruct, DWORD ItemIndex);
	void GMSignRender(DWORD BMDStruct, DWORD ObjectStruct);

public:
	std::map<int, PATENT_MODEL_INFO> m_PatentModelInfo;
	std::map<int, PATENT_IMAGE_INFO> m_PatentImageInfo;
	std::map<int, PATENT_IMAGE_ANIMATION_INFO> m_PatentImageAnimationInfo;
	int m_PatenteImageCount;

private:
	Lua m_Lua;
	std::vector<PATENTE_PLAYER_INFO> m_PlayerInfo;
	LuaBMD m_LuaBMD;
	LuaGlobal m_LuaGlobal;

};

extern CPatente gPatente;