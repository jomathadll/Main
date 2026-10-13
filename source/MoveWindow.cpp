#include "stdafx.h"
#include "MoveWindow.h"
#include "WideScreen.h"

cMoveWindow::cMoveWindow()
{
    m_PosTemp.x = 0;
    m_PosTemp.y = 0;
    m_OriginalPos.x = 0;  // Nova variável para guardar a posição original
    m_OriginalPos.y = 0;
    //
    Width = 0;
    Height = 0;
    //
    IsMove = false;
    //
    m_PosOffset.x = 0;
    m_PosOffset.y = 0;
}

cMoveWindow::~cMoveWindow()
{

}

void cMoveWindow::Create(int x, int y, int width, int height)
{
    m_PosTemp.x = x;
    m_PosTemp.y = y;
    m_OriginalPos.x = x;  // Guarda a posição original ao criar
    m_OriginalPos.y = y;
    //
    Width = width;
    Height = height;
}

bool cMoveWindow::UpdatePosition(int x, int y, int WindowID)
{
    IsMove = false;
    m_PosTemp.x = x;
    m_PosTemp.y = y;
    return false;
}

// Novo método para resetar a posição
void cMoveWindow::ResetToOriginalPosition()
{
    m_PosTemp.x = m_OriginalPos.x;
    m_PosTemp.y = m_OriginalPos.y;
    IsMove = false;
}