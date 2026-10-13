#include "stdafx.h"
#include "AutoPotion.h"
#include "NewUISystem.h"
#include "wsclientinline.h"
#include "zzzinterface.h"
#include "zzzinventory.h"
#include "NewUIMainFrameWindow.h"


CAutoPotion gAutoPotion;

CAutoPotion::CAutoPotion()
{
    this->m_Potion = false;
    this->m_Thread = NULL;
}

CAutoPotion::~CAutoPotion()
{
    if (this->m_Thread != NULL)
    {
        TerminateThread(this->m_Thread, 0);
        CloseHandle(this->m_Thread);
        this->m_Thread = NULL;
    }
}

void CAutoPotion::CreateAutoPotion()
{
    if (this->m_Thread == NULL)
    {
        this->m_Thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)this->AutoPotionWorker, this, 0, 0);
    }
}

void CAutoPotion::Toggle()
{
    this->m_Potion = !this->m_Potion;

    if (g_pChatListBox != NULL)
    {
        char Text[256];
        sprintf(Text, "Auto Potion %s", this->m_Potion ? "Ativado" : "Desativado");
        g_pChatListBox->AddText("", Text, SEASON3B::TYPE_SYSTEM_MESSAGE, this->m_Potion ? 2 : 4);
    }
}

bool CAutoPotion::IsEnabled() const
{
    return this->m_Potion;
}
DWORD WINAPI CAutoPotion::AutoPotionWorker(CAutoPotion* AutoPotion)
{
    bool f6Last = false;
    bool lastf6Last = false;

    while (1)
    {
        Sleep(10);

        if (GetForegroundWindow() != g_hWnd)
            continue;

        if (g_hWnd == NULL ||
            g_pChatListBox == NULL ||
            g_pMyInventory == NULL)
        {
            continue;
        }
        if (AutoPotion->m_Potion &&
            (g_pNewUISystem->IsVisible(7) || g_pNewUISystem->IsVisible(8)))
        {
            AutoPotion->m_Potion = false;
            char Text[256];
            sprintf(Text, "Auto Potion Desativado (interface visível)");
            g_pChatListBox->AddText("", Text, SEASON3B::TYPE_SYSTEM_MESSAGE);
            lastf6Last = false;
        }

        bool f6 = (GetAsyncKeyState(VK_F6) & 0x8000) != 0;

        if (f6 && !f6Last)
        {
            AutoPotion->Toggle();

            lastf6Last = AutoPotion->m_Potion;
        }


        f6Last = f6;

        if (AutoPotion->m_Potion)
        {
            AutoPotion->UseItem(1);
            Sleep(5);
            AutoPotion->UseItem(2);
            Sleep(5);
            AutoPotion->UseItem(3);
            Sleep(5);
        }
    }

    return 0;
}

void CAutoPotion::UseItem(int itemType)
{
    if (g_pMyInventory == NULL)
    {
        return;
    }
    int iIndex = -1;

    if (itemType == 1) {
        SEASON3B::CNewUIItemHotKey hotKeyInstance;
        iIndex = hotKeyInstance.GetHotKeyItemIndex(SEASON3B::HOTKEY_Q);
    }
    else if (itemType == 2) {
        SEASON3B::CNewUIItemHotKey hotKeyInstance;
        iIndex = hotKeyInstance.GetHotKeyItemIndex(SEASON3B::HOTKEY_W);
    }
    else if (itemType == 3) {
        SEASON3B::CNewUIItemHotKey hotKeyInstance;
        iIndex = hotKeyInstance.GetHotKeyItemIndex(SEASON3B::HOTKEY_E);
    }

    if (iIndex != -1)
    {
        ITEM* pItem = NULL;
        pItem = g_pMyInventory->FindItem(iIndex);
        if (pItem == NULL)
        {
            return;
        }
        if ((pItem->Type >= ITEM_POTION + 78 && pItem->Type <= ITEM_POTION + 82))
        {
            std::list<eBuffState> secretPotionbufflist;
            secretPotionbufflist.push_back(eBuff_SecretPotion1);
            secretPotionbufflist.push_back(eBuff_SecretPotion2);
            secretPotionbufflist.push_back(eBuff_SecretPotion3);
            secretPotionbufflist.push_back(eBuff_SecretPotion4);
            secretPotionbufflist.push_back(eBuff_SecretPotion5);

            if (g_isCharacterBufflist((&Hero->Object), secretPotionbufflist) != eBuffNone) {
                SEASON3B::CreateOkMessageBox(GlobalText[2530], RGBA(255, 30, 0, 255));
            }
            else {
                SendRequestUse(iIndex, 0);
            }
        }
        else

        {
            SendRequestUse(iIndex, 0);
        }
    }
}