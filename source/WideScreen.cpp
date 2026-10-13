#include "stdafx.h"
#include "WideScreen.h"

CWideScreen gWideScreen;

void CWideScreen::Init()
{
	if (m_Resolution > 3)
	{
		this->g_WideWindowWidth = (float)WindowWidth / g_fScreenRate_x;
		this->g_WideWindowWidthAdd = this->g_WideWindowWidth - 640;
	
	
		this->g_WideWindowHeight = (float)WindowHeight / g_fScreenRate_y;
		this->g_WideWindowHeightAdd = this->g_WideWindowHeight - 480;
	}
	else {
		this->g_WideWindowWidth = (float)WindowWidth / g_fScreenRate_y;
		this->g_WideWindowWidthAdd = this->g_WideWindowWidth - 640;

		this->g_WideWindowHeight = 480;
		this->g_WideWindowHeightAdd = 0;
	}


	
}

void CWideScreen::SceneLogin()
{
	if (gMapManager.WorldActive == 94 && SceneFlag == LOG_IN_SCENE)
	{
		CameraViewFar = 33000.0;		//	CameraViewFar
		CameraAngle[0] = -84.0;		//  CameraAngle[0]
		CameraAngle[1] = 0.0;			//  CameraAngle[1]
		CameraAngle[2] = -45.0;		//  CameraAngle[2]
		CameraPosition[0] = 24475.79687;	//  CameraPosition[0]
		CameraPosition[1] = 7581.581055;	//	CameraPosition[1]
		CameraPosition[2] = 1834.539917;	//  CameraPosition[2]
		CameraFOV = 35.0;			//  CameraFOV
	}

	if (SceneFlag == CHARACTER_SCENE)
	{
		CameraViewFar = 3500.0;
		CameraAngle[0] = -84.5f;
		CameraAngle[1] = 0.0f;
		CameraAngle[2] = -75.0f;
		CameraPosition[0] = 9758.93f;
		CameraPosition[1] = 18913.11f;
		CameraPosition[2] = 675.5f;
		CameraFOV = 35.0;

		//CameraViewFar = 3500.0;
		//CameraAngle[0] = -82.0; //meu Y
		//CameraAngle[1] = 0.0;
		//CameraAngle[2] = -90.0;
		//CameraPosition[0] = 9758.9297;
		//CameraPosition[1] = 18913.109;
		//CameraPosition[2] = 500.0;
		//CameraFOV = 35.0;
	}
}

int CWideScreen::GetAddPosition(int x) {
	return x + this->g_WideWindowWidthAdd;

}
int CWideScreen::GetCenterPosition(int x) {
	return x + this->g_WideWindowWidthAdd/2;

}

float CWideScreen::GetBottomPosition(float y) {
	return (float)this->g_WideWindowHeight - (480 - y);
}