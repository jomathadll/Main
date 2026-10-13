#pragma once

#include "Lua.h"
#include "LuaBMD.h"
#include "LuaUser.h"
#include "LuaInterface.h"

class CRenderModel {
public:
	CRenderModel();
	~CRenderModel();

	void Init();
	void RestartLua();

	int RenderModelBodyLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct);
	int RenderModelBodyColorLua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct);
	int RenderModelBodyColor2Lua(int ItemIndex, BMD* BMDStruct, OBJECT* ObjectStruct);
	void LoadImageRenderModel();

public:
	Lua m_Lua;

private:
	LuaBMD m_LuaBMD;
	CLuaUser m_LuaUser;
	LuaInterface m_LuaInterface;
};

extern CRenderModel gRenderModel;