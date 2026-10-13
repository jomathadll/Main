#include "stdafx.h"
#include "Patente.h"
#include "LuaStack.hpp"
#include "LuaObject.h"
#include "ZzzTexture.h"
#include "ZzzCharacter.h"
#include "CSParts.h"

using namespace luaaa;

CPatente gPatente;

CPatente::CPatente() : m_Lua()
{
	this->m_PatentImageInfo.clear();
	this->m_PatentImageAnimationInfo.clear();
	this->m_PatentModelInfo.clear();
	this->m_PlayerInfo.clear();
	this->m_PatenteImageCount = BITMAP_PATENTE_START;
}

CPatente::~CPatente()
{
	this->m_PatentImageInfo.clear();
	this->m_PatentImageAnimationInfo.clear();
	this->m_PatentModelInfo.clear();
	this->m_PlayerInfo.clear();
	this->m_PatenteImageCount = BITMAP_PATENTE_START;
}

void LoadPatenteModel(int Index, char* Dir, char* ModelName, int modelType, int bone, float posx, float posy, float posz, float anglex, float angley, float anglez, bool BillBoard)
{
	PATENT_MODEL_INFO info;

	info.Index = Index;
	info.ModelIndex = -1;
	info.Bone = bone;
	info.Position[0] = posx;
	info.Position[1] = posy;
	info.Position[2] = posz;
	info.Angle[0] = anglex;
	info.Angle[1] = angley;
	info.Angle[2] = anglez;
	info.Type = modelType;
	info.BillBoard = BillBoard;

	strcpy_s(info.ModelName, ModelName);

	strcpy_s(info.Dir, Dir);

	gPatente.m_PatentModelInfo.insert(std::make_pair(info.Index, info));
}

void LoadPatenteImage(int Index, int transparent, char* dir, int imageType, int bone, float posx, float posy, float posz, float width, float height)
{
	PATENT_IMAGE_INFO info;
	info.Index = Index;
	info.Transparent = transparent;
	info.Bone = bone;
	info.Position[0] = posx;
	info.Position[1] = posy;
	info.Position[2] = posz;
	info.ImageType = imageType;
	strcpy_s(info.Dir, dir);
	info.ImageIndex = gPatente.m_PatenteImageCount;
	info.Width = width;
	info.Height = height;

	gPatente.m_PatenteImageCount++;

	gPatente.m_PatentImageInfo.insert(std::make_pair(info.Index, info));
}

void LoadPatenteImageAnimation(int Index, int transparent, char* dir, char* dirChange, int imageType, int bone, float posx, float posy, float posz, float width, float height, 
	int FramesH, int FramesV, int WaitTime, float FrameSpeed, int imageChange, bool Reverse)
{
	PATENT_IMAGE_ANIMATION_INFO info;
	info.Index = Index;
	info.Transparent = transparent;
	info.Bone = bone;
	info.Position[0] = posx;
	info.Position[1] = posy;
	info.Position[2] = posz;
	info.ImageType = imageType;
	strcpy_s(info.Dir, dir);
	info.ImageIndex = gPatente.m_PatenteImageCount;
	if (dirChange[0] != '\0') {
		gPatente.m_PatenteImageCount++;
		strcpy_s(info.DirChange, dirChange);
		info.ImageIndexChange = gPatente.m_PatenteImageCount;
	}
	gPatente.m_PatenteImageCount++;
	info.Width = width;
	info.Height = height;
	info.FramesH = FramesH;
	info.FramesV = FramesV;
	info.WaitTime = WaitTime;
	info.FrameSpeed = FrameSpeed;
	info.ImageChangeCount = imageChange;
	info.Reverse = Reverse;

	gPatente.m_PatentImageAnimationInfo.insert(std::make_pair(info.Index, info));
}

void LoadPatenteModelReload(int Index, char* Dir, char* ModelName, int modelType, int bone, float posx, float posy, float posz, float anglex, float angley, float anglez, bool BillBoard)
{
	gPatente.m_PatentModelInfo[Index].Bone = bone;
	gPatente.m_PatentModelInfo[Index].Position[0] = posx;
	gPatente.m_PatentModelInfo[Index].Position[1] = posy;
	gPatente.m_PatentModelInfo[Index].Position[2] = posz;
	gPatente.m_PatentModelInfo[Index].Angle[0] = anglex;
	gPatente.m_PatentModelInfo[Index].Angle[1] = angley;
	gPatente.m_PatentModelInfo[Index].Angle[2] = anglez;
	gPatente.m_PatentModelInfo[Index].BillBoard = BillBoard;
}

void LoadPatenteImageReload(int Index, int transparent, char* dir, int imageType, int bone, float posx, float posy, float posz, float width, float height)
{
	gPatente.m_PatentImageInfo[Index].Transparent = transparent;
	gPatente.m_PatentImageInfo[Index].Bone = bone;
	gPatente.m_PatentImageInfo[Index].Position[0] = posx;
	gPatente.m_PatentImageInfo[Index].Position[1] = posy;
	gPatente.m_PatentImageInfo[Index].Position[2] = posz;
	gPatente.m_PatentImageInfo[Index].Width = width;
	gPatente.m_PatentImageInfo[Index].Height = height;
}

void LoadPatenteImageAnimationReload(int Index, int transparent, char* dir, char* dirChange, int imageType, int bone, float posx, float posy, float posz, float width, float height, int FramesH, int FramesV, int WaitTime, int FrameSpeed, int imageChange, bool Reverse)
{
	gPatente.m_PatentImageAnimationInfo[Index].Transparent = transparent;
	gPatente.m_PatentImageAnimationInfo[Index].Bone = bone;
	gPatente.m_PatentImageAnimationInfo[Index].Position[0] = posx;
	gPatente.m_PatentImageAnimationInfo[Index].Position[1] = posy;
	gPatente.m_PatentImageAnimationInfo[Index].Position[2] = posz;
	gPatente.m_PatentImageAnimationInfo[Index].Width = width;
	gPatente.m_PatentImageAnimationInfo[Index].Height = height;
	gPatente.m_PatentImageAnimationInfo[Index].FramesH = FramesH;
	gPatente.m_PatentImageAnimationInfo[Index].FramesV = FramesV;
	gPatente.m_PatentImageAnimationInfo[Index].WaitTime = WaitTime;
	gPatente.m_PatentImageAnimationInfo[Index].FrameSpeed = FrameSpeed;
	gPatente.m_PatentImageAnimationInfo[Index].ImageChangeCount = imageChange;
	gPatente.m_PatentImageAnimationInfo[Index].Reverse = Reverse;
}

void CPatente::RestartLua() {

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	LuaModule(this->m_Lua.GetState()).fun("LoadPatenteModel", &LoadPatenteModelReload);

	LuaModule(this->m_Lua.GetState()).fun("LoadPatenteImage", &LoadPatenteImageReload);

	LuaModule(this->m_Lua.GetState()).fun("LoadPatenteImageAnimation", &LoadPatenteImageAnimationReload);

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());
	
	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\PatentSystem.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadPatentes", "");
}

void CPatente::Init()
{
	LuaModule(this->m_Lua.GetState()).fun("LoadPatenteModel", &LoadPatenteModel);

	LuaModule(this->m_Lua.GetState()).fun("LoadPatenteImage", &LoadPatenteImage);

	LuaModule(this->m_Lua.GetState()).fun("LoadPatenteImageAnimation", &LoadPatenteImageAnimation);

	this->m_LuaBMD.RegisterClassBMD(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	RegisterClassObject(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\CharacterSystem\\PatentSystem.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartLoadPatentes", "");
}

void CPatente::LoadImages()
{
	for (std::map<int, PATENT_IMAGE_INFO>::iterator it = this->m_PatentImageInfo.begin(); it != this->m_PatentImageInfo.end(); it++)

	if (it != this->m_PatentImageInfo.end()) {
		if (it->second.ImageType == 1)
		{
			LoadBitmapCustom(it->second.Dir, it->second.ImageIndex, GL_LINEAR, GL_REPEAT);
		}
		else
		{
			LoadBitmap(it->second.Dir, it->second.ImageIndex, GL_LINEAR, GL_REPEAT);
		}
	}

	for (std::map<int, PATENT_IMAGE_ANIMATION_INFO>::iterator it = this->m_PatentImageAnimationInfo.begin(); it != this->m_PatentImageAnimationInfo.end(); it++)

	if (it != this->m_PatentImageAnimationInfo.end()) {
		if (it->second.ImageType == 1)
		{
			LoadBitmapCustom(it->second.Dir, it->second.ImageIndex, GL_LINEAR, GL_REPEAT);

			if (it->second.ImageIndexChange != -1) {
				LoadBitmapCustom(it->second.DirChange, it->second.ImageIndexChange, GL_LINEAR, GL_REPEAT);
			}
		}
		else
		{
			LoadBitmap(it->second.Dir, it->second.ImageIndex, GL_LINEAR, GL_REPEAT);
			if (it->second.ImageIndexChange != -1) {
				LoadBitmap(it->second.DirChange, it->second.ImageIndexChange, GL_LINEAR, GL_REPEAT);
			}
		}
	}
}

int CPatente::GetPatentIndex(int ModelIndex)
{
	for (std::map<int, PATENT_MODEL_INFO>::iterator it = this->m_PatentModelInfo.begin(); it != this->m_PatentModelInfo.end(); it++)
	{
		auto lpInfo = &(*it).second;

		if (lpInfo->ModelIndex == ModelIndex) {
			return lpInfo->Index;
		}
	}

	return 0;
}

void CPatente::Clear()
{
	this->m_PlayerInfo.clear();
}

void CPatente::DeletePlayer(int aIndex)
{
	std::vector<PATENTE_PLAYER_INFO>::iterator it = this->m_PlayerInfo.begin();

	for (; it != this->m_PlayerInfo.end(); it++)
	{
		if (it->aIndex == aIndex)
		{
			this->m_PlayerInfo.erase(it);
			return;
		}
	}
}

void CPatente::GCPatentePlayerRecv(PMSG_VIEWPORT_RECV* lpMsg)
{
	if (lpMsg->count == 1)
	{
		auto lpInfo = (PMSG_VIEWPORT_PATENT*)(((BYTE*)lpMsg) + sizeof(PMSG_VIEWPORT_RECV) + (sizeof(PMSG_VIEWPORT_PATENT) * 0));

		int aIndex = MAKE_NUMBERW(lpInfo->index[0], lpInfo->index[1]);

		if (this->IsPlayer(aIndex, lpInfo->patent, lpInfo->type) != 0) {
			return;
		}

		PATENTE_PLAYER_INFO info;

		info.aIndex = aIndex;

		info.Patente = lpInfo->patent;

		info.Type = lpInfo->type;

		this->m_PlayerInfo.push_back(info);
	}
	else
	{
		for (int n = 0; n < lpMsg->count; n++)
		{
			auto lpInfo = (PMSG_VIEWPORT_PATENT*)(((BYTE*)lpMsg) + sizeof(PMSG_VIEWPORT_RECV) + (sizeof(PMSG_VIEWPORT_PATENT) * n));

			int aIndex = MAKE_NUMBERW(lpInfo->index[0], lpInfo->index[1]);

			if (this->IsPlayer(aIndex, lpInfo->patent, lpInfo->type) != 0) {
				return;
			}

			PATENTE_PLAYER_INFO info;

			info.aIndex = aIndex;

			info.Patente = lpInfo->patent;

			info.Type = lpInfo->type;

			this->m_PlayerInfo.push_back(info);
		}
	}
}

bool CPatente::IsPlayer(int aindex, int patent, BYTE type)
{
	for (auto& Player : this->m_PlayerInfo)
	{
		if (Player.aIndex == aindex)
		{
			Player.Patente = patent;
			Player.Type = type;
			return 1;
		}
	}

	return 0;
}

PATENTE_PLAYER_INFO* CPatente::GetPlayer(int aIndex)
{
	for (auto& Player : this->m_PlayerInfo)
	{
		if (Player.aIndex == aIndex)
		{
			return &Player;
		}
	}

	return 0;
}

PATENT_MODEL_INFO* CPatente::GetPatentModelInfo(int index)
{
	auto it = this->m_PatentModelInfo.find(index);

	if (it == this->m_PatentModelInfo.end()) {
		return 0;
	}

	return &it->second;
}

PATENT_IMAGE_INFO* CPatente::GetPatentImageInfo(int index)
{
	auto it = this->m_PatentImageInfo.find(index);

	if (it == this->m_PatentImageInfo.end()) {
		return 0;
	}

	return &it->second;
}

PATENT_IMAGE_INFO* CPatente::GetPatentImageInfoByImageIndex(int index)
{
	for (auto& Patente : this->m_PatentImageInfo)
	{
		if (Patente.second.ImageIndex == index)
		{
			return &Patente.second;
		}
	}
	
	return 0;
}

PATENT_IMAGE_ANIMATION_INFO* CPatente::GetPatentImageAnimationInfo(int index)
{
	auto it = this->m_PatentImageAnimationInfo.find(index);

	if (it == this->m_PatentImageAnimationInfo.end()) {
		return 0;
	}

	return &it->second;
}

PATENT_IMAGE_ANIMATION_INFO* CPatente::GetPatentImageAnimationInfoByImageIndex(int index)
{

	for (auto& Patente : this->m_PatentImageAnimationInfo)
	{
		if (Patente.second.ImageIndex == index)
		{
			return &Patente.second;
		}
	}

	return 0;
}


void CPatente::PatentModelRender(DWORD BMDStruct, DWORD ObjectStruct, DWORD ItemIndex)
{
	if (BMDStruct == 0 || ObjectStruct == 0)
	{
		return;
	}

	this->m_Lua.Generic_Call("PatentModelRender", "jji>", BMDStruct, ObjectStruct, this->GetPatentIndex(ItemIndex));
}

void CPatente::GMSignRender(DWORD BMDStruct, DWORD ObjectStruct)
{
	if (BMDStruct == 0 || ObjectStruct == 0)
	{
		return;
	}

	this->m_Lua.Generic_Call("GMSignRender", "jj>", BMDStruct, ObjectStruct);
}