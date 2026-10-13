#pragma once

#include "Lua.h"
#include <vector>

struct MESH_GLOW
{
	int ItemIndex;
	int Mesh;
};

class CCustomMeshGlow
{
public:
	CCustomMeshGlow();
	~CCustomMeshGlow();

	void Init();
	void RestartLua();

private:
	Lua m_Lua;

public:
	std::vector<MESH_GLOW> m_MeshList;
};

extern CCustomMeshGlow gCustomMeshGlow;