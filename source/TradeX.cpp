#include "stdafx.h"
#include "TradeX.h"
#include "NewUISystem.h"
#include "NewUIInventoryCtrl.h"
#include "ZzzInventory.h"
#include "wsclientinline.h"
#include "WideScreen.h"
#include "DrawBarItem.h"
#include "VisualDescriptions.h"

using namespace SEASON3B;

CTradeX::CTradeX()
{
	this->m_PosX = 260;

	this->m_PosY = 0;

	this->m_isVisible = false;

	this->m_TradeItemSource.clear();

	this->m_TradeItemTarget.clear();

	m_iMouseYSource = 0;

	m_iMouseYTarget = 0;

	TradeXHidden = 0;

	TradeXTransation = 0;

	TimeTradeX = 10;

	TickBlockOk = 0;

	this->m_nBackupMouseX = 0;

	this->m_nBackupMouseY = 0;

	this->m_bItemAutoMove = false;

	memcpy(NameTarget, "OgocX", sizeof(NameTarget));
}

CTradeX::~CTradeX()
{
	std::vector<TradeItem>::iterator target = this->m_TradeItemTarget.begin();

	for (; target != this->m_TradeItemTarget.end(); target++)
	{
		target->Clear();
	}

	this->m_TradeItemTarget.clear();

	std::vector<TradeItem>::iterator source = this->m_TradeItemSource.begin();

	for (; source != this->m_TradeItemSource.end(); source++)
	{
		source->Clear();
	}

	this->m_TradeItemSource.clear();
}

bool CTradeX::Create(CNewUIManager* pNewUIMng, int x, int y)
{
	if (NULL == pNewUIMng || NULL == g_pNewUI3DRenderMng || NULL == g_pNewItemMng)
		return false;

	m_pNewUIMng = pNewUIMng;
	m_pNewUIMng->AddUIObj(SEASON3B::INTERFACE_TRADEX, this);

	this->m_PosY = 0;
	this->m_PosX = 260 + gWideScreen.g_WideWindowWidthAdd;

	this->m_BtnAccept.SetButtonSize(1);
	this->m_BtnAccept.ChangeButtonImgState(true, 31322, true);
	this->m_BtnAccept.SetFont(g_hFontBold);
	this->m_BtnAccept.ChangeTextColor(RGBA(245, 232, 202, 255));
	this->m_BtnAccept.ChangeButtonInfo(this->m_PosX + 16, this->m_PosY + 380, 75, 30);
	this->m_BtnAccept.ChangeText(GlobalText[3186]);

	this->m_BtnCancel.SetButtonSize(1);
	this->m_BtnCancel.ChangeButtonImgState(true, 31322, true);
	this->m_BtnCancel.SetFont(g_hFontBold);
	this->m_BtnCancel.ChangeTextColor(RGBA(245, 232, 202, 255));
	this->m_BtnCancel.ChangeButtonInfo(this->m_PosX + 100, this->m_PosY + 380, 75, 30);
	this->m_BtnCancel.ChangeText(GlobalText[3185]);

	this->m_BtnShow.SetButtonSize(0);
	this->m_BtnShow.ChangeButtonImgState(true, BITMAP_TRADEX_ICON, true, false, true);
	this->m_BtnShow.ChangeButtonInfo(100, 50, 25, 25);
	this->m_BtnShow.ChangeToolTipText(GlobalText[3172], true);

	//LoadBlooImages();

	Show(false);

	return true;
}

bool CTradeX::isVisible()
{
	return this->m_isVisible;
}

void CTradeX::SetItemAutoMove(bool ItemAutoMove)
{
	m_bItemAutoMove = ItemAutoMove;

	if (ItemAutoMove)
	{
		m_nBackupMouseX = MouseX;
		m_nBackupMouseY = MouseY;
	}
	else
	{
		m_nBackupMouseX = m_nBackupMouseY = 0;
	}

}

bool CTradeX::CheckMouse()
{
	if (SEASON3B::CheckMouseIn(this->m_PosX, this->m_PosY, 190, 429)) {
		return true;
	}

	return false;
}

void CTradeX::OpeningProcess()
{
	this->m_isVisible = 1;
	m_EventStateTarget = EVENT_NONE;
	m_EventStateSource = EVENT_NONE;

	ScrollBarPosXTarget = (float)(this->m_PosX + 160);

	ScrollBarPosXSource = (float)(this->m_PosX + 160);

	m_iMouseYSource = 0;
	m_iMouseYTarget = 0;
}

bool CTradeX::ClosingProcess()
{
	gDrawBarItem.ResetItemDrawBar(3);
	gDrawBarItem.ResetItemDrawBar(2);
	this->m_isVisible = 0;
	return true;
}

void CTradeX::Close()
{
}

void CTradeX::ClearTradeX()
{
	this->ClearTradeXSource();
	this->ClearTradeXTarget();

	gDrawBarItem.ResetItemDrawBar(3);
	gDrawBarItem.ResetItemDrawBar(2);

	this->m_isVisible = 0;

	memcpy(this->NameTarget, "", sizeof(this->NameTarget));

	this->TradeXTransation = 0;

	this->TradeXHidden = 0;

	this->m_SourceAcceptTradeX = 0;
	this->m_TargetAcceptTradeX = 0;

	this->TickBlockOk = 0;

	this->ClickEvent = GetTickCount();
}

bool CTradeX::Render()
{
	EnableAlphaTest(1);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	this->RenderFrame();

	this->RenderText();

	EnableAlphaTest();

	this->RenderButtons();

	this->RenderScrollTarget((float)(this->m_PosX + 162), (float)(this->m_PosY + 115), 4, 70);

	this->RenderScrollSource((float)(this->m_PosX + 162), (float)(this->m_PosY + 275), 4, 70);

	DisableAlphaBlend();

	return true;
}

void CTradeX::RenderFrame()
{
	EnableAlphaTest();
	SEASON3B::RenderImageScaleAuto(33001, m_PosX, m_PosY, 190, 429);
	SEASON3B::RenderImageScaleAuto(31395, m_PosX + 7.1f, m_PosY + 212, 176.f, 10.f);

	if (this->m_TargetAcceptTradeX)
	{
		RenderBitmap(BITMAP_NEW_CHECKBOX_ON, (float)(this->m_PosX + 15), (float)(this->m_PosY + 66.7), 12, 12, 0.008, 0.213, 0.83, 0.20, 1, 1, 1.0);
	}
	else
	{
		RenderBitmap(BITMAP_NEW_CHECKBOX_OFF, (float)(this->m_PosX + 15), (float)(this->m_PosY + 66.7), 12, 12, 0.008, 0.429, 0.83, 0.20, 1, 1, 1.0);
	}

	this->RenderBoxItem((float)(this->m_PosX + 15), (float)(this->m_PosY + 85), 160, 120);

	if (this->m_SourceAcceptTradeX)
	{
		RenderBitmap(BITMAP_NEW_CHECKBOX_ON, this->m_PosX + 15, this->m_PosY + 228.1, 12, 12, 0.008, 0.213, 0.83, 0.20, 1, 1, 1.0);
	}
	else
	{
		RenderBitmap(BITMAP_NEW_CHECKBOX_OFF, this->m_PosX + 15, this->m_PosY + 228.1, 12, 12, 0.008, 0.429, 0.83, 0.20, 1, 1, 1.0);
	}

	this->RenderBoxItem(this->m_PosX + 15, this->m_PosY + 245, 160, 120);

	this->RenderItensSource();

	this->RenderItensTarget();
	DisableAlphaBlend();
}

void CTradeX::RenderText()
{
	EnableAlphaTest();
	char buffer[256];

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetBgColor(0);

	g_pRenderText->SetTextColor(0xD8, 0xD8, 0xD8, 0xFF);

	g_pRenderText->RenderText(this->m_PosX, this->m_PosY + 11, GlobalText[3172], 190, 0, 3, 0);

	g_pRenderText->SetFont(g_hFont);

	g_pRenderText->SetTextColor(225, 225, 225, 0xFF);

	SIZE pBox;

	g_pRenderText->RenderText(this->m_PosX + 20, this->m_PosY + 45, GlobalText[3173], 190, 0, 1, &pBox);

	g_pRenderText->SetTextColor(0xFF, 0xFF, 0xFF, 0xFF);

	if (this->TradeXTransation == 0)
	{
		sprintf_s(buffer, GlobalText[3174]);
	}
	else if (this->TradeXTransation == 1)
	{
		g_pRenderText->SetTextColor(172, 255, 56, 255);

		sprintf_s(buffer, GlobalText[3175]);
	}
	else if (this->TradeXTransation == 2)
	{
		g_pRenderText->SetTextColor(225, 0, 0, 255);

		sprintf_s(buffer, GlobalText[3176]);
	}

	g_pRenderText->RenderText(this->m_PosX + 95 + pBox.cx, this->m_PosY + 45, buffer, 190, 0, 7, 0);

	DisableAlphaBlend();

	this->RenderTextSource();

	this->RenderTextTarget();
}

void CTradeX::RenderButtons()
{
	DWORD tick = (DWORD)((GetTickCount() - this->TickBlockOk) / 1000);

	this->m_BtnAccept.ChangeTextColor(RGBA(245, 232, 202, 255));
	this->m_BtnAccept.ChangeButtonInfo(this->m_PosX + 20, this->m_PosY + 380, 75, 30);
	this->m_BtnAccept.ChangeText(GlobalText[3186]);

	if (tick < 10)
	{
		this->m_BtnAccept.ForceButtonDisabled(true);
		this->m_BtnAccept.ChangeTextColor(RGBA(255, 84, 84, 200));
		char buffer[20];
		sprintf_s(buffer, GlobalText[3187], this->CountTimeTradeX(tick));
		this->m_BtnAccept.ChangeText(buffer);
	}
	else
	{
		this->m_BtnAccept.ChangeTextColor(RGBA(245, 232, 202, 255));
		this->m_BtnAccept.ChangeText(GlobalText[3186]);
		this->m_BtnAccept.ForceButtonDisabled(false);
	}

	this->m_BtnAccept.Render();
	this->m_BtnCancel.Render();
}

bool CTradeX::Update()
{
	if (IsVisible())
	{
		this->ItemPointer = -1;

		this->ItemPointerTarget = -1;

		this->m_PosY = 0;
		this->m_PosX = 260 + gWideScreen.g_WideWindowWidthAdd;

		ScrollBarPosXTarget = (float)(this->m_PosX + 160);

		ScrollBarPosXSource = (float)(this->m_PosX + 160);
	}

	return true;
}

bool CTradeX::UpdateKeyEvent()
{
	if (g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADEX))
	{
		if (SEASON3B::IsPress(VK_ESCAPE) == true)
		{
			this->TradeXHiddenWindow();
			return false;
		}
	}
	return true;
}

bool CTradeX::UpdateMouseEvent()
{
	if (SEASON3B::IsPress(VK_RBUTTON))
	{
		if (this->MoveItemToInventory())
		{
			MouseRButton = false;
			MouseRButtonPop = false;
			MouseRButtonPush = false;
			return false;
		}

	}

	if (this->m_BtnCancel.UpdateMouseEvent() == true)
	{ //Cancel
		SendRequestTradeXResult(0, 1, 25);
		return false;
	}

	if (this->m_BtnAccept.UpdateMouseEvent() == true)
	{ //aceitar

		if ((DWORD)((GetTickCount() - this->TickBlockOk) / 1000) > 9)
		{
			if (this->TradeXTransation == 0)
			{
				this->m_SourceAcceptTradeX ^= 1;

				SendRequestTradeXResult(this->m_SourceAcceptTradeX, 0, 0);
			}
		}

		return false;
	}


	if (SEASON3B::CheckMouseIn(this->m_PosX + 158, this->m_PosY + 115 + TargetScrollBarPos, 12, 12))
	{
		if (SEASON3B::IsPress(VK_LBUTTON))
		{
			m_EventStateTarget = EVENT_SCROLL_BTN_CLIKED;
		}

		m_EventStateTarget = EVENT_SCROLL_BTN_DOWN;
	}
	else
	{
		m_EventStateTarget = EVENT_NONE;
	}

	if (SEASON3B::CheckMouseIn(this->m_PosX + 158, this->m_PosY + 275 + SourceScrollBarPos, 12, 12))
	{
		m_EventStateSource = EVENT_SCROLL_BTN_DOWN;
	}
	else
	{
		m_EventStateSource = EVENT_NONE;
	}

	if (SEASON3B::CheckMouseIn(this->m_PosX + 156, this->m_PosY + 100, 16, 100))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int iMoveValue = MouseY - m_iMouseYTarget;

			if (iMoveValue < 0)
			{
				if (MouseY <= (this->m_PosY + 110 + TargetScrollBarPos) + 6)
				{
					this->ScrollLinesTarget(0);
				}
			}
			else if (iMoveValue > 0)
			{
				if (MouseY >= (this->m_PosY + 110 + TargetScrollBarPos) + 6)
				{
					this->ScrollLinesTarget(1);
				}
			}

			m_iMouseYTarget = MouseY;

		}
	}

	if (SEASON3B::CheckMouseIn(this->m_PosX + 156, this->m_PosY + 260, 16, 100))
	{
		if (SEASON3B::IsRepeat(VK_LBUTTON))
		{
			int iMoveValue = MouseY - m_iMouseYSource;

			if (iMoveValue < 0)
			{
				if (MouseY <= (this->m_PosY + 270 + SourceScrollBarPos) + 6)
				{
					this->ScrollLinesSource(0);
				}
			}
			else if (iMoveValue > 0)
			{
				if (MouseY >= (this->m_PosY + 270 + SourceScrollBarPos) + 6)
				{
					this->ScrollLinesSource(1);
				}
			}

			m_iMouseYSource = MouseY;

		}
	}

	if (CheckMouseIn(this->m_PosX, this->m_PosY, 190, 429))
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

	return false;
}

float CTradeX::GetLayerDepth()
{
	return 3.4f;
}

void CTradeX::RenderTextSource()
{
	EnableAlphaTest();
	char buffer[256];

	g_pRenderText->SetFont(g_hFont);

	g_pRenderText->SetTextColor(225, 225, 225, 255);

	if (this->m_SourceAcceptTradeX)
	{
		sprintf_s(buffer, GlobalText[3178]);
	}
	else
	{
		sprintf_s(buffer, GlobalText[3179]);
	}

	g_pRenderText->RenderText(this->m_PosX + 31, this->m_PosY + 230, buffer, 190, 0, 1, 0);

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetTextColor(240, 240, 240, 255);

	g_pRenderText->RenderText(this->m_PosX + 20, this->m_PosY + 247, GlobalText[3180], 190, 0, 1, 0);

	sprintf_s(buffer, GlobalText[3181], this->m_TradeItemSource.size());

	g_pRenderText->RenderText(this->m_PosX + 165, this->m_PosY + 247, buffer, 190, 0, 7, 0);
	DisableAlphaBlend();
}

void CTradeX::RenderTextTarget()
{
	EnableAlphaTest();
	char buffer[256];

	g_pRenderText->SetFont(g_hFont);

	g_pRenderText->SetTextColor(225, 225, 225, 255);

	if (this->m_TargetAcceptTradeX)
	{
		sprintf_s(buffer, GlobalText[3182], NameTarget);
	}
	else
	{
		sprintf_s(buffer, GlobalText[3183], NameTarget);
	}

	g_pRenderText->RenderText(this->m_PosX + 31, this->m_PosY + 69, buffer, 190, 0, 1, 0);

	g_pRenderText->SetFont(g_hFontBold);

	g_pRenderText->SetTextColor(240, 240, 240, 255);

	sprintf_s(buffer, GlobalText[3184], NameTarget);

	g_pRenderText->RenderText(this->m_PosX + 20, this->m_PosY + 87, buffer, 190, 0, 1, 0);

	sprintf_s(buffer, GlobalText[3181], this->m_TradeItemTarget.size());

	g_pRenderText->RenderText(this->m_PosX + 165, this->m_PosY + 87, buffer, 190, 0, 7, 0);
	DisableAlphaBlend();
}

void CTradeX::RenderBoxItem(float x, float y, float width, float height)
{
	EnableAlphaTest(1);

	glColor4f(0.0f, 0.0f, 0.0f, 0.3f);

	RenderColor(x, y, width, height, 0.0, 0);

	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);

	RenderColor(x, y, width, height, 0.0, 0);

	glColor4f(0.2f, 0.2f, 0.2f, 0.6f);

	RenderColor(x + 1, y + 1, width - 2, 12.0, 0.0, 0);

	glColor4f(0.54f, 0.422f, 0.2322f, 0.7f);

	RenderColor(x, y, width, 1);

	RenderColor(x, y + 1, 1, height - 2);

	RenderColor(x + width - 1, y + 1, 1, height - 2);

	RenderColor(x, y + height - 1, width, 1);

	EndRenderColor();

	DisableAlphaBlend();

	EnableAlphaTest(1);

}

void CTradeX::RenderItensSource()
{
	float SizeY = 260;

	char buffer[256];

	g_pRenderText->SetFont(g_hFont);

	int LineStart = 0;
	int LineCount = 0;

	if (this->m_TradeItemSource.size() <= 0) {
		gDrawBarItem.ResetItemDrawBar(2);
	}

	for (auto& itemSource : this->m_TradeItemSource) {
		if (LineStart >= SourceScrollBarOffset && LineCount < 10) {
			if (!itemSource.pItemObj) {
				continue;
			}

			if (MouseX >= this->m_PosX + 20 && MouseX <= this->m_PosX + 130) {
				if (MouseY >= this->m_PosY + SizeY && MouseY <= this->m_PosY + (SizeY + 8)) {
					this->ItemPointer = itemSource.slot;
					RenderItemInformation(this->m_PosX, this->m_PosY + 125, itemSource.pItemObj, 2);
				}
			}
			else {
				gDrawBarItem.ResetItemDrawBar(2);
			}

			auto itemAttr = &ItemAttribute[itemSource.pItemObj->Type];

			if (!itemAttr) {
				continue;
			}

			EnableAlphaTest();

			g_pRenderText->SetBgColor(0);

			g_pRenderText->SetFont(g_hFont);

			g_pRenderText->SetTextColor(225, 225, 225, 255);

			if (itemAttr->m_byItemSlot >= 240 && itemAttr->m_byItemSlot <= 248 || itemSource.pItemObj->Jewel_Of_Harmony_OptionLevel > 0 || itemSource.pItemObj->Jewel_Of_Harmony_Option > 0) {
				sprintf_s(buffer, sizeof(buffer), "%s", Visuals.GetItemHybridName(itemSource.pItemObj->Type));
			}
			else {
				if (((itemSource.pItemObj->Level >> 3) & 0xF) == 0) {
					sprintf_s(buffer, sizeof(buffer), "%s", itemAttr->Name);
				}
				else {
					sprintf_s(buffer, sizeof(buffer), "%s + %d", itemAttr->Name, (itemSource.pItemObj->Level >> 3) & 0xF);
				}
			}
			g_pRenderText->RenderText(this->m_PosX + 20, this->m_PosY + SizeY, buffer, 190, 0, 1, 0);

			DisableAlphaBlend();

			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}
}

void CTradeX::RenderItensTarget()
{
	float SizeY = 100;

	char buffer[256];

	g_pRenderText->SetFont(g_hFont);

	int LineStart = 0;
	int LineCount = 0;

	if (this->m_TradeItemTarget.size() <= 0) {
		gDrawBarItem.ResetItemDrawBar(3);
	}

	for (auto& itemTarget : this->m_TradeItemTarget) {
		if (LineStart >= TargetScrollBarOffset && LineCount < 10) {
			if (!itemTarget.pItemObj) {
				continue;
			}

			if (MouseX >= this->m_PosX + 20 && MouseX <= this->m_PosX + 130) {
				if (MouseY >= this->m_PosY + SizeY && MouseY <= this->m_PosY + (SizeY + 8)) {
					this->ItemPointerTarget = itemTarget.slot;
					RenderItemInformation(this->m_PosX, this->m_PosY + 125, itemTarget.pItemObj, 3);
				}
			}
			else {
				gDrawBarItem.ResetItemDrawBar(3);
			}

			auto itemAttr = &ItemAttribute[itemTarget.pItemObj->Type];

			if (!itemAttr) {
				continue;
			}

			EnableAlphaTest();

			g_pRenderText->SetBgColor(0);

			g_pRenderText->SetTextColor(225, 225, 225, 255);

			g_pRenderText->SetFont(g_hFont);

			if (itemAttr->m_byItemSlot >= 240 && itemAttr->m_byItemSlot <= 248 || itemTarget.pItemObj->Jewel_Of_Harmony_OptionLevel > 0 || itemTarget.pItemObj->Jewel_Of_Harmony_Option > 0) {
				sprintf_s(buffer, sizeof(buffer), "%s", Visuals.GetItemHybridName(itemTarget.pItemObj->Type));
			}
			else {
				if (((itemTarget.pItemObj->Level >> 3) & 0xF) == 0) {
					sprintf_s(buffer, sizeof(buffer), "%s", itemAttr->Name);
				}
				else {
					sprintf_s(buffer, sizeof(buffer), "%s + %d", itemAttr->Name, (itemTarget.pItemObj->Level >> 3) & 0xF);
				}
			}
			g_pRenderText->RenderText(this->m_PosX + 20, this->m_PosY + SizeY, buffer, 190, 0, 1, 0);

			DisableAlphaBlend();
			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}
}

void CTradeX::RenderItemInformation(float x, float y, ITEM* pItemObj, int Source)
{
	EnableAlphaTest(1);
	gDrawBarItem.SetItemDrawBar(Source, pItemObj->Type, (int)pItemObj->Level * 8, (int)pItemObj->ExtOption, 0, 1.0);
	RenderItemInfo(x - 30, MouseY, pItemObj, false, 0, false, true);
	DisableAlphaBlend();
}

void CTradeX::RenderItemImage(ITEM* pItemObj)
{
	glMatrixMode(0x1701);
	glPushMatrix();
	glLoadIdentity();

	glViewport2(0, 0, WindowWidth, WindowHeight);

	gluPerspective2(1.f, (float)(WindowWidth) / (float)(WindowHeight), RENDER_ITEMVIEW_NEAR, RENDER_ITEMVIEW_FAR);

	glMatrixMode(0x1700);
	glPushMatrix();
	glLoadIdentity();

	GetOpenGLMatrix(CameraMatrix);
	EnableDepthTest();
	EnableDepthMask();

	auto lpAttr = &ItemAttribute[pItemObj->Type];

	float width = lpAttr->Width;
	float height = lpAttr->Height;

	RenderItem3D((float)155, (float)100, (float)width * 3, (float)height * 4, (int)pItemObj->Type, (int)pItemObj->Level * 8, (int)pItemObj->Option1, (int)pItemObj->ExtOption, 0);

	glMatrixMode(0x1700);
	glPopMatrix();
	glMatrixMode(0x1701);
	glPopMatrix();

	glColor3f(1, 1, 1);
	EnableAlphaTest(false);
}

void CTradeX::TargetInserItens(int slot, BYTE* ItemInfo)
{
	TradeItem info;

	info.pItemObj = g_pNewItemMng->CreateItem(ItemInfo);

	info.slot = slot;

	this->m_TradeItemTarget.push_back(info);

	this->CalcLinesRenderTarget();
}

void CTradeX::SourceInsertItens(int slot, BYTE* ItemInfo)
{
	TradeItem info;

	info.pItemObj = g_pNewItemMng->CreateItem(ItemInfo);

	info.slot = slot;

	this->m_TradeItemSource.push_back(info);

	this->CalcLinesRenderSource();
}

void CTradeX::DelItemTarget(BYTE slot)
{
	std::vector<TradeItem>::iterator li = this->m_TradeItemTarget.begin();

	for (; li != this->m_TradeItemTarget.end(); li++)
	{
		auto pItem = &(*li);

		if (pItem->slot == slot)
		{
			pItem->Clear();
			this->m_TradeItemTarget.erase(li);
			break;
		}
	}
}

void CTradeX::DelItemSource(BYTE slot)
{
	std::vector<TradeItem>::iterator li = this->m_TradeItemSource.begin();

	for (; li != this->m_TradeItemSource.end(); li++)
	{
		auto pItem = &(*li);

		if (pItem->slot == slot)
		{
			pItem->Clear();
			this->m_TradeItemSource.erase(li);
			break;
		}
	}
}

void CTradeX::CalcLinesRenderTarget()
{
	TargetScrollBarOffset = 0;
	TargetScrollBarPos = 0;
	TargetScrollBarMaxOffset = (this->m_TradeItemTarget.size() > 10) ? this->m_TradeItemTarget.size() - 10 : 0;
	TargetScrollBarMultiplier = (float)((float)60 / (float)(this->m_TradeItemTarget.size() - 10));
}

void CTradeX::CalcLinesRenderSource()
{
	SourceScrollBarOffset = 0;
	SourceScrollBarPos = 0;
	SourceScrollBarMaxOffset = (this->m_TradeItemSource.size() > 10) ? this->m_TradeItemSource.size() - 10 : 0;
	SourceScrollBarMultiplier = (float)((float)60 / (float)(this->m_TradeItemSource.size() - 10));
}

void CTradeX::RenderScrollTarget(float x, float y, float width, float height)
{
	//Bar
	EnableAlphaTest(1);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderBitmap(BITMAP_NEW_SCROLL_UP, x, y, width, 3, 0.002, 0.03023, 0.6026349, 0.558239, 1, 1, 1.0);
	RenderBitmap(BITMAP_NEW_SCROLL_MIDDLE2, x, y + 3, width, height, 0.002, 0.03023, 0.6026349, 0.908239, 1, 1, 1.0);
	RenderBitmap(BITMAP_NEW_SCROLL_DOWN, x, y + height, width, 3, 0.002, 0.07023, 0.6026349, 0.558239, 1, 1, 1.0);

	int Line = 0;

	if (m_EventStateTarget == EVENT_SCROLL_BTN_DOWN)
	{
		glColor4f(0.7f, 0.7f, 0.7f, 1.0f);
	}
	else
	{
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

	RenderBitmap(BITMAP_NEW_SCROLL_ON, x - 4, y + TargetScrollBarPos, 12, 12, 0.0, 0.01, 0.6823348, 0.6943348, 1, 1, 1.0);

	DisableAlphaBlend();
}

void CTradeX::RenderScrollSource(float x, float y, float width, float height)
{
	//Bar
	EnableAlphaTest(1);

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	RenderBitmap(BITMAP_NEW_SCROLL_UP, x, y, width, 3, 0.002, 0.03023, 0.6026349, 0.558239, 1, 1, 1.0);
	RenderBitmap(BITMAP_NEW_SCROLL_MIDDLE2, x, y + 3, width, height, 0.002, 0.03023, 0.6026349, 0.908239, 1, 1, 1.0);
	RenderBitmap(BITMAP_NEW_SCROLL_DOWN, x, y + height, width, 3, 0.002, 0.07023, 0.6026349, 0.558239, 1, 1, 1.0);

	RenderBitmap(BITMAP_NEW_SCROLL_ON, x - 4, y + SourceScrollBarPos, 12, 12, 0.0, 0.01, 0.6823348, 0.6943348, 1, 1, 1.0);

	DisableAlphaBlend();
}

void CTradeX::ScrollLinesTarget(int mode)
{
	if (mode == 0)
	{ //Up
		if (TargetScrollBarOffset > 0)
		{
			TargetScrollBarOffset = TargetScrollBarOffset - 1;
			TargetScrollBarPos = TargetScrollBarPos - TargetScrollBarMultiplier;
		}
	}
	else if (mode == 1)
	{ //Down
		if (TargetScrollBarOffset < TargetScrollBarMaxOffset)
		{
			TargetScrollBarOffset = TargetScrollBarOffset + 1;
			TargetScrollBarPos = TargetScrollBarPos + TargetScrollBarMultiplier;
		}
	}
}

void CTradeX::ScrollLinesSource(int mode)
{
	if (mode == 0)
	{ //Up
		if (SourceScrollBarOffset > 0)
		{
			SourceScrollBarOffset = SourceScrollBarOffset - 1;
			SourceScrollBarPos = SourceScrollBarPos - SourceScrollBarMultiplier;
		}
	}
	else if (mode == 1)
	{ //Down
		if (SourceScrollBarOffset < SourceScrollBarMaxOffset)
		{
			SourceScrollBarOffset = SourceScrollBarOffset + 1;
			SourceScrollBarPos = SourceScrollBarPos + SourceScrollBarMultiplier;
		}
	}
}

void CTradeX::MouseWheel(int lpMouse)
{
	//Target
	if (SEASON3B::CheckMouseIn(this->m_PosX + 15, this->m_PosY + 85, 160, 120))
	{
		if (((int)lpMouse) > 0)
		{
			this->ScrollLinesTarget(0);
		}

		if (((int)lpMouse) < 0)
		{
			this->ScrollLinesTarget(1);
		}
		return;
	}

	//Source
	if (SEASON3B::CheckMouseIn(this->m_PosX + 15, this->m_PosY + 245, 160, 120))
	{
		if (((int)lpMouse) > 0)
		{
			this->ScrollLinesSource(0);
		}

		if (((int)lpMouse) < 0)
		{
			this->ScrollLinesSource(1);
		}
		return;
	}
}

void CTradeX::ClearTradeXSource()
{
	std::vector<TradeItem>::iterator li = this->m_TradeItemSource.begin();

	for (; li != m_TradeItemSource.end(); li++)
	{
		auto SourceItem = &(*li);

		if (SourceItem->pItemObj)
		{
			g_pNewItemMng->DeleteItem(SourceItem->pItemObj);
			SourceItem->pItemObj = nullptr;
		}
	}

	m_TradeItemSource.clear();

	this->CalcLinesRenderSource();
}

void CTradeX::ClearTradeXTarget()
{
	std::vector<TradeItem>::iterator li = this->m_TradeItemTarget.begin();

	for (; li != m_TradeItemTarget.end(); li++)
	{
		auto TargetItem = &(*li);

		if (TargetItem->pItemObj)
		{
			g_pNewItemMng->DeleteItem(TargetItem->pItemObj);
			TargetItem->pItemObj = nullptr;
		}
	}

	m_TradeItemTarget.clear();

	this->CalcLinesRenderTarget();
}

int CTradeX::CountTimeTradeX(DWORD tick)
{
	return TimeTradeX - tick;
}

void CTradeX::TradeXHiddenWindow()
{
	PMSG_TRADEX_HIDDEN_WINDOW_RECV pMsg;

	pMsg.header.set(0xF3, 0xC1, sizeof(pMsg));

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CTradeX::RenderIconTradeExtended()
{
	if (this->TradeXHidden)
		this->m_BtnShow.Render();
}

bool CTradeX::UpdateMouseEventTradeExtended()
{
	if (this->TradeXHidden)
	{
		if (this->m_BtnShow.UpdateMouseEvent() == true)
		{
			this->TradeXOpenWindow();

			this->ClickEvent = GetTickCount();

			return true;
		}
	}

	return false;
}

void CTradeX::TradeXOpenWindow()
{
	PMSG_TRADEX_OPEN_WINDOW_SEND pMsg;

	pMsg.header.set(0xF3, 0xC0, sizeof(pMsg));

	DataSend((BYTE*)&pMsg, pMsg.header.size);
}

void CTradeX::GCTradeResponseRecv(PMSG_TRADE_RESPONSE_RECV* lpMsg)
{
	if (lpMsg->TradeX)
	{
		std::memcpy(this->NameTarget, lpMsg->name, sizeof(this->NameTarget));

		this->ClearTradeXSource();
		this->ClearTradeXTarget();

		g_pNewUISystem->Show(SEASON3B::INTERFACE_TRADEX);

		this->m_TargetAcceptTradeX = 0;
		this->m_SourceAcceptTradeX = 0;
		this->TradeXHidden = 0;
		this->m_isVisible = 1;
	}
	else
	{
		this->m_isVisible = 0;
		gDrawBarItem.ResetItemDrawBar(3);
		gDrawBarItem.ResetItemDrawBar(2);
	}
}

void CTradeX::TradeXItemAddRecv(PMSG_TRADEX_ITEM_ADD_RECV* lpMsg)
{
	this->TargetInserItens(lpMsg->slot, lpMsg->ItemInfo);
	this->CalcLinesRenderTarget();
}

void CTradeX::TradeXStateRecv(PMSG_TRADEX_STATE_RECV* lpMsg)
{
	g_ConsoleDebug->Write(4, "TradeX State: %d", lpMsg->state);

	switch (lpMsg->state)
	{
	case 1:
		this->TradeXHidden = 1;
		this->TradeXTransation = lpMsg->Transation;
		this->m_isVisible = 0;
		gDrawBarItem.ResetItemDrawBar(3);
		gDrawBarItem.ResetItemDrawBar(2);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_TRADEX);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY);
		break;
	case 2:
		this->TradeXHidden = 0;
		this->m_isVisible = 0;
		this->TradeXTransation = lpMsg->Transation;
		gDrawBarItem.ResetItemDrawBar(3);
		gDrawBarItem.ResetItemDrawBar(2);
		std::memset(this->NameTarget, 0, sizeof(this->NameTarget));

		this->ClearTradeXSource();
		this->CalcLinesRenderSource();
		this->ClearTradeXTarget();
		this->CalcLinesRenderTarget();

		g_pNewUISystem->Hide(SEASON3B::INTERFACE_TRADEX);
		g_pNewUISystem->Hide(SEASON3B::INTERFACE_INVENTORY);
		break;
	case 3:
		this->ClearTradeXSource();
		this->CalcLinesRenderSource();
		this->ClearTradeXTarget();
		this->CalcLinesRenderTarget();

		TradeXTransation = 1;
		break;
	case 4:
		this->ClearTradeXTarget();
		this->CalcLinesRenderTarget();

		TradeXTransation = 2;
		break;
	case 5:
		this->TradeXTransation = 0;
		this->m_TargetAcceptTradeX = 0;
		this->TimeTradeX = 10;
		break;
	}
}

void CTradeX::TradeXListRecv(PMSG_TRADEX_ITEM_LIST_RECV* lpMsg)
{
	if (lpMsg->count <= 0)
	{
		return;
	}

	for (int n = 0; n < lpMsg->count; n++)
	{
		auto lpInfo = (PMSG_TRADEX_ITEM_RECV*)(((BYTE*)lpMsg) + sizeof(PMSG_TRADEX_ITEM_LIST_RECV) + (sizeof(PMSG_TRADEX_ITEM_RECV) * n));

		if (lpMsg->target == 1)
		{
			this->TargetInserItens(lpInfo->slot, lpInfo->ItemInfo);
		}
		else
		{
			this->SourceInsertItens(lpInfo->slot, lpInfo->ItemInfo);
		}
	}

	if (lpMsg->target == 1)
	{
		this->CalcLinesRenderTarget();
	}
	else
	{
		this->CalcLinesRenderSource();
	}
}

void CTradeX::TradeXDelItemTarget(PMSG_TRADEX_ITEM_DEL_RECV* lpMsg)
{
	this->DelItemTarget(lpMsg->slot);

	this->CalcLinesRenderTarget();
}

void CTradeX::TradeXOkButtonRecv(PMSG_TRADE_OK_BUTTON_RECV* lpMsg)
{
	if (lpMsg->tradeX == 1)
	{
		if (lpMsg->flag == 0)
		{
			this->m_TargetAcceptTradeX = 0;

			this->TimeTradeX = 10;
			this->TickBlockOk = GetTickCount();
		}
		else if (lpMsg->flag == 1)
		{
			this->m_TargetAcceptTradeX = 1;
		}
		else if (lpMsg->flag == 2)
		{
			this->m_SourceAcceptTradeX = 0;

			this->TimeTradeX = 10;
			this->TickBlockOk = GetTickCount();
		}
		else if (lpMsg->flag == 3)
		{
			this->m_SourceAcceptTradeX = 1;
		}
	}
}

void CTradeX::TradeXResultRecv(PMSG_TRADE_RESULT_RECV* lpMsg)
{
	if (lpMsg->tradeX == 1)
	{
		if (lpMsg->result == 1)
		{
			this->ClearTradeXSource();

			this->ClearTradeXTarget();

			this->TradeXTransation = 1;
		}
	}
}

bool CTradeX::CheckTradeXSourceEmptySlot()
{
	if (this->m_TradeItemSource.size() < 120)
	{
		return 1;
	}

	return 0;
}

void CTradeX::ProcessToReceiveTradeXItemsSource(int nIndex, BYTE* pbyItemPacket)
{
	SEASON3B::CNewUIInventoryCtrl::DeletePickedItem();

	if (nIndex >= 0 && nIndex < 120)
	{
		if (IsItemAutoMove())
		{
			SEASON3B::CNewUIInventoryCtrl* pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();

			ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(m_nBackupMouseX, m_nBackupMouseY);

			g_pMyInventory->GetInventoryCtrl()->RemoveItem(pItemObj);

			SetItemAutoMove(false);

			this->SourceInsertItens(nIndex, pbyItemPacket);

			this->CalcLinesRenderSource();
		}
	}
}

void CTradeX::ProcessToRemoveTradeXItemsSource(int nIndex, BYTE* pbyItemPacket)
{
	g_pMyInventory->InsertItem(this->m_TargetSlotBackup, pbyItemPacket);

	this->DelItemSource(nIndex);

	this->CalcLinesRenderSource();

	SetItemAutoMove(false);
}

void CTradeX::MoveItemFromInventory()
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

	if (this->CheckTradeXSourceEmptySlot() == 0)
	{
		return;
	}

	SEASON3B::CNewUIInventoryCtrl* pMyInvenCtrl = g_pMyInventory->GetInventoryCtrl();

	ITEM* pItemObj = pMyInvenCtrl->FindItemAtPt(MouseX, MouseY);


	if (pItemObj)
	{
		if (::IsTradeBan(pItemObj))
		{
			g_pChatListBox->AddText("", GlobalText[494], SEASON3B::TYPE_ERROR_MESSAGE);
			return;
		}

		SetItemAutoMove(true);

		int nSrcIndex = pItemObj->y * pMyInvenCtrl->GetNumberOfColumn() + pItemObj->x;

		SendRequestEquipmentItem(REQUEST_EQUIPMENT_INVENTORY, MAX_EQUIPMENT_INDEX + nSrcIndex, pItemObj, REQUEST_EQUIPMENT_TRADEX, -1);

		::PlayBuffer(SOUND_GET_ITEM01);
	}
}

void CTradeX::ProcessItemAutoMoveFailure()
{
	if (!g_pNewUISystem->IsVisible(SEASON3B::INTERFACE_TRADEX))
	{
		return;
	}

	SetItemAutoMove(false);
}

bool CTradeX::MoveItemToInventory()
{
	if (!this->CheckMouse())
	{
		return false;
	}

	float SizeY = 260;
	int LineStart = 0;
	int LineCount = 0;

	for (auto& itemSource : this->m_TradeItemSource)
	{
		if (LineStart >= SourceScrollBarOffset && LineCount < 10)
		{
			if (!itemSource.pItemObj)
			{
				continue;
			}

			if (MouseX >= this->m_PosX + 20 && MouseX <= this->m_PosX + 130)
			{
				if (MouseY >= this->m_PosY + SizeY && MouseY <= this->m_PosY + (SizeY + 8))
				{
					auto itemAttr = &ItemAttribute[itemSource.pItemObj->Type];

					if (itemAttr)
					{
						int nDstIndex = g_pMyInventory->FindEmptySlot(itemSource.pItemObj);

						if (-1 != nDstIndex)
						{
							SetItemAutoMove(true);

							m_TargetSlotBackup = nDstIndex;

							SendRequestEquipmentItem(REQUEST_EQUIPMENT_TRADEX, itemSource.slot, itemSource.pItemObj, REQUEST_EQUIPMENT_INVENTORY, MAX_EQUIPMENT_INDEX + nDstIndex);

							::PlayBuffer(SOUND_GET_ITEM01);

							return true;
						}
					}
				}
			}

			SizeY += 10;
			LineCount++;
		}

		LineStart++;
	}

	return false;
}

void CTradeX::NewDeleteItemTradeX(ITEM* item) {
	g_pNewItemMng->DeleteItem(item);
}