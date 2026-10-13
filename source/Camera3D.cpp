// Camera.cpp: implementation of the CCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Camera3D.h"
#include "ZzzLodTerrain.h"
#include "NewUISystem.h"

CCamera gCamera;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction Dakosmu
//////////////////////////////////////////////////////////////////////

CCamera::CCamera() // OK
{
	this->m_CursorX = MouseX;
	this->m_CursorY = MouseY;
	this->m_WheelDelta = 0;
}

CCamera::~CCamera() // OK
{

}


void CCamera::Toggle() // OK
{
	m_CameraOnOff ^= 1;

	if (m_CameraOnOff == 0)
	{
		this->Restore();
	}

	if (g_pChatListBox == NULL)
	{
		return;
	}

	if (m_CameraOnOff != 0)
	{
		g_pChatListBox->AddText("", "Camera 3D ativada.", SEASON3B::TYPE_SYSTEM_MESSAGE);
	}
	else
	{
		g_pChatListBox->AddText("", "Camera 3D desativada.", SEASON3B::TYPE_SYSTEM_MESSAGE);
	}
}

void CCamera::Restore() // OK
{
	this->m_WheelDelta = 0;
	CameraZoom = 0;
	CameraAngle[2] = -45.f;
	AngleY3D = 0;
	AngleZ3D = 0;
}

void CCamera::OnMouseWheel(int delta) // OK
{
	if (SceneFlag != MAIN_SCENE)
	{
		return;
	}

	if (m_CameraOnOff == 0)
	{
		return;
	}

	this->m_WheelDelta += delta;
}

void CCamera::Update() // OK
{
	if (SceneFlag != MAIN_SCENE) return;

	if (m_CameraOnOff == 0)
	{
		this->m_WheelDelta = 0;
		this->m_CursorX = MouseX;
		this->m_CursorY = MouseY;
		return;
	}

	if (HIBYTE(GetAsyncKeyState(VK_MBUTTON)) == 128)
	{
		if (this->m_CursorX < MouseX)
		{
			CameraAngle[2] -= 4;
		}

		if (this->m_CursorX > MouseX)
		{
			CameraAngle[2] += 4;
		}
	}

	if (HIBYTE(GetAsyncKeyState(VK_RIGHT)) == 128)
		CameraAngle[2] -= 4;

	if (HIBYTE(GetAsyncKeyState(VK_LEFT)) == 128)
		CameraAngle[2] += 4;

	if (HIBYTE(GetAsyncKeyState(VK_DOWN)) == 128)
		if (CameraZoom < 30)
		{
			CameraZoom += 1;
			if (AngleY3D < 5)
			{
				CameraZoom += 3;
				AngleY3D += 1.5f;
				AngleZ3D += 27.0f;
			}
		}

	if (HIBYTE(GetAsyncKeyState(VK_UP)) == 128)
	{
		if (CameraZoom > -19)
		{
			CameraZoom -= 1;
			if (AngleY3D > -25)
			{
				AngleY3D -= 1.5f;
				AngleZ3D -= 28.0f;
			}
		}
	}

	int wheelDelta = this->m_WheelDelta;
	this->m_WheelDelta = 0;

	if (wheelDelta == 0)
	{
		wheelDelta = MouseWheel;
	}

	if (wheelDelta > 0)
	{
		if (CameraZoom > -19)
		{
			CameraZoom -= 1;
			if (AngleY3D > -25)
			{
				AngleY3D -= 1.5f;
				AngleZ3D -= 27.0f;
			}
		}
	}
	else if (wheelDelta < 0)
	{
		if (CameraZoom < 30)
		{
			CameraZoom += 1;
			if (AngleY3D < 5)
			{
				CameraZoom += 3;
				AngleY3D += 1.5f;
				AngleZ3D += 27.0f;
			}
		}
	}

	if (MouseWheel != 0)
		MouseWheel = 0;

	this->m_CursorX = MouseX;
	this->m_CursorY = MouseY;
}
