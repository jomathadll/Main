#pragma once
#include "NewUIButton.h"

namespace SEASON3B
{
	class CCustomButtons : public CNewUIBaseButton
	{
	public:
		CCustomButtons();
		virtual ~CCustomButtons();
		void ChangeButtonImgState(bool imgregister, int imgindex, bool overflg = false, bool isimgwidth = false, bool bClickEffect = false);
		void ChangeButtonInfo(int x, int y, int sx, int sy);

	private:
		void Initialize();
		void Destroy();

	public:
		void RegisterButtonState(BUTTON_STATE eventstate, int imgindex, int btstate);
		void UnRegisterButtonState();

	public:
		void ChangeImgColor(BUTTON_STATE eventstate, unsigned int color);
		void ChangeText(unicode::t_string btname);
		void SetFont(HFONT hFont);

		void ChangeButtonState(BUTTON_STATE eventstate, int iButtonState);
		void MoveTextPos(int iX, int iY);

		void ChangeTextBackColor(const DWORD bcolor);
		void ChangeTextColor(const DWORD color);

		void ChangeToolTipText(unicode::t_string tooltiptext, bool istoppos = false);
		void ChangeToolTipTextColor(const DWORD color);
		void SetToolTipFont(HFONT hFont);

		void ChangeImgWidth(bool isimgwidth);
		void ChangeImgIndex(int imgindex, int curimgstate = 0);
		void ChangeAlpha(unsigned char fAlpha, bool isfontalph = true);
		void ChangeAlpha(float fAlpha, bool isfontalph = true);

		void ForceButtonDown(bool state);
		void ForceButtonDisabled(bool state);

		void SetButtonSize(int size);
		int GetButtonSize();

	public:
		bool UpdateMouseEvent();

	public:
		bool Render(bool RendOption = false);

	private:
		void ChangeFrame();

	private:
		ButtonStateMap           m_ButtonInfo;

	private:
		unicode::t_string		m_Name;
		unicode::t_string		m_TooltipText;

		HFONT					m_hTextFont;
		HFONT					m_hToolTipFont;
		DWORD					m_NameColor;
		DWORD					m_NameBackColor;
		DWORD					m_TooltipTextColor;

		int						m_BtnSize;

		bool					m_BtnForceDisabled;
		bool					m_BtnForceDown;

		int						m_CurImgIndex;
		int						m_CurImgState;

		WORD					m_ImgWidth;
		WORD					m_ImgHeight;

		unsigned int			m_CurImgColor;
		bool					m_IsTopPos;
		bool                    m_IsImgWidth;

		unsigned char			m_fAlpha;

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		bool					m_bClickEffect;
		int						m_iMoveTextPosX;
		int						m_iMoveTextPosY;
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
	};

	inline
		void CCustomButtons::ChangeImgWidth(bool isimgwidth)
	{
		m_IsImgWidth = isimgwidth;
	}

	inline
		void CCustomButtons::ChangeText(unicode::t_string btname)
	{
		m_Name = btname;
	}

	inline
		void CCustomButtons::SetFont(HFONT hFont)
	{
		m_hTextFont = hFont;
	}

	inline
		void CCustomButtons::ChangeTextBackColor(const DWORD bcolor)
	{
		m_NameBackColor = bcolor;
	}

	inline
		void CCustomButtons::ChangeTextColor(const DWORD color)
	{
		m_NameColor = color;
	}

	inline
		void CCustomButtons::ChangeToolTipText(unicode::t_string tooltiptext, bool istoppos)
	{
		m_TooltipText = tooltiptext;
		m_IsTopPos = istoppos;
		//m_hToolTipFont = g_hFont;
	}

	inline
		void CCustomButtons::SetToolTipFont(HFONT hFont)
	{
		m_hToolTipFont = hFont;
	}

	inline
		void CCustomButtons::ChangeToolTipTextColor(const DWORD color)
	{
		m_TooltipTextColor = color;
	}



	class CVoidMuRadioButton : public CNewUIBaseButton
	{
	public:
		CVoidMuRadioButton();
		virtual ~CVoidMuRadioButton();

	public:
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		void ChangeRadioButtonImgState(int imgindex, bool isDown = false, bool bClickEffect = false);
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
		void ChangeRadioButtonImgState(int imgindex, bool isDown = false);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
		void ChangeRadioButtonInfo(int x, int y, int sx, int sy);
		void ChangeFrame(BUTTON_STATE eventstate);

	public:
		void ChangeImgColor(BUTTON_STATE eventstate, unsigned int color);
		void ChangeText(unicode::t_string btname);
		void ChangeTextBackColor(const DWORD bcolor);
		void ChangeTextColor(const DWORD color);
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		void ChangeButtonState(BUTTON_STATE eventstate, int iButtonState);
		void ChangeButtonState(int iImgIndex, BUTTON_STATE eventstate, int iButtonState);
		void SetFont(HFONT hFont);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

	public:
		void RegisterButtonState(BUTTON_STATE eventstate, int imgindex, int btstate);
		void UnRegisterButtonState();
		void SetButtonSize(int size);
		int GetButtonSize();

	public:
		bool UpdateMouseEvent(bool isGroupevent = false);

	public:
		bool Render();

	private:
		void ChangeImgIndex(int imgindex, int curimgstate = 0);
		void ChangeFrame();
		void Initialize();
		void Destroy();

	private:
		ButtonStateMap           m_RadioButtonInfo;
		unicode::t_string		 m_Name;

		int						m_BtnSize;
		bool					m_BtnForceDisabled;
		bool					m_BtnForceDown;

		DWORD					m_NameColor;
		DWORD					m_NameBackColor;
		DWORD					m_CurImgIndex;
		DWORD					m_CurImgState;
		DWORD					m_ImgWidth;
		DWORD					m_ImgHeight;
		DWORD					m_CurImgColor;
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		HFONT					m_hTextFont;
		bool					m_bClickEffect;
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
#ifdef KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
		bool					m_bLockImage;
#endif // KJH_MOD_RADIOBTN_MOUSE_OVER_IMAGE
	};

	inline
		void CVoidMuRadioButton::ChangeText(unicode::t_string btname)
	{
		m_Name = btname;
	}

	inline
		void CVoidMuRadioButton::ChangeTextBackColor(const DWORD bcolor)
	{
		m_NameBackColor = bcolor;
	}

	inline
		void CVoidMuRadioButton::ChangeTextColor(const DWORD color)
	{
		m_NameColor = color;
	}

#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
	inline
		void CVoidMuRadioButton::SetFont(HFONT hFont)
	{
		m_hTextFont = hFont;
	}
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

	class CVoidMuRadioGroupButton
	{
	public:
		CVoidMuRadioGroupButton();
		virtual ~CVoidMuRadioGroupButton();

	public:
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		void CreateRadioGroup(int radiocount, int imgindex, bool bClickEffect = false);
		void ChangeRadioButtonInfo(bool iswidth, int x, int y, int sx, int sy, int iDistance = 1);
		void MoveTextPos(int iX, int iY);
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
		void CreateRadioGroup(int radiocount, int imgindex);
		void ChangeRadioButtonInfo(bool iswidth, int x, int y, int sx, int sy);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
		void ChangeRadioText(std::list<unicode::t_string>& textlist);
		void ChangeFrame(int buttonIndex);
		void LockButtonindex(int buttonIndex);
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		void SetFont(HFONT hFont, int iButtonIndex);
		void SetFont(HFONT hFont);
		void ChangeButtonState(BUTTON_STATE eventstate, int iButtonState);
		void ChangeButtonState(int iBtnIndex, int iImgIndex,
			BUTTON_STATE eventstate, int iButtonState);
		void SetButtonSize(int size);
		POINT GetPos(int iButtonIndex);
#else // KJH_ADD_INGAMESHOP_UI_SYSTEM
		void SetFontIndex(int buttonIndex, HFONT hFont);
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM

	public:
		void RegisterRadioButton(CVoidMuRadioButton* button);
		void UnRegisterRadioButton();

	public:
		const int GetCurButtonIndex();

	public:
		int UpdateMouseEvent();

	public:
		bool Render();

	private:
		void SetCurButtonIndex(int index);
		void Initialize();
		void Destroy();

	private:
		typedef std::list<CVoidMuRadioButton*>      RadioButtonList;

	private:
		RadioButtonList				m_RadioList;
		DWORD						m_CurButtonIndex;
#ifdef KJH_ADD_INGAMESHOP_UI_SYSTEM
		int							m_iButtonDistance;			// 버튼과 버튼사이의 간격
#endif // KJH_ADD_INGAMESHOP_UI_SYSTEM
	};

	inline
		void CVoidMuRadioGroupButton::SetCurButtonIndex(int index)
	{
		m_CurButtonIndex = index;
	}

	inline
		const int CVoidMuRadioGroupButton::GetCurButtonIndex()
	{
		return m_CurButtonIndex;
	}

};