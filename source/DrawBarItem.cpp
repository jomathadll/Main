#include "stdafx.h"
#include "DrawBarItem.h"
#include "LuaStack.hpp"
#include "ZzzInventory.h"

using namespace luaaa;

CDrawBarItem gDrawBarItem;

CDrawBarItem::CDrawBarItem() : m_Lua()
{
	this->m_Item.ItemID = -1;
	this->m_ItemBorderColor.ItemID = -1;
	this->m_ItemBorderColorList.clear();
}

CDrawBarItem::~CDrawBarItem() 
{

}

void SetItemBorderColor(int ItemIndex, int ItemLevel, float R, float G, float B, float A) {
	ITEM_BORDERCOLOR info;

	info.ItemID = ItemIndex;

	info.Level = ItemLevel;

	info.R = R;

	info.G = G;

	info.B = B;

	info.A = A;

	gDrawBarItem.m_ItemBorderColorList.push_back(info);
}

void CDrawBarItem::Init() {
	LuaModule(this->m_Lua.GetState()).fun("SetItemBorderColor", &SetItemBorderColor);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomItemBorderColor.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartItemBorderColor", "");
}

void CDrawBarItem::RestartLua() {
	this->m_ItemBorderColorList.clear();

	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	LuaModule(this->m_Lua.GetState()).fun("SetItemBorderColor", &SetItemBorderColor);

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Configs\\CustomItemBorderColor.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	this->m_Lua.Generic_Call("StartItemBorderColor", "");
}

bool CDrawBarItem::GetItemBorderColor(int ItemID, int Level) {
	for (std::vector<ITEM_BORDERCOLOR>::iterator it = this->m_ItemBorderColorList.begin(); it != this->m_ItemBorderColorList.end(); it++)
	{
		if (it->ItemID == ItemID && (it->Level == -1 || it->Level == Level))
		{
			this->m_ItemBorderColor.ItemID = it->ItemID;
			this->m_ItemBorderColor.Level = it->Level;
			this->m_ItemBorderColor.R = it->R;
			this->m_ItemBorderColor.G = it->G;
			this->m_ItemBorderColor.B = it->B;
			this->m_ItemBorderColor.A = it->A;
			return 1;
		}
	}

	this->m_ItemBorderColor.ItemID = -1;

	return 0;
}

void CDrawBarItem::DrawItem()
{
	EndBitmap();
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glViewport2(0, 0, WindowWidth, WindowHeight);
	gluPerspective2(this->m_Item.Size, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();
	RenderItem3D(this->m_Item.PosX, this->m_Item.PosY, this->m_Item.Width, this->m_Item.Height, this->m_Item.ItemID, this->m_Item.Level, this->m_Item.Excl, this->m_Item.Anc, 0);
	UpdateMousePositionn();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	BeginBitmap();
}

void CDrawBarItem::SetItemDrawBar(int Source, int ItemID, int Level, int Excl, int Anc, float Size)
{
	this->m_Item.Source = Source;
	this->m_Item.ItemID = ItemID;
	this->m_Item.Level = Level;
	this->m_Item.Excl = Excl;
	this->m_Item.Anc = Anc;
	this->m_Item.Size = Size;
}

void CDrawBarItem::ResetItemDrawBar(int Source) {
	if (Source == this->m_Item.Source && this->m_Item.ItemID != -1) {
		this->m_Item.ItemID = -1;
	}
}