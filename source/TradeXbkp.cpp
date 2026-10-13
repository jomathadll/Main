//*****************************************************************************
// Desc: implementation of the CNewUITradeX class.
//*****************************************************************************

#include "stdafx.h"

#include "TradeX.h"
#include "NewUISystem.h"
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
#include "NewUICustomMessageBox.h"
#elif CLIENT_VERSION >= 1001
#include "VoidMuCustomMessageBox.h"
#endif
#include "wsclientinline.h"
#include "CComGem.h"
#include "CustomWing.h"

using namespace SEASON3B;

CNewUITradeX::CNewUITradeX()
{
	m_pNewUIMng = NULL;
	m_Pos.x = m_Pos.y = 0;
	this->m_MyItens.clear();
	this->m_YourItens.clear();
}

CNewUITradeX::~CNewUITradeX()
{
	Release();
}

bool CNewUITradeX::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng || NULL == g_pNewUI3DRenderMng
		|| NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_TRADEX, this);

	//m_pYourInvenCtrl = new CNewUIInventoryCtrl;
	//if (false == m_pYourInvenCtrl->Create(g_pNewUI3DRenderMng, g_pNewItemMng,
	//	this, x + 16, y + 68, COLUMN_TRADE_INVEN, ROW_TRADE_INVEN))
	//{
	//	SAFE_DELETE(m_pYourInvenCtrl);
	//	return false;
	//}
	//m_pYourInvenCtrl->LockInventory();	// 상대방 물품은 클릭 안되게 함.
	//
	//m_pMyInvenCtrl = new CNewUIInventoryCtrl;
	//if (false == m_pMyInvenCtrl->Create(g_pNewUI3DRenderMng, g_pNewItemMng,
	//	this, x + 16, y + 274, COLUMN_TRADE_INVEN, ROW_TRADE_INVEN))
	//{
	//	SAFE_DELETE(m_pMyInvenCtrl);
	//	return false;
	//}

	SetPos(x, y);

	LoadImages();

	m_abtn[BTN_CLOSE].ChangeButtonImgState(true, IMAGE_TRADE_BTN_CLOSE);
	m_abtn[BTN_CLOSE].ChangeButtonInfo(x + 13, y + 390, 36, 29);
	m_abtn[BTN_CLOSE].ChangeToolTipText(GlobalText[1002], true);

	m_abtn[BTN_ZEN_INPUT].ChangeButtonImgState(true, IMAGE_TRADE_BTN_ZEN_INPUT);
	m_abtn[BTN_ZEN_INPUT].ChangeButtonInfo(x + 104, y + 390, 36, 29);
	m_abtn[BTN_ZEN_INPUT].ChangeToolTipText(GlobalText[227], true);


#if CLIENT_VERSION >= 1001
	m_abtn[BTN_CLOSE].ChangeButtonInfo(x + 23, y + 390, 25, 24);
	m_abtn[BTN_ZEN_INPUT].ChangeButtonInfo(x + 104, y + 390, 25, 24);
#endif

	::memset(m_szYourID, 0, MAX_ID_SIZE + 1);
	m_bTradeAlert = false;

	InitTradeInfo();

	Show(false);

	return true;
}

void CNewUITradeX::InitTradeInfo()
{
	m_nYourLevel = 0;
	m_nYourGuildType = -1;
	m_nYourTradeGold = 0;
	m_nMyTradeGold = 0;
	m_nMyTradeWait = 0;
	m_bYourConfirm = m_bMyConfirm = false;
}

const POINT& SEASON3B::CNewUITradeX::GetPos() const
{
	return m_Pos;
}

void CNewUITradeX::Release()
{
	UnloadImages();

	if (m_pNewUIMng)
	{
		m_pNewUIMng->RemoveUIObj(this);
		m_pNewUIMng = NULL;
	}

	if (g_pNewUI3DRenderMng)
		g_pNewUI3DRenderMng->DeleteUI2DEffectObject(UI2DEffectCallback);
}

void CNewUITradeX::SetPos(int x, int y)
{
	m_Pos.x = x;
	m_Pos.y = y;

//m_posMyConfirm.x = m_Pos.x + 144;
//m_posMyConfirm.y = m_Pos.y + 390;
}

bool CNewUITradeX::UpdateMouseEvent()
{
	//if ((m_pYourInvenCtrl && false == m_pYourInvenCtrl->UpdateMouseEvent())
	//	|| (m_pMyInvenCtrl && false == m_pMyInvenCtrl->UpdateMouseEvent()))
	//{
	//	if (SEASON3B::IsPress(VK_LBUTTON)
	//		&& CNewUIInventoryCtrl::GetPickedItem()->GetOwnerInventory() == m_pMyInvenCtrl
	//		&& m_bMyConfirm)
	//	{
	//		m_bMyConfirm = false;
	//		SendRequestTradeResult(m_bMyConfirm);
	//	}
	//
	//	return false;
	//}

	ProcessMyInvenCtrl();

	if (ProcessBtns())
		return false;

	if (CheckMouseIn(m_Pos.x, m_Pos.y, TRADE_WIDTH, TRADE_HEIGHT))
	{
		if (SEASON3B::IsPress(VK_RBUTTON))
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
			return false;
		}

		if (SEASON3B::IsNone(VK_LBUTTON) == false)
		{
			return false;
		}
	}

	return true;
}

bool CNewUITradeX::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADEX) == true)
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			SendRequestTradeExit();
			g_pNewUISystem->Hide(SEASON3B::INTERFACE_TRADEX);
			PlayBuffer(SOUND_CLICK01);

			return false;
		}
	}
	return true;
}

bool CNewUITradeX::Update()
{
	//if ((m_pYourInvenCtrl && false == m_pYourInvenCtrl->Update())
	//	|| (m_pMyInvenCtrl && false == m_pMyInvenCtrl->Update()))
	//	return false;

	return true;
}

bool CNewUITradeX::Render()
{
	::EnableAlphaTest();

	::glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderBackImage();
	RenderText();

	//if (m_pYourInvenCtrl)
	//	m_pYourInvenCtrl->Render();
	//if (m_pMyInvenCtrl)
	//	m_pMyInvenCtrl->Render();

	for (int i = BTN_CLOSE; i < MAX_BTN; ++i)
		m_abtn[i].Render();

	if (g_pNewUI3DRenderMng)
		g_pNewUI3DRenderMng->RenderUI2DEffect(INVENTORY_CAMERA_Z_ORDER,
			UI2DEffectCallback, this, 0, 0);

	::DisableAlphaBlend();

	return true;
}

void CNewUITradeX::UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB)
{
	if (pClass)
	{
		CNewUITradeX* pNewUITrade = (CNewUITradeX*)pClass;
		//pNewUITrade->RenderWarningArrow();
	}
}

void CNewUITradeX::RenderBackImage()
{

	RenderImageScaleAuto(IMAGE_TRADE_BACK_VOID, m_Pos.x, m_Pos.y, TRADE_WIDTH, TRADE_HEIGHT);
	RenderImageScaleAuto(IMAGE_TRADE_LINE, m_Pos.x + 7.1f, m_Pos.y + 228, 176.f, 10.f);

	RenderBitmap(IMAGE_TRADE_MONEY, m_Pos.x + 11, m_Pos.y + 39, 167.f, 26.f, 0.002, 0.002, 0.994234, 0.7204928, 1, 1, 0.0);
	RenderBitmap(IMAGE_TRADE_MONEY, m_Pos.x + 11, m_Pos.y + 243, 167.f, 26.f, 0.002, 0.002, 0.994234, 0.7204928, 1, 1, 0.0);


	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);
	g_pRenderText->SetTextColor(235, 210, 0, 255);
	g_pRenderText->RenderText(m_Pos.x + 23, m_Pos.y + 157, GlobalText[224], 20, 0, RT3_SORT_LEFT);
	g_pRenderText->RenderText(m_Pos.x + 23, m_Pos.y + 363, GlobalText[224], 20, 0, RT3_SORT_LEFT);

	RenderBitmap(IMAGE_TRADE_MONEY, m_Pos.x + 43, m_Pos.y + 152, 133, 18, 0.002, 0.002, 0.994234, 0.7204928, 1, 1, 0.0);
	RenderBitmap(IMAGE_TRADE_MONEY, m_Pos.x + 43, m_Pos.y + 358, 133, 18, 0.002, 0.002, 0.994234, 0.7204928, 1, 1, 0.0);


	if (m_bYourConfirm) {
		RenderBitmap(IMAGE_TRADE_CONFIRM, m_Pos.x + 144, m_Pos.y + 186, 25, 24, 0.008, 0.429, 0.83, 0.20, 1, 1, 1.0);
	}
	else {
		RenderBitmap(IMAGE_TRADE_CONFIRM, m_Pos.x + 144, m_Pos.y + 186, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
	}

	if (m_nMyTradeWait > 0) {
		RenderBitmap(IMAGE_TRADE_CONFIRM, m_Pos.x + 144, m_Pos.y + 390, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 0.5);
	}
	else {
		if (m_bMyConfirm) {
			RenderBitmap(IMAGE_TRADE_CONFIRM, m_Pos.x + 144, m_Pos.y + 390, 25, 24, 0.008, 0.429, 0.83, 0.20, 1, 1, 1.0);
		}
		else {
			RenderBitmap(IMAGE_TRADE_CONFIRM, m_Pos.x + 144, m_Pos.y + 390, 25, 24, 0.008, 0.002, 0.83, 0.20, 1, 1, 1.0);
		}
	}
}

void CNewUITradeX::RenderText()
{
	unicode::t_char szTemp[128];

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetBgColor(0);

#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502

	g_pRenderText->SetTextColor(216, 216, 216, 255);
	g_pRenderText->RenderText(
		m_Pos.x, m_Pos.y + 11, GlobalText[226], TRADE_WIDTH, 0, RT3_SORT_CENTER);

	for (int i = 0; i < MAX_MARKS; ++i)
	{
		if (GuildMark[i].Key != -1 && GuildMark[i].Key == m_nYourGuildType)
		{
			::CreateGuildMark(i, false);
			::RenderBitmap(BITMAP_GUILD, (float)m_Pos.x + 15, (float)m_Pos.y + 42, 16, 16);
			g_pRenderText->RenderText(m_Pos.x + 16, m_Pos.y + 30, GuildMark[i].GuildName);
			break;
		}
	}

	g_pRenderText->SetFont(g_hFontBig);

	g_pRenderText->SetTextColor(210, 230, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 32, m_Pos.y + 43, m_szYourID);

	g_pRenderText->SetFont(g_hFontBold);

	int nLevel;
	DWORD dwColor;
	ConvertYourLevel(nLevel, dwColor);
	if (nLevel == 400)
	{
		unicode::_sprintf(szTemp, "%d", nLevel);
	}
	else
	{
		unicode::_sprintf(szTemp, GlobalText[369], nLevel);
	}
	g_pRenderText->SetTextColor(dwColor);
	g_pRenderText->RenderText(m_Pos.x + 134, m_Pos.y + 48, "Lv.");
	g_pRenderText->RenderText(m_Pos.x + 148, m_Pos.y + 48, szTemp);

#elif CLIENT_VERSION >= 1001
	g_pRenderText->SetTextColor(255, 255, 255, 255);
	g_pRenderText->RenderText(
		m_Pos.x, m_Pos.y + 14, GlobalText[226], TRADE_WIDTH, 0, RT3_SORT_CENTER);
	g_pRenderText->SetTextColor(210, 230, 255, 255);

	for (int i = 0; i < MAX_MARKS; ++i)
	{
		if (GuildMark[i].Key != -1 && GuildMark[i].Key == m_nYourGuildType)
		{
			::CreateGuildMark(i, false);
			::RenderBitmap(BITMAP_GUILD, (float)m_Pos.x + 157, (float)m_Pos.y + 44, 16, 16);
			g_pRenderText->RenderText(m_Pos.x + 153, m_Pos.y + 48, GuildMark[i].GuildName, 50, 0, RT3_WRITE_RIGHT_TO_LEFT);
			break;
		}
	}

	g_pRenderText->SetFont(g_hFontBig);

	g_pRenderText->SetTextColor(210, 230, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 44, m_szYourID);
	g_pRenderText->SetFont(g_hFont);

#endif

	::ConvertGold(m_nYourTradeGold, szTemp);
	g_pRenderText->SetTextColor(::getGoldColor(m_nYourTradeGold));
	g_pRenderText->RenderText(
		m_Pos.x + 170, m_Pos.y + 150 + 7, szTemp, 0, 0, RT3_WRITE_RIGHT_TO_LEFT);

	::ConvertGold(m_nMyTradeGold, szTemp);
	g_pRenderText->SetTextColor(::getGoldColor(m_nMyTradeGold));
	g_pRenderText->RenderText(
		m_Pos.x + 170, m_Pos.y + 356 + 7, szTemp, 0, 0, RT3_WRITE_RIGHT_TO_LEFT);

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(210, 230, 255, 255);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 253, Hero->ID);

	g_pRenderText->SetFont(g_hFont);
	int nAlpha = int(min(255, sin(WorldTime / 200) * 200 + 275));
	g_pRenderText->SetTextColor(210, 0, 0, nAlpha);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 185, GlobalText[370]);
	g_pRenderText->SetTextColor(255, 220, 150, 255);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 200, GlobalText[365]);
	g_pRenderText->RenderText(m_Pos.x + 20, m_Pos.y + 215, GlobalText[367]);
}

float CNewUITradeX::GetLayerDepth()
{
	return 2.1f;
}

void CNewUITradeX::LoadImages()
{
	LoadBitmap("Interface\\btn_exit.jpg", IMAGE_TRADE_BTN_CLOSE, GL_LINEAR, GL_REPEAT);
	LoadBitmap("Interface\\division_bar.tga", IMAGE_TRADE_LINE, GL_LINEAR);
	LoadBitmap("Interface\\newui_Account_title.tga", IMAGE_TRADE_NICK_BACK, GL_LINEAR);
	LoadBitmap("Interface\\item_money.tga", IMAGE_TRADE_MONEY, GL_LINEAR);
	LoadBitmap("Interface\\CursorSitDown.tga", IMAGE_TRADE_WARNING_ARROW, GL_LINEAR, GL_CLAMP);
	LoadBitmap("Interface\\btn_deposit.jpg", IMAGE_TRADE_BTN_ZEN_INPUT, GL_LINEAR);
	LoadBitmap("Interface\\btn_circle_trade.jpg", IMAGE_TRADE_CONFIRM, GL_LINEAR);
	LoadBitmap("Interface\\default_window_back.jpg", IMAGE_TRADE_BACK_VOID, GL_LINEAR);
}

void CNewUITradeX::UnloadImages()
{
	DeleteBitmap(IMAGE_TRADE_BTN_ZEN_INPUT);
	DeleteBitmap(IMAGE_TRADE_BTN_CLOSE);
	DeleteBitmap(IMAGE_TRADE_WARNING_ARROW);
	DeleteBitmap(IMAGE_TRADE_CONFIRM);
	DeleteBitmap(IMAGE_TRADE_MONEY);
	DeleteBitmap(IMAGE_TRADE_NICK_BACK);
	DeleteBitmap(IMAGE_TRADE_LINE);
	DeleteBitmap(IMAGE_TRADE_BACK_VOID);

}

void CNewUITradeX::ProcessClosing()
{
	//m_pYourInvenCtrl->RemoveAllItems();
	//m_pMyInvenCtrl->RemoveAllItems();

//	if (m_bTradeAlert)
//		InitYourInvenBackUp();
}

void CNewUITradeX::ProcessMyInvenCtrl()
{
	/*
	if (SEASON3B::IsPress(VK_LBUTTON))
	{
		CNewUIPickedItem* pPickedItem = CNewUIInventoryCtrl::GetPickedItem();
		if (NULL == pPickedItem)
			return;

		ITEM* pItemObj = pPickedItem->GetItem();
		if (pPickedItem->GetOwnerInventory() == g_pMyInventory->GetInventoryCtrl())
		{
			int nSrcIndex = pPickedItem->GetSourceLinealPos();
			int nDstIndex = pPickedItem->GetTargetLinealPos(m_pMyInvenCtrl);
			if (nDstIndex != -1 && m_pMyInvenCtrl->CanMove(nDstIndex, pItemObj))
				SendRequestItemToTrade(pItemObj, MAX_EQUIPMENT_INDEX + nSrcIndex, nDstIndex);
		}
		else if (pPickedItem->GetOwnerInventory() == m_pMyInvenCtrl)
		{
			int nSrcIndex = pPickedItem->GetSourceLinealPos();
			int nDstIndex = pPickedItem->GetTargetLinealPos(m_pMyInvenCtrl);
			if (nDstIndex != -1 && m_pMyInvenCtrl->CanMove(nDstIndex, pItemObj))
			{
				SendRequestEquipmentItem(REQUEST_EQUIPMENT_TRADE, nSrcIndex, pItemObj, REQUEST_EQUIPMENT_TRADE, nDstIndex);
			}
		}
		else if (pItemObj->ex_src_type == ITEM_EX_SRC_EQUIPMENT)
		{
			int nSrcIndex = pPickedItem->GetSourceLinealPos();
			int nDstIndex = pPickedItem->GetTargetLinealPos(m_pMyInvenCtrl);
			if (nDstIndex != -1 && m_pMyInvenCtrl->CanMove(nDstIndex, pItemObj))
				SendRequestItemToTrade(pItemObj, nSrcIndex, nDstIndex);
		}
	}
	*/
}

void CNewUITradeX::SendRequestItemToTrade(ITEM* pItemObj, int nInvenIndex,
	int nTradeIndex)
{
	if (::IsTradeBan(pItemObj))
	{
		g_pChatListBox->AddText(
			"", GlobalText[494], SEASON3B::TYPE_ERROR_MESSAGE);
	}
	else
	{
		m_bMyConfirm = false;
		SendRequestTradeResult(m_bMyConfirm);

		SendRequestEquipmentItem(REQUEST_EQUIPMENT_INVENTORY, nInvenIndex,
			pItemObj, REQUEST_EQUIPMENT_TRADE, nTradeIndex);
	}
}

void CNewUITradeX::SendRequestItemToMyInven(ITEM* pItemObj, int nTradeIndex, int nInvenIndex)
{
	SendRequestEquipmentItem(REQUEST_EQUIPMENT_TRADE, nTradeIndex, pItemObj, REQUEST_EQUIPMENT_INVENTORY, nInvenIndex);

	if (m_bMyConfirm)
	{
		AlertTrade();
	}
	m_nMyTradeWait = 150;
}

void CNewUITradeX::SendRequestMyGoldInput(int nInputGold)
{
	if (nInputGold <= (int)CharacterMachine->Gold + m_nMyTradeGold)
	{
		if (m_bMyConfirm)
		{
			m_bMyConfirm = false;
			SendRequestTradeResult(m_bMyConfirm);
		}

		if (m_nMyTradeGold > 0)
			m_nMyTradeWait = 150;

		m_nTempMyTradeGold = nInputGold;
		SendRequestTradeGold(nInputGold);
	}
	else
	{
		SEASON3B::CreateOkMessageBox(GlobalText[423]);
	}

}

void CNewUITradeX::ProcessCloseBtn()
{
	if (CNewUIInventoryCtrl::GetPickedItem() == NULL)
	{
		m_bTradeAlert = false;
		SendRequestTradeExit();
	}
}

bool CNewUITradeX::ProcessBtns()
{
	if (m_nMyTradeWait > 0)
		--m_nMyTradeWait;

	if (m_abtn[BTN_CLOSE].UpdateMouseEvent())
	{
		::PlayBuffer(SOUND_CLICK01);
		ProcessCloseBtn();
		return true;
	}
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	else if (SEASON3B::IsPress(VK_LBUTTON)
		&& CheckMouseIn(m_Pos.x + 169, m_Pos.y + 7, 13, 12))
	{
		::PlayBuffer(SOUND_CLICK01);
		ProcessCloseBtn();
		return true;
	}
#endif
	else if (m_abtn[BTN_ZEN_INPUT].UpdateMouseEvent())
	{
		SEASON3B::CreateMessageBox(
			MSGBOX_LAYOUT_CLASS(SEASON3B::CTradeZenMsgBoxLayout));
		::PlayBuffer(SOUND_CLICK01);
		return true;
	}
#if CLIENT_VERSION >= 401 && CLIENT_VERSION <= 502
	else if (SEASON3B::IsRelease(VK_LBUTTON)
		&& CheckMouseIn(m_posMyConfirm.x, m_posMyConfirm.y, CONFIRM_WIDTH, CONFIRM_HEIGHT))
#elif CLIENT_VERSION >= 1001
	else if (SEASON3B::IsRelease(VK_LBUTTON)
		)//&& CheckMouseIn(m_posMyConfirm.x, m_posMyConfirm.y, 25, 24))
#endif
	{
		if (0 == m_nMyTradeWait && CNewUIInventoryCtrl::GetPickedItem() == NULL)
		{
			::PlayBuffer(SOUND_CLICK01);

			if (m_bTradeAlert && !m_bMyConfirm)
			{
				SEASON3B::CreateMessageBox(
					MSGBOX_LAYOUT_CLASS(SEASON3B::CTradeAlertMsgBoxLayout));
			}
			else
			{
				AlertTrade();
			}
		}
		return true;
	}

	return false;
}

void CNewUITradeX::AlertTrade()
{
	m_bMyConfirm = !m_bMyConfirm;

	m_bTradeAlert = true;
	SendRequestTradeResult(m_bMyConfirm);
}

void CNewUITradeX::GetYourID(char* pszYourID)
{
	::strcpy(pszYourID, m_szYourID);
}

void CNewUITradeX::ProcessToReceiveTradeRequest(BYTE* pbyYourID)
{
	if (g_pNewUISystem->IsImpossibleTradeInterface())
	{
		SendRequestTradeAnswer(false);
		return;
	}

	::memcpy(m_szYourID, pbyYourID, MAX_ID_SIZE);
	m_szYourID[MAX_ID_SIZE] = NULL;

	SEASON3B::CreateMessageBox(MSGBOX_LAYOUT_CLASS(SEASON3B::CTradeMsgBoxLayout));

	SEASON3B::CNewUIInventoryCtrl::BackupPickedItem();
}

void CNewUITradeX::ProcessToReceiveTradeResult(LPPTRADE pTradeData)
{
	switch (pTradeData->SubCode)
	{
	case 0:
		g_pChatListBox->AddText("", GlobalText[492], SEASON3B::TYPE_ERROR_MESSAGE);
		break;

	case 2:
		g_pChatListBox->AddText("", GlobalText[493], SEASON3B::TYPE_ERROR_MESSAGE);
		break;

	case 1:
		g_pNewUISystem->Show(SEASON3B::INTERFACE_TRADEX);

		InitTradeInfo();

		int x = 260 * MouseX / 640;
		::SetCursorPos(x * WindowWidth / 640, MouseY * WindowHeight / 480);

		char szTempID[MAX_ID_SIZE + 1];
		::memcpy(szTempID, pTradeData->ID, MAX_ID_SIZE);
		szTempID[MAX_ID_SIZE] = NULL;

		//if (!m_bTradeAlert && ::strcmp(m_szYourID, szTempID))
		//	InitYourInvenBackUp();

		m_bTradeAlert = false;
		m_nYourGuildType = pTradeData->GuildKey;
		::memcpy(m_szYourID, pTradeData->ID, MAX_ID_SIZE);
		m_szYourID[MAX_ID_SIZE] = NULL;
		m_nYourLevel = pTradeData->Level;   //  상대방 레벨.
		break;
	}
}

void CNewUITradeX::ProcessToReceiveYourItemDelete(BYTE byYourInvenIndex)
{
	/*
	BackUpYourInven(int(byYourInvenIndex));
	ITEM* pYourItemObj = m_pYourInvenCtrl->FindItem(int(byYourInvenIndex));
	m_pYourInvenCtrl->RemoveItem(pYourItemObj);
	AlertYourTradeInven();
	::PlayBuffer(SOUND_GET_ITEM01);
	*/
}

void CNewUITradeX::BackUpYourInven(int nYourInvenIndex)
{
	/*
	ITEM* pYourItemObj = m_pYourInvenCtrl->FindItem(nYourInvenIndex);
	BackUpYourInven(pYourItemObj);
	*/
}

void CNewUITradeX::BackUpYourInven(ITEM* pYourItemObj)
{
	/*
	if ((pYourItemObj->Type >= ITEM_HELPER && pYourItemObj->Type <= ITEM_HELPER + 4)
		|| (pYourItemObj->Type == ITEM_POTION + 13 || pYourItemObj->Type == ITEM_POTION + 14 || pYourItemObj->Type == ITEM_POTION + 16 || gCustomJewel.CheckCustomJewelByItem(pYourItemObj->Type))
		|| (pYourItemObj->Type >= ITEM_POTION + 31)
		|| (COMGEM::isCompiledGem(pYourItemObj))
		|| (pYourItemObj->Type >= ITEM_WING && pYourItemObj->Type <= ITEM_WING + 6)
		|| (pYourItemObj->Type >= ITEM_HELPER + 30)
		|| (pYourItemObj->Type >= ITEM_WING + 36 && pYourItemObj->Type <= ITEM_WING + 43)
		|| (pYourItemObj->Type == ITEM_WING + 15)
#ifdef PBG_ADD_NEWCHAR_MONK_ITEM
		|| (pYourItemObj->Type >= ITEM_WING + 49 && pYourItemObj->Type <= ITEM_WING + 50)
#endif //PBG_ADD_NEWCHAR_MONK_ITEM
		|| gCustomWing.CheckCustomWingByItem(pYourItemObj->Type)
		|| ((((pYourItemObj->Level >> 3) & 15) > 4 && pYourItemObj->Type < ITEM_WING) || (pYourItemObj->Option1 & 63) > 0))
	{
		int nCompareValue;
		bool bSameItem = false;

		for (int i = 0; i < MAX_TRADE_INVEN; ++i)
		{
			if (-1 == m_aYourInvenBackUp[i].Type)
				continue;

			nCompareValue = ::CompareItem(m_aYourInvenBackUp[i], *pYourItemObj);
			if (0 == nCompareValue)
			{
				bSameItem = true;
				break;
			}
			else if (-1 == nCompareValue)
			{
				bSameItem = true;
				m_aYourInvenBackUp[i] = *pYourItemObj;
				break;
			}
			else if (2 != nCompareValue)
			{
				bSameItem = true;
			}
		}

		if (!bSameItem)
		{
			for (int i = 0; i < MAX_TRADE_INVEN; ++i)
			{
				if (-1 == m_aYourInvenBackUp[i].Type)
				{
					m_aYourInvenBackUp[i] = *pYourItemObj;
					break;
				}
			}
		}
	}
	*/
}

void CNewUITradeX::ProcessToReceiveYourItemAdd(BYTE byYourInvenIndex, BYTE* pbyItemPacket)
{
	/*
	m_pYourInvenCtrl->AddItem(byYourInvenIndex, pbyItemPacket);
	AlertYourTradeInven();
	::PlayBuffer(SOUND_GET_ITEM01);
	*/
}

void CNewUITradeX::AlertYourTradeInven()
{

}

void CNewUITradeX::ProcessToReceiveMyTradeGold(BYTE bySuccess)
{
	m_nMyTradeGold = bySuccess ? m_nTempMyTradeGold : 0;
}

void CNewUITradeX::ProcessToReceiveYourConfirm(BYTE byState)
{
	switch (byState)
	{
	case 0:
		m_bYourConfirm = false;
		break;
	case 1:
		m_bYourConfirm = true;
		break;
	case 2:
		m_bMyConfirm = false;
		m_bYourConfirm = false;
		m_nMyTradeWait = 150;
		break;
	case 3:
		break;

	}

	PlayBuffer(SOUND_CLICK01);
}

void CNewUITradeX::ProcessToReceiveTradeExit(BYTE byState)
{
	switch (byState)
	{
	case 0:
	{
		g_pChatListBox->AddText("", GlobalText[492], SEASON3B::TYPE_ERROR_MESSAGE);

		m_bTradeAlert = false;

		//int nYourItems = m_pYourInvenCtrl->GetNumberOfItems();
		//for (int i = 0; i < nYourItems; ++i)
		//	BackUpYourInven(m_pYourInvenCtrl->GetItem(i));
	}
	break;

	case 2:
		g_pChatListBox->AddText("", GlobalText[495], SEASON3B::TYPE_ERROR_MESSAGE);
		break;

	case 3:
		g_pChatListBox->AddText("", GlobalText[496], SEASON3B::TYPE_ERROR_MESSAGE);
		break;

	case 4:
		g_pChatListBox->AddText("", GlobalText[2108], SEASON3B::TYPE_ERROR_MESSAGE);
		break;
	}

	SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();

	g_MessageBox->PopMessageBox();

	g_pNewUISystem->Hide(SEASON3B::INTERFACE_TRADEX);
}

void CNewUITradeX::ProcessToReceiveTradeItems(int nIndex, BYTE* pbyItemPacket)
{
	SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();

	//if (nIndex >= 0 && nIndex < (m_pMyInvenCtrl->GetNumberOfColumn()
	//	* m_pMyInvenCtrl->GetNumberOfRow()))
	//	m_pMyInvenCtrl->AddItem(nIndex, pbyItemPacket);
}

int SEASON3B::CNewUITradeX::GetPointedItemIndexMyInven()
{
	return 0;
	//return m_pMyInvenCtrl->GetPointedSquareIndex();
}

int SEASON3B::CNewUITradeX::GetPointedItemIndexYourInven()
{
	return 0;
	//return m_pYourInvenCtrl->GetPointedSquareIndex();
}

void CNewUITradeX::NewDeleteItem(ITEM* item) {
	g_pNewItemMng->DeleteItem(item);
}