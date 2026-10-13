//////////////////////////////////////////////////////////////////////
// NewUICommon.h: interface for the CNewUICommon class.
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWUICOMMON_H__0668BCBC_7537_454B_82FD_9D6BBBBDBA84__INCLUDED_)
#define AFX_NEWUICOMMON_H__0668BCBC_7537_454B_82FD_9D6BBBBDBA84__INCLUDED_

#pragma once

namespace SEASON3B
{
	bool CreateOkMessageBox(const unicode::t_string& strMsg, DWORD dwColor = 0xffffffff, float fPriority = 3.f);

	int IsPurchaseShop(); 
	#define g_IsPurchaseShop SEASON3B::IsPurchaseShop()

	bool CheckMouseIn(int x, int y, int width, int height);
	
	void RenderImage(GLuint uiImageType, float x, float y, float width, float height);
	void RenderImageScale(GLuint uiImageType, float x, float y, float width, float height, float su, float sv, float uw, float vh, float alpha = 0.0f);
	void RenderImageScaleAuto(GLuint uiImageType, float x, float y, float width, float height);
	void RenderImage(GLuint uiImageType, float x, float y, float width, float height, float su, float sv);
	void RenderImage(GLuint uiImageType, float x, float y, float width, float height, float su, float sv, DWORD color);
	void RenderImage(GLuint uiImageType, float x, float y, float width, float height, float su, float sv,float uw, float vh, DWORD color = RGBA(255,255,255,255));
	void RenderImageAnimation(GLuint uiImageType, float x, float y, float width, float height, int framesX, int framesY, float speed = 1.0f, float alpha = 0.0f, bool reverse = false, int framesInvalid = -1);
	
	float RenderNumber(float x, float y, int iNum, float fScale = 1.0f);
#if CLIENT_VERSION >= 1001
	float RenderOldNumber(float x, float y, int iNum, float fScale = 1.0f);
#endif
	
	bool IsNone(int iVirtKey);
	bool IsRelease(int iVirtKey);
	bool IsPress(int iVirtKey);
	bool IsRepeat(int iVirtKey);
	
	class CNewKeyInput
	{
		struct INPUTSTATEINFO 
		{
			BYTE byKeyState;
		} m_pInputInfo[256];

#ifndef ASG_FIX_ACTIVATE_APP_INPUT
		void Init();
#endif

	public:
		enum KEY_STATE 
		{ 
			KEY_NONE=0,			
			KEY_RELEASE,		
			KEY_PRESS,			
			KEY_REPEAT			
		};
		~CNewKeyInput();
		
		static CNewKeyInput* GetInstance();
#ifdef ASG_FIX_ACTIVATE_APP_INPUT
		void Init();
#endif	
		void ScanAsyncKeyState();

		bool IsNone(int iVirtKey);
		bool IsRelease(int iVirtKey);
		bool IsPress(int iVirtKey); 
		bool IsRepeat(int iVirtKey);
		void SetKeyState(int iVirtKey, KEY_STATE KeyState);
		

	protected:
		CNewKeyInput();
	};
}

#define g_pNewKeyInput	SEASON3B::CNewKeyInput::GetInstance()

#endif // !defined(AFX_NEWUICOMMON_H__0668BCBC_7537_454B_82FD_9D6BBBBDBA84__INCLUDED_)
