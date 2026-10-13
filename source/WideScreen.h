#pragma once

extern float g_fScreenRate_x;
extern float g_fScreenRate_y;

class CWideScreen
{
public:
	void Init();
	int GetAddPosition(int x);
	int GetCenterPosition(int x);
	float GetBottomPosition(float y);
	void SceneLogin();

public:
	float g_WideWindowWidth;
	float g_WideWindowWidthAdd;
	float g_WideWindowHeight;
	float g_WideWindowHeightAdd;


public:
};

extern CWideScreen gWideScreen;