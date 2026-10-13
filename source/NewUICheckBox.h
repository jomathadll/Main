#pragma once
namespace SEASON3B
{
	class CNewUICheckBox
	{
	public:
		CNewUICheckBox();
		virtual ~CNewUICheckBox();
		void CheckBoxImgState(int imgindex);
		void RegisterBoxState(bool eventstate);
		void ChangeText(unicode::t_string btname);
		void CheckBoxInfo(int x, int y, int sx, int sy);
		bool GetBoxState();

		void Render();
		bool UpdateMouseEvent();
	private:
		int						s_ImgIndex;
		POINT					m_Pos;
		POINT					m_Size;
		unicode::t_string		m_Name;
		HFONT					m_hTextFont;
		DWORD					m_NameColor;
		DWORD					m_NameBackColor;
		float					m_ImgWidth;
		float					m_ImgHeight;
		bool					State;
	};
};