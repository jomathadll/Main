#pragma once

#include "NewUIBase.h"
#include "NewUIMessageBox.h"
#include "NewUIMyInventory.h"
#include "NewUIMyQuestInfoWindow.h"
#include "NewUIStorageInventory.h"
#if CLIENT_VERSION >= 1001
#include "VoidMuButtons.h"
#endif

namespace SEASON3B
{
	class CNewUITradeX : public CNewUIObj
	{
	public:
		enum IMAGE_LIST
		{
			IMAGE_TRADE_LINE = CNewUIMyQuestInfoWindow::IMAGE_MYQUEST_LINE,
			IMAGE_TRADE_NICK_BACK = BITMAP_INTERFACE_NEW_TRADE_BEGIN,
			IMAGE_TRADE_MONEY = CNewUIMyInventory::IMAGE_INVENTORY_MONEY,
			IMAGE_TRADE_CONFIRM = BITMAP_INTERFACE_NEW_TRADE_BEGIN + 1,
			IMAGE_TRADE_WARNING_ARROW = BITMAP_CURSOR + 7,

			IMAGE_TRADE_BTN_CLOSE = CNewUIMyInventory::IMAGE_INVENTORY_EXIT_BTN,
			IMAGE_TRADE_BTN_ZEN_INPUT = CNewUIStorageInventory::IMAGE_STORAGE_BTN_INSERT_ZEN,
#if CLIENT_VERSION >= 1001
			IMAGE_TRADE_BACK_VOID = BITMAP_NEW_DEFAULT_WINDOW_BACK,
#endif
		};

	private:
		enum
		{
			TRADE_WIDTH = 190,
			TRADE_HEIGHT = 429,
			CONFIRM_WIDTH = 36,
			CONFIRM_HEIGHT = 29,
			MAX_TRADE_INVEN = 120,
		};

		enum TRADE_BUTTON
		{
			BTN_CLOSE = 0,			// Ã¢ ´Ý±â.
			BTN_ZEN_INPUT,			// Á¨ ÀÔ·Â.
			MAX_BTN
		};

		class TRADEX_ITENS {
		public:
			TRADEX_ITENS() {

			};

			~TRADEX_ITENS() {

			};

			void Clear() {
				this->slot = -1;

				if (this->pItemObj) {
					NewDeleteItem(this->pItemObj);
					this->pItemObj = nullptr;
				}
			}


			ITEM* pItemObj;
			BYTE slot;
		};

		CNewUIManager* m_pNewUIMng;
		POINT					m_Pos;
		CVoidMuButtons		m_abtn[MAX_BTN];


		char					m_szYourID[MAX_ID_SIZE + 1];
		int						m_nYourLevel;
		int						m_nYourGuildType;
		int						m_nYourTradeGold;
		int						m_nMyTradeGold;
		int						m_nTempMyTradeGold;
		bool					m_bYourConfirm;
		bool					m_bMyConfirm;
		int						m_nMyTradeWait;
		bool					m_bTradeAlert;

	public:
		std::vector<TRADEX_ITENS> m_MyItens;
		std::vector<TRADEX_ITENS> m_YourItens;

	public:
		CNewUITradeX();
		virtual ~CNewUITradeX();

		bool Create(CNewUIManager* pNewUIMng, int x, int y);
		void Release();

		void SetPos(int x, int y);
		const POINT& GetPos() const;

		bool UpdateMouseEvent();
		bool UpdateKeyEvent();
		bool Update();
		bool Render();

		static void NewDeleteItem(ITEM* item);

		float GetLayerDepth();	//. 2.1f

		static void UI2DEffectCallback(LPVOID pClass, DWORD dwParamA, DWORD dwParamB);

		void ProcessCloseBtn();
		void ProcessClosing();

		void GetYourID(char* pszYourID);
		void SetYourTradeGold(int nGold) { m_nYourTradeGold = nGold; }

		void SendRequestMyGoldInput(int nInputGold);
		void SendRequestItemToMyInven(ITEM* pItemObj,
			int nTradeIndex, int nInvenIndex);

		void ProcessToReceiveTradeRequest(BYTE* pbyYourID);
		void ProcessToReceiveTradeResult(LPPTRADE pTradeData);
		void ProcessToReceiveYourItemDelete(BYTE byYourInvenIndex);
		void ProcessToReceiveYourItemAdd(BYTE byYourInvenIndex, BYTE* pbyItemPacket);
		void ProcessToReceiveMyTradeGold(BYTE bySuccess);
		void ProcessToReceiveYourConfirm(BYTE byState);
		void ProcessToReceiveTradeExit(BYTE byState);
		void ProcessToReceiveTradeItems(int nIndex, BYTE* pbyItemPacket);

		void AlertTrade();

		int GetPointedItemIndexMyInven();
		int GetPointedItemIndexYourInven();

	private:
		void LoadImages();
		void UnloadImages();

		void RenderBackImage();
		void RenderText();

		void ProcessMyInvenCtrl();
		bool ProcessBtns();

		void InitTradeInfo();
		void BackUpYourInven(int nYourInvenIndex);
		void BackUpYourInven(ITEM* pYourItemObj);
		void AlertYourTradeInven();

		void SendRequestItemToTrade(ITEM* pItemObj, int nInvenIndex, int nTradeIndex);
	};
}

