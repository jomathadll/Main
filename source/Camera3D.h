#pragma once

class CCamera
{
public:
	CCamera();
	virtual ~CCamera();
	void Toggle();
	void Restore();
	void Update();
	void OnMouseWheel(int delta);
private:
	int m_CursorX;
	int m_CursorY;
	int m_WheelDelta;
};

extern CCamera gCamera;
