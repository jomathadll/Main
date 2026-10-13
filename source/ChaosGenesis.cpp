// NewUIMixInventory.cpp: implementation of the CChaosGenesis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChaosGenesis.h"
#include "NewUISystem.h"
#include "NewUICustomMessageBox.h"
#include "MixMgr.h"
#include "ZzzBMD.h"
#include "ZzzEffect.h"
#include "ZzzObject.h"
#include "ZzzInventory.h"
#include "ZzzInterface.h"
#include "ZzzInfomation.h"
#include "ZzzCharacter.h"
#include "wsclientinline.h"
#include "DSPlaySound.h"
#include "SocketSystem.h"
#include "LuaStack.hpp"
#include "VisualDescriptions.h"
#include "DrawBarItem.h"

using namespace SEASON3B;

class ChaosMachineGenesisLua {
public:
	ChaosMachineGenesisLua() : index(-1) {

	};

	ChaosMachineGenesisLua(int index) {
		this->index = index;
	};

	~ChaosMachineGenesisLua() {

	};

	int getIndex() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return -1;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Type;
	};

	int getLevel() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return (g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level / 8) & 0xF;
	};

	int getOption1() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Option1;
	};

	int getExtOption() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->ExtOption;
	};

	int getHarmony() const { //OK
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		if (g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Jewel_Of_Harmony_OptionLevel > 0 || g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Jewel_Of_Harmony_Option > 0) {
			return 1;
		}

		return 0;
	}

	int getLuck() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return ((g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level >> 2) & 1);
	};

	int getSkill() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return ((g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level >> 7) & 1);
	};

	int getLife() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		int oplevel = g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Level;
		int option1 = g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Option1;
		int Option3 = ((oplevel) & 3) + ((option1) & 64) / 64 * 4;

		return Option3;
	};

	int getDurability() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		return g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Durability;
	};

	int getStack() const {
		if (this->index == -1 || this->index > (int)g_pChaosGenesis->m_ChaosMachineItens.size()) {
			return 0;
		}

		if (gJewelStack.CheckIsStack(g_pChaosGenesis->m_ChaosMachineItens[this->index].pItemObj->Type)) {
			return 1;
		}
		return 0;
	};

private:
	int index;
};

void RenderItensChaosMachineLua(float x, float y, float height, int MaxLines) {
	g_pChaosGenesis->RenderItensChaosMachine(x, y, height, MaxLines);
}

int getCountItensByIndexLua(int index, int level, int exc, int luck, int skill, int life, int ancient, int MaxAncient, int visual) {
	return g_pChaosGenesis->getCountItensByIndex(index, level, exc, luck, skill, life, ancient, MaxAncient, visual);
}

int getCountItensByIndexLevelLua(int index, int level, int exc, int luck, int skill, int life) {
	return g_pChaosGenesis->getCountItensByIndexLevel(index, level, exc, luck, skill, life);
}

int getChaosBoxGenesisState() {
	return g_pChaosGenesis->ChaosBoxGenesisState;
}

int getChaosBoxGenesisSelectedKey() {
	return g_pChaosGenesis->ChaosBoxGenesisSelectedKey;
}

int getChaosBoxGenesisSelectedComb() {
	return g_pChaosGenesis->ChaosBoxGenesisSelectedComb;
}

void SetChaosBoxGenesis(int selectedKey, int selectedComb) {
	g_pChaosGenesis->ChaosBoxGenesisSelectedKey = selectedKey;
	g_pChaosGenesis->ChaosBoxGenesisSelectedComb = selectedComb;

	SDHP_CHAOS_BOX_GENESIS_SET_INFO_SEND pMsg;

	pMsg.header.set(0xEF, 0x20, sizeof(pMsg));

	pMsg.state = 0;

	pMsg.selectedKey = selectedKey;

	pMsg.selectedComb = selectedComb;

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

int getMaxItens() {
	return (int)g_pChaosGenesis->m_ChaosMachineItens.size();
}

void SendChaosMachineMix() {
	SDHP_CHAOS_BOX_GENESIS_MIX_SEND pMsg;

	pMsg.header.set(0xEF, 0x22, sizeof(pMsg));

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

CChaosGenesis::CChaosGenesis() : m_Lua()
{
	m_pNewUIMng = NULL;

	m_Pos.x = m_Pos.y = 0;

	this->m_nBackupMouseX = 0;

	this->m_nBackupMouseY = 0;

	this->m_TargetSlotBackup = 0;

	this->m_bItemAutoMove = false;

	this->m_iMouseY = 0;

	this->ChaosBoxGenesisState = 0;

	this->m_MaxLines = 9;

	this->m_HeightBox = 70;

	this->ItemPointer = -1;

	this->m_ChaosMachineItens.clear();
}

CChaosGenesis::~CChaosGenesis() { Release(); }

bool CChaosGenesis::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if(NULL == pNewUIMng || NULL == g_pNewUI3DRenderMng || NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_CHAOSGENESIS, this);

	SetPos(x, y);

	LoadImages();

	Show(false);


	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Manager\\ChaosMachine\\ChaosMachine.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("RenderItensChaosMachine", &RenderItensChaosMachineLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndex", &getCountItensByIndexLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndexLevel", &getCountItensByIndexLevelLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisState", &getChaosBoxGenesisState);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedKey", &getChaosBoxGenesisSelectedKey);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedComb", &getChaosBoxGenesisSelectedComb);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SetChaosBoxGenesis", &SetChaosBoxGenesis);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getMaxItens", &getMaxItens);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SendChaosMachineMix", &SendChaosMachineMix);

	luaaa::LuaClass<ChaosMachineGenesisLua> luaChaosMachine(this->m_Lua.GetState(), "ChaosMachine");
	luaChaosMachine.ctor<int>();
	luaChaosMachine.fun("getIndex", &ChaosMachineGenesisLua::getIndex);
	luaChaosMachine.fun("getLevel", &ChaosMachineGenesisLua::getLevel);
	luaChaosMachine.fun("getOption1", &ChaosMachineGenesisLua::getOption1);
	luaChaosMachine.fun("getExtOption", &ChaosMachineGenesisLua::getExtOption);
	luaChaosMachine.fun("getLuck", &ChaosMachineGenesisLua::getLuck);
	luaChaosMachine.fun("getSkill", &ChaosMachineGenesisLua::getSkill);
	luaChaosMachine.fun("getLife", &ChaosMachineGenesisLua::getLife);
	luaChaosMachine.fun("getDurability", &ChaosMachineGenesisLua::getDurability);
	luaChaosMachine.fun("getStack", &ChaosMachineGenesisLua::getStack);
	luaChaosMachine.fun("getHarmony", &ChaosMachineGenesisLua::getHarmony);

	return true;
}

void CChaosGenesis::RestartLua() {
	this->m_Lua.CloseLua();

	this->m_Lua.RegisterLua();

	this->m_LuaInterface.RegisterInterface(this->m_Lua.GetState());

	this->m_LuaGlobal.RegisterGlobal(this->m_Lua.GetState());

	char name[MAX_PATH] = { 0 };
	sprintf_s(name, "%s\\Manager\\ChaosMachine\\ChaosMachine.lua", LUA_LOAD_PATH);

	this->m_Lua.DoFile(name);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("RenderItensChaosMachine", &RenderItensChaosMachineLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndex", &getCountItensByIndexLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getCountItensByIndexLevel", &getCountItensByIndexLevelLua);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisState", &getChaosBoxGenesisState);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedKey", &getChaosBoxGenesisSelectedKey);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getChaosBoxGenesisSelectedComb", &getChaosBoxGenesisSelectedComb);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SetChaosBoxGenesis", &SetChaosBoxGenesis);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("getMaxItens", &getMaxItens);

	luaaa::LuaModule(this->m_Lua.GetState()).fun("SendChaosMachineMix", &SendChaosMachineMix);

	luaaa::LuaClass<ChaosMachineGenesisLua> luaChaosMachine(this->m_Lua.GetState(), "ChaosMachine");
	luaChaosMachine.ctor<int>();
	luaChaosMachine.fun("getIndex", &ChaosMachineGenesisLua::getIndex);
	luaChaosMachine.fun("getLevel", &ChaosMachineGenesisLua::getLevel);
	luaChaosMachine.fun("getOption1", &ChaosMachineGenesisLua::getOption1);
	luaChaosMachine.fun("getExtOption", &ChaosMachineGenesisLua::getExtOption);
	luaChaosMachine.fun("getLuck", &ChaosMachineGenesisLua::getLuck);
	luaChaosMachine.fun("getSkill", &ChaosMachineGenesisLua::getSkill);
	luaChaosMachine.fun("getLife", &ChaosMachineGenesisLua::getLife);
	luaChaosMachine.fun("getDurability", &ChaosMachineGenesisLua::getDurability);
	luaChaosMachine.fun("getStack", &ChaosMachineGenesisLua::getStack);
	luaChaosMachine.fun("getHarmony", &ChaosMachineGenesisLua::getHarmony);
}

void CChaosGenesis::Release()
{
	UnloadImages();

	if(m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}
}

void CChaosGenesis::Open(SDHP_CHAOS_BOX_GENESIS_OPEN_CLIENT_RECV* lpMsg)
{
	m_EventState = EVENT_NONE;

	g_pChaosGenesis->ChaosBoxGenesisState = lpMsg->state;

	g_pChaosGenesis->ChaosBoxGenesisSelectedKey = lpMsg->selectedKey;

	g_pChaosGenesis->ChaosBoxGenesisSelectedComb = lpMsg->selectedComb;

	g_pNewUISystem->Show(SEASON3B::INTERFACE_CHAOSGENESIS);
}

void CChaosGenesis::ChaosBoxGenesisStateRecv(PMSG_CHAOS_BOX_GENESIS_STATE_RECV* lpMsg)
{
	this->ChaosBoxGenesisState = lpMsg->state;
	
	this->m_Lua.Generic_Call("ChaosMachineMixResult", "i>", this->ChaosBoxGenesisState);
}

void CChaosGenesis::ChaosBoxGenesisListItemRecv(PMSG_CHAOS_BOX_GENESIS_LIST_RECV* lpMsg)
{
	this->DeleteAllItems();


	if (lpMsg->count <= 0) {
		return;
	}

	for (int n = 0; n < lpMsg->count; n++) {
		auto lpInfo = (PMSG_CHAOS_BOX_GENESIS_ITEM_INFO*)(((BYTE*)lpMsg) + sizeof(PMSG_CHAOS_BOX_GENESIS_LIST_RECV) + (sizeof(PMSG_CHAOS_BOX_GENESIS_ITEM_INFO) * n));
		this->InsertItemChaosMachine(lpInfo->slot, lpInfo->ItemInfo);

	}

	this->CalcLinesRender();
}

void CChaosGenesis::NewDeleteItem(ITEM* item) {
	g_pNewItemMng->DeleteItem(item);
}

void CChaosGenesis::RenderItensChaosMachine(float x, float y, float height, int MaxLines)
{
	EnableAlphaTest(1);

	float SizeY = 0;

	char buffer[256];
	g_pRenderText->SetFont(g_hFont);
	g_pRenderText->SetBgColor(0, 0, 0, 0);
	g_pRenderText->SetTextColor(225, 225, 225, 255);

	int LineStart = 0;
	int LineCount = 0;

	if (this->m_ChaosMachineItens.size() <= 0) {
		gDrawBarItem.ResetItemDrawBar(1);
	}

	for (auto& itemTarget : this->m_ChaosMachineItens) {
		if (LineStart >= ScrollBarOffset && LineCount <= MaxLines) {
			if (!itemTarget.pItemObj) {
				continue;
			}

			if (MouseX >= x - 142 && MouseX <= x - 140 + 130 && MouseY >= y + SizeY && MouseY <= y + (SizeY + 8)) {
				this->ItemPointer = itemTarget.slot;

				RenderItemInformation(x - 142, y + 125, itemTarget.pItemObj);
			}
			else {
				gDrawBarItem.ResetItemDrawBar(1);
			}

			auto itemAttr = &ItemAttribute[itemTarget.pItemObj->Type];

			if (!itemAttr) {
				continue;
			}

			EnableAlphaTest(true);

			if (itemTarget.pItemObj->Jewel_Of_Harmony_Option > 0 || itemTarget.pItemObj->Jewel_Of_Harmony_OptionLevel > 0 || (itemAttr->m_byItemSlot >= 240 && itemAttr->m_byItemSlot <= 248)) {
				sprintf_s(buffer, sizeof(buffer), "%s", Visuals.GetItemHybridName(itemTarget.pItemObj->Type));
			}
			else {
				if (((itemTarget.pItemObj->Level >> 3) & 0xF) == 0) {
					if (gJewelStack.CheckIsStack(itemTarget.pItemObj->Type)) {
						int count = 1;
						if (itemTarget.pItemObj->Durability > 1) {
					
							count = itemTarget.pItemObj->Durability;
						}
						sprintf_s(buffer, sizeof(buffer), "%s (%dx)", itemAttr->Name, count);
					}
					else {
						sprintf_s(buffer, sizeof(buffer), "%s", itemAttr->Name);
					}
				}
				else {
					sprintf_s(buffer, sizeof(buffer), "%s +%d", itemAttr->Name, (itemTarget.pItemObj->Level >> 3) & 0xF);
				}
			}

			g_pRenderText->SetFont(g_hFont);
			if (MouseX >= x - 142 && MouseX <= x - 140 + 130 && MouseY >= y + SizeY && MouseY <= y + (SizeY + 8)) {
				g_pRenderText->SetBgColor(50, 50, 50, 100);

			}
			else {
				g_pRenderText->SetBgColor(0, 0, 0, 0);
			}
			
			g_pRenderText->SetTextColor(225, 225, 225, 255);
			g_pRenderText->RenderText((int)(x - 142), (int)(y + SizeY), buffer);
			DisableAlphaBlend();

			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}

	DisableAlphaBlend();

	this->m_PosX = x - 142;
	this->m_PosY = y;
	this->m_HeightBox = height - 35;

	if (this->m_MaxLines != MaxLines) {
		this->m_MaxLines = MaxLines;
		this->CalcLinesRender();
	}

	this->RenderScroll(x, y+8, 160, height, MaxLines);

}

void CChaosGenesis::CalcLinesRender()
{
	ScrollBarOffset = 0;
	ScrollBarPos = 0;
	ScrollBarMaxOffset = (int)(this->m_ChaosMachineItens.size() - (m_MaxLines + 1));
	ScrollBarMultiplier = (float)((float)45 / (float)(this->m_ChaosMachineItens.size() - (m_MaxLines)));
}

void CChaosGenesis::InsertItemChaosMachine(BYTE slot, BYTE* ItemInfo)
{
	CHAOS_MACHINE_ITENS info;

	info.pItemObj = g_pNewItemMng->CreateItem(ItemInfo);

	info.slot = slot;

	this->m_ChaosMachineItens.push_back(info);

	this->CalcLinesRender();
}

void CChaosGenesis::RenderItemInformation(float x, float y, ITEM* pItemObj)
{
	EnableAlphaTest(1);
	gDrawBarItem.SetItemDrawBar(1, pItemObj->Type, (int)pItemObj->Level * 8, (int)pItemObj->ExtOption, 0, 1.0);
	RenderItemInfo(x-30, MouseY, pItemObj, false, 0, false, true);
	DisableAlphaBlend();
}

void CChaosGenesis::DelItem(BYTE slot)
{
	std::vector<CHAOS_MACHINE_ITENS>::iterator li = this->m_ChaosMachineItens.begin();

	for (; li != this->m_ChaosMachineItens.end(); li++) {
		auto pItem = &(*li);

		if (pItem->slot == slot) {
			pItem->Clear();
			this->m_ChaosMachineItens.erase(li);
			break;
		}
	}
}

void CChaosGenesis::DeleteAllItems()
{
	for (auto& itemChaosBox : this->m_ChaosMachineItens) {
		itemChaosBox.Clear();
	}

	this->m_ChaosMachineItens.clear();
}

void CChaosGenesis::RenderScroll(float x, float y, float width, float height, int MaxLines)
{
	this->m_MaxLines = MaxLines;

	//Bar
	EnableAlphaTest(1);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	
	RenderImageScale(IMAGE_GENESISMIXINVENTORY_SCROLL_BACK, x, y, 10, 70, 0, 0, 30, 375, 1.0);
	
	if (m_EventState == EVENT_SCROLL_BTN_DOWN) {
		RenderImageScale(IMAGE_GENESISMIXINVENTORY_SCROLL_MIDDLE, x + 0.5, y + ScrollBarPos, 9, 25, 0, 0, 30, 60, 0.7);
	}
	else {
		RenderImageScale(IMAGE_GENESISMIXINVENTORY_SCROLL_MIDDLE, x + 0.5, y + ScrollBarPos, 9, 25, 0, 0, 30, 60, 1.0);
	}


	ScrollBarPosY = y + ScrollBarPos;
	ScrollBarPosX = (float)x+0.5f;

	DisableAlphaBlend();
}

void CChaosGenesis::ScrollLines(int mode)
{
	if (mode == 0) { //Up
		if (ScrollBarOffset > 0) {
			ScrollBarOffset = ScrollBarOffset - 1;
			ScrollBarPos = ScrollBarPos - ScrollBarMultiplier;
		}
	}
	else if (mode == 1) { //Down
		if (ScrollBarOffset < ScrollBarMaxOffset) {
			ScrollBarOffset = ScrollBarOffset + 1;
			ScrollBarPos = ScrollBarPos + ScrollBarMultiplier;
		}
	}
}

int CChaosGenesis::CheckCanMoveItemChaosBox(int ItemIndex)
{
	int retorno = 0;

	this->m_Lua.Generic_Call("ChaosMachineCheckCanMoveItem", "i>i", ItemIndex, &retorno);

	return retorno;
}

void CChaosGenesis::OpeningProcess()
{
	m_EventState = EVENT_NONE;

	this->m_Lua.Generic_Call("ChaosMachineOpenning", ">");	
}

bool CChaosGenesis::ClosingProcess()
{
	gDrawBarItem.ResetItemDrawBar(1);
	SendChaosGenesisClose();
	this->DeleteAllItems();
	return true;
}

void CChaosGenesis::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;
}

void CChaosGenesis::Scrolling(int value)
{
	if (CheckMouseIn((int)this->m_PosX - 5, (int)this->m_PosY, 145, 100)) {
		if (value > 0) {
			this->ScrollLines(0);
		}
	
		if (value < 0) {
			this->ScrollLines(1);
		}

		MouseWheel = 0;
	}

	int ret = 0;

	this->m_Lua.Generic_Call("ChaosMachineScrolling", "i>i", value, &ret);

	if (ret)
		MouseWheel = 0;

}

bool CChaosGenesis::UpdateMouseEvent()
{	
	if(CheckMouseIn(m_Pos.x, m_Pos.y, INVENTORY_WIDTH, INVENTORY_HEIGHT))
	{
		if (SEASON3B::IsPress(VK_RBUTTON))
		{
			if (this->MoveItemToInventory()) {
				MouseRButton = false;
				MouseRButtonPop = false;
				MouseRButtonPush = false;
				return false;
			}
			
		}

		if (CheckMouseIn(ScrollBarPosX, ScrollBarPosY, 9, 25)) {
			if (IsPress(VK_LBUTTON)) {
				m_EventState = EVENT_SCROLL_BTN_CLIKED;
				MouseLButton = false;
				MouseLButtonPop = false;
				MouseLButtonPush = false;
			}

			m_EventState = EVENT_SCROLL_BTN_DOWN;
		}
		else {
			m_EventState = EVENT_NONE;
		}

		if (CheckMouseIn((int)ScrollBarPosX-10, (int)(this->m_PosY), 30, 100)) {
			if (IsRepeat(VK_LBUTTON)) {
				
				int nextPos = ScrollBarPosY + ScrollBarMultiplier;
				if (MouseY < ScrollBarPosY) {
					this->ScrollLines(0);
				}
				else if (MouseY > nextPos) {
					this->ScrollLines(1);
				}
						
				m_iMouseY = MouseY;
			}
		}

		this->m_Lua.Generic_Call("ChaosMachineUpdateMouse", ">");

		
		if(SEASON3B::IsNone(VK_LBUTTON) == false)
		{
			return false;
		}
	}

	return true;
}

bool CChaosGenesis::UpdateKeyEvent()
{

	return true;
}

bool CChaosGenesis::Update()
{
	if (IsVisible())
	{
		this->ItemPointer = -1;

		this->m_Lua.Generic_Call("ChaosMachineUpdate", ">");
	}

	return true;
}

bool CChaosGenesis::Render()
{
	EnableAlphaTest();
	glColor4f(1.f, 1.f, 1.f, 1.f);

	this->m_Lua.Generic_Call("ChaosMachineRender", ">");

	DisableAlphaBlend();

	return true;
}

float CChaosGenesis::GetLayerDepth()
{ return 3.4f; }

void CChaosGenesis::LoadImages()
{
	LoadBitmap("Interface\\btn_mix.jpg", IMAGE_GENESISMIXINVENTORY_MIXBTN, GL_LINEAR);
	LoadBitmap("Interface\\default_window_back.jpg", IMAGE_GENESISMIXINVENTORY_BACK_VOID, GL_LINEAR);

	LoadBitmap("Interface\\scroll_back.jpg", IMAGE_GENESISMIXINVENTORY_SCROLL_BACK);
	LoadBitmap("Interface\\scroll_middle.jpg", IMAGE_GENESISMIXINVENTORY_SCROLL_MIDDLE);
}

void CChaosGenesis::UnloadImages()
{
	DeleteBitmap(IMAGE_GENESISMIXINVENTORY_BACK_VOID);
	DeleteBitmap(IMAGE_GENESISMIXINVENTORY_MIXBTN);

	DeleteBitmap(IMAGE_GENESISMIXINVENTORY_SCROLL_BACK);
	DeleteBitmap(IMAGE_GENESISMIXINVENTORY_SCROLL_MIDDLE);
}

void CChaosGenesis::SetItemAutoMove(bool bItemAutoMove)
{
	m_bItemAutoMove = bItemAutoMove;

	if (bItemAutoMove)
	{
		m_nBackupMouseX = MouseX;
		m_nBackupMouseY = MouseY;
	}
	else
		m_nBackupMouseX = m_nBackupMouseY = 0;
}

void CChaosGenesis::ProcessMyInvenItemAutoMove()
{
	if (g_pPickedItem)
	{
		if (g_pPickedItem->GetItem())
		{
			return;
		}
	}

	if (IsItemAutoMove())
	{
		return;
	}

	if (this->CheckChaosBoxGenesisSourceEmptySlot() == 0)
	{
		return;
	}

	CNewUIInventoryCtrl* pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();
	ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(MouseX, MouseY);
	if (pItemObj)
	{
		if (this->CheckCanMoveItemChaosBox(pItemObj->Type) == 0) {
			return;
		}

		SetItemAutoMove(true);

		int nSrcIndex
			= pItemObj->y * pMyInvenCtrl->GetNumberOfColumn()
			+ pItemObj->x;
		SendRequestEquipmentItem(REQUEST_EQUIPMENT_INVENTORY, MAX_EQUIPMENT_INDEX + nSrcIndex, pItemObj, REQUEST_EQUIPMENT_CHAOS_GENESIS, -1);

		::PlayBuffer(SOUND_GET_ITEM01);
	}
}

void CChaosGenesis::ProcessToRemoveGenesisItems(int nIndex, BYTE* pbyItemPacket)
{
	g_pMyInventory->InsertItem(this->m_TargetSlotBackup, pbyItemPacket);
	this->DelItem(nIndex);
	this->CalcLinesRender();
	SetItemAutoMove(false);
}

void CChaosGenesis::ProcessToReceiveGenesisItems(int nIndex, BYTE* pbyItemPacket)
{
	SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();

	if (nIndex >= 0 && nIndex < 120)
	{
		if (IsItemAutoMove())
		{
			CNewUIInventoryCtrl* pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();
			ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(m_nBackupMouseX, m_nBackupMouseY);
			g_pMyInventory->GetInventoryCtrl()->RemoveItem(pItemObj);

			SetItemAutoMove(false);
		}

		InsertItemChaosMachine(nIndex, pbyItemPacket);
	}
}

bool CChaosGenesis::MoveItemToInventory()
{
	if (IsItemAutoMove()) {
		return 0;
	}

	float SizeY = 0;
	int LineStart = 0;
	int LineCount = 0;

	for (auto& itemChaosBox : this->m_ChaosMachineItens) {
		if (LineStart >= ScrollBarOffset && LineCount <= 8) {
			if (!itemChaosBox.pItemObj) {
				continue;
			}

			if (MouseX >= this->m_PosX && MouseX <= this->m_PosX + 130) {
				if (MouseY >= this->m_PosY + SizeY && MouseY <= this->m_PosY + (SizeY + 8)) {
					auto itemAttr = &ItemAttribute[itemChaosBox.pItemObj->Type];

					if (itemAttr) {

						int nDstIndex = g_pMyInventory->FindEmptySlot(itemChaosBox.pItemObj);
						if (-1 != nDstIndex)
						{
							SetItemAutoMove(true);
							m_TargetSlotBackup = nDstIndex;
							SendRequestEquipmentItem(REQUEST_EQUIPMENT_CHAOS_GENESIS, itemChaosBox.slot,
								itemChaosBox.pItemObj, REQUEST_EQUIPMENT_INVENTORY, MAX_EQUIPMENT_INDEX + nDstIndex);

							::PlayBuffer(SOUND_GET_ITEM01);
							return 1;
						}
					}
				}
			}

			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}

	return 0;
}
