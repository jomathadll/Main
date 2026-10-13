#pragma once
#include "Lua.h"

struct ITEM_BORDERCOLOR
{
	int ItemID;
	int Level;
	float R;
	float G;
	float B;
	float A;
};

struct ITEM_DRAWBAR
{
	int Source;
	float PosX;
	float PosY;
	float Width;
	float Height;
	int ItemID;
	int Level;
	int Excl;
	int Anc;
	float Size;
};

class CDrawBarItem
{
public:
	CDrawBarItem();
	~CDrawBarItem();

	void Init();
	void RestartLua();
	void DrawItem();
	void SetItemDrawBar(int Source, int ItemID, int Level, int Excl, int Anc, float Size);
	void ResetItemDrawBar(int Source);
	bool GetItemBorderColor(int ItemID, int Level);
	
	ITEM_DRAWBAR m_Item;
	ITEM_BORDERCOLOR m_ItemBorderColor;
	std::vector<ITEM_BORDERCOLOR> m_ItemBorderColorList;

private:
	Lua m_Lua;
};

extern CDrawBarItem gDrawBarItem;