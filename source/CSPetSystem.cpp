//////////////////////////////////////////////////////////////////////////
//  CSPetSystem.
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CSPetSystem.h"
#include "zzzlodterrain.h"
#include "zzzAi.h"
#include "ZzzOpenglUtil.h"
#include "zzzInfomation.h"
#include "zzzBmd.h"
#include "zzzObject.h"
#include "zzztexture.h"
#include "zzzCharacter.h"
#include "zzzscene.h"
#include "zzzInterface.h"
#include "zzzinventory.h"
#include "zzzeffect.h"
#include "dsplaysound.h"
#include "wsclientinline.h"
#include "UIManager.h"
#include "DuelMgr.h"
#include "MapManager.h"
#include "DarkSpirit.h"
#include "CustomWing.h"
#include "ItemManager.h"
#include "HelperSpeedConfig.h"
#include "GIPetManager.h"

extern bool g_PetEnableDuel;

extern  int     MoveSceneFrame;
extern  double   WorldTime;
extern	char    TextList[50][100];
extern	int     TextListColor[50];
extern	int     TextBold[50];
extern  float   g_fScreenRate_x;
extern  float   g_fScreenRate_y;

static int s_DarkRavenSpeedTunerFrame = -1;
static bool s_DarkRavenSpeedTunerVisible = false;
static bool s_DarkRavenSpeedTunerPositionReady = false;
static bool s_DarkRavenSpeedTunerDragging = false;
static float s_DarkRavenSpeedTunerX = 0.0f;
static float s_DarkRavenSpeedTunerY = 0.0f;
static float s_DarkRavenSpeedTunerDragX = 0.0f;
static float s_DarkRavenSpeedTunerDragY = 0.0f;
static bool s_DarkRavenSpeedTunerMouseDown = false;
static bool s_DarkRavenSpeedTunerMoved = false;
static DWORD s_DarkRavenSpeedTunerFeedbackTick = 0;
static int s_DarkRavenSpeedTunerLastChanged = -1;
static char s_DarkRavenSpeedTunerFeedbackText[32] = { 0 };
static const float DARK_RAVEN_APPROVED_FLIGHT_SPEED = 0.90f;
static const float DARK_RAVEN_APPROVED_ATTACK_SPEED = 3.00f;
static const float DARK_RAVEN_APPROVED_WING_SPEED = 0.46f;

static bool s_DarkRavenSpeedTunerValuesReady = false;
static float s_DarkRavenSpeedTunerFlight = DARK_RAVEN_APPROVED_FLIGHT_SPEED;
static float s_DarkRavenSpeedTunerAttack = DARK_RAVEN_APPROVED_ATTACK_SPEED;
static float s_DarkRavenSpeedTunerWings = DARK_RAVEN_APPROVED_WING_SPEED;

static const float DARK_RAVEN_TUNER_WIDTH = 180.0f;
static const float DARK_RAVEN_TUNER_HEIGHT = 118.0f;
static const float DARK_RAVEN_TUNER_HEADER_HEIGHT = 20.0f;
static const float DARK_RAVEN_TUNER_BUTTON_WIDTH = 22.0f;
static const float DARK_RAVEN_TUNER_LABEL_WIDTH = 122.0f;
static const float DARK_RAVEN_TUNER_ROW_HEIGHT = 15.0f;
static const float DARK_RAVEN_TUNER_GAP = 3.0f;
static const float DARK_RAVEN_TUNER_ROW_GAP = 17.0f;
static const float DARK_RAVEN_TUNER_DEFAULT_Y = 78.0f;

static bool ReadDarkRavenTunerInt(const char* key, int* value)
{
    (void)key;
    (void)value;
    return false;
}

static void WriteDarkRavenTunerInt(const char* key, int value)
{
    (void)key;
    (void)value;
}

static void SetDarkRavenSpeedTunerFeedback(int changed, const char* text)
{
    s_DarkRavenSpeedTunerLastChanged = changed;
    strcpy_s(s_DarkRavenSpeedTunerFeedbackText, text);
    s_DarkRavenSpeedTunerFeedbackTick = GetTickCount();
}

static float ClampDarkRavenSpeedTunerValue(float value, float minValue, float maxValue)
{
    if (value < minValue)
    {
        return minValue;
    }
    if (value > maxValue)
    {
        return maxValue;
    }
    return value;
}

static void EnsureDarkRavenSpeedTunerValues()
{
    if (s_DarkRavenSpeedTunerValuesReady)
    {
        return;
    }

    s_DarkRavenSpeedTunerFlight = DARK_RAVEN_APPROVED_FLIGHT_SPEED;
    s_DarkRavenSpeedTunerAttack = DARK_RAVEN_APPROVED_ATTACK_SPEED;
    s_DarkRavenSpeedTunerWings = DARK_RAVEN_APPROVED_WING_SPEED;
    s_DarkRavenSpeedTunerValuesReady = true;
}

static void ApplyDarkRavenSpeedTunerValues(bool save)
{
    EnsureDarkRavenSpeedTunerValues();
    gHelperSpeedConfig.SetDarkRavenFlightSpeed(s_DarkRavenSpeedTunerFlight, save);
    gHelperSpeedConfig.SetDarkRavenAttackSpeed(s_DarkRavenSpeedTunerAttack, save);
    gHelperSpeedConfig.SetDarkRavenWingAnimationSpeed(s_DarkRavenSpeedTunerWings, save);
}

void ToggleDarkRavenSpeedTuner()
{
    s_DarkRavenSpeedTunerVisible = false;
}

bool IsDarkRavenSpeedTunerVisible()
{
    return false;
}

void ShowDarkRavenSpeedTunerCentered()
{
    if (g_pNewUISystem != NULL)
    {
        lua_State* lua = g_pNewUISystem->m_Lua.GetState();
        if (lua != NULL)
        {
            lua_getglobal(lua, "NoxyDev_Command");
            const bool hasNoxyDevCommand = (lua_isfunction(lua, -1) != 0);
            lua_pop(lua, 1);

            if (hasNoxyDevCommand && g_pNewUISystem->m_Lua.Generic_Call("NoxyDev_Command", ">"))
            {
                return;
            }
        }
    }

    s_DarkRavenSpeedTunerVisible = false;
}

static void KeepDarkSpiritAboveTerrain(OBJECT* o)
{
    if (o == NULL)
    {
        return;
    }

    const float minHeight = RequestTerrainHeight(o->Position[0], o->Position[1]) + gHelperSpeedConfig.DarkRavenMinTerrainHeight();
    if (o->Position[2] < minHeight)
    {
        o->Position[2] = minHeight;
        if (o->Direction[2] < 0.0f)
        {
            o->Direction[2] = 0.0f;
        }
    }
}

static void KeepDarkSpiritInOwnerView(OBJECT* o, OBJECT* owner)
{
    if (o == NULL || owner == NULL || o->Type == MODEL_DARK_SPIRIT)
    {
        return;
    }

    const float maxHeight = owner->Position[2] + gHelperSpeedConfig.DarkRavenMaxOwnerHeight();
    if (o->Position[2] > maxHeight)
    {
        o->Position[2] = maxHeight;
        if (o->Direction[2] > 0.0f)
        {
            o->Direction[2] = 0.0f;
        }
        if (o->Angle[0] < 8.0f)
        {
            o->Angle[0] = 8.0f;
        }
    }
}

static bool IsMouseInDarkRavenSpeedButton(float x, float y, float width, float height)
{
    return MouseX >= x && MouseX <= x + width && MouseY >= y && MouseY <= y + height;
}

static void ClampDarkRavenSpeedTunerPosition()
{
    const float screenWidth = (float)GetScreenWidth();
    if (s_DarkRavenSpeedTunerX < 2.0f)
    {
        s_DarkRavenSpeedTunerX = 2.0f;
    }
    else if (s_DarkRavenSpeedTunerX > screenWidth - DARK_RAVEN_TUNER_WIDTH - 2.0f)
    {
        s_DarkRavenSpeedTunerX = screenWidth - DARK_RAVEN_TUNER_WIDTH - 2.0f;
    }

    if (s_DarkRavenSpeedTunerY < 2.0f)
    {
        s_DarkRavenSpeedTunerY = 2.0f;
    }
    else if (s_DarkRavenSpeedTunerY > 480.0f - DARK_RAVEN_TUNER_HEIGHT - 2.0f)
    {
        s_DarkRavenSpeedTunerY = 480.0f - DARK_RAVEN_TUNER_HEIGHT - 2.0f;
    }
}

static void EnsureDarkRavenSpeedTunerPosition()
{
    if (!s_DarkRavenSpeedTunerPositionReady)
    {
        int savedX = 0;
        int savedY = 0;
        if (ReadDarkRavenTunerInt("PanelX", &savedX) && ReadDarkRavenTunerInt("PanelY", &savedY))
        {
            s_DarkRavenSpeedTunerX = (float)savedX;
            s_DarkRavenSpeedTunerY = (float)savedY;
        }
        else
        {
            s_DarkRavenSpeedTunerX = ((float)GetScreenWidth() - DARK_RAVEN_TUNER_WIDTH) * 0.5f;
            s_DarkRavenSpeedTunerY = 190.0f;
        }
        s_DarkRavenSpeedTunerPositionReady = true;
    }

    ClampDarkRavenSpeedTunerPosition();
}

static void SaveDarkRavenSpeedTunerPosition(bool showFeedback)
{
    ClampDarkRavenSpeedTunerPosition();
    WriteDarkRavenTunerInt("PanelX", (int)s_DarkRavenSpeedTunerX);
    WriteDarkRavenTunerInt("PanelY", (int)s_DarkRavenSpeedTunerY);

    if (showFeedback)
    {
        SetDarkRavenSpeedTunerFeedback(3, "POSICAO SALVA");
    }
}

static void ConsumeDarkRavenSpeedTunerMouse()
{
    MouseOnWindow = true;
    MouseLButtonPush = false;
    MouseLButtonPop = false;
    MouseLButtonDBClick = false;
}

static void RenderDarkRavenSpeedButton(float x, float y, float width, float height, const char* text, bool hover)
{
    EnableAlphaBlend3();
    glColor4f(hover ? 0.34f : 0.20f, hover ? 0.34f : 0.20f, hover ? 0.34f : 0.20f, 1.00f);
    RenderColor(x, y, width, height);
    glColor4f(hover ? 0.78f : 0.58f, hover ? 0.78f : 0.58f, hover ? 0.78f : 0.58f, 1.00f);
    RenderColor(x, y, width, 1.0f);
    RenderColor(x, y + height - 1.0f, width, 1.0f);
    RenderColor(x, y, 1.0f, height);
    RenderColor(x + width - 1.0f, y, 1.0f, height);
    EndRenderColor();

    EnableAlphaTest();
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->SetTextColor(hover ? 255 : 230, hover ? 255 : 230, hover ? 255 : 230, 255);
    g_pRenderText->RenderText((int)(x + (width * 0.5f)), (int)(y + 3.0f), text, (int)width, 0, RT3_WRITE_CENTER);
}

enum DARK_RAVEN_SPEED_TUNER
{
    DARK_RAVEN_SPEED_FLIGHT = 0,
    DARK_RAVEN_SPEED_ATTACK = 1,
    DARK_RAVEN_SPEED_WINGS = 2,
};

static float GetDarkRavenSpeedTunerValue(DARK_RAVEN_SPEED_TUNER tuner)
{
    EnsureDarkRavenSpeedTunerValues();

    switch (tuner)
    {
    case DARK_RAVEN_SPEED_FLIGHT:
        return s_DarkRavenSpeedTunerFlight;
    case DARK_RAVEN_SPEED_ATTACK:
        return s_DarkRavenSpeedTunerAttack;
    case DARK_RAVEN_SPEED_WINGS:
    default:
        return s_DarkRavenSpeedTunerWings;
    }
}

static void SetDarkRavenSpeedTunerValue(DARK_RAVEN_SPEED_TUNER tuner, float value)
{
    EnsureDarkRavenSpeedTunerValues();

    switch (tuner)
    {
    case DARK_RAVEN_SPEED_FLIGHT:
        s_DarkRavenSpeedTunerFlight = ClampDarkRavenSpeedTunerValue(value, 0.10f, 3.0f);
        break;
    case DARK_RAVEN_SPEED_ATTACK:
        s_DarkRavenSpeedTunerAttack = ClampDarkRavenSpeedTunerValue(value, 0.10f, 3.0f);
        break;
    case DARK_RAVEN_SPEED_WINGS:
        s_DarkRavenSpeedTunerWings = ClampDarkRavenSpeedTunerValue(value, 0.01f, 3.0f);
        break;
    }

    ApplyDarkRavenSpeedTunerValues(true);
    SetDarkRavenSpeedTunerFeedback((int)tuner, "APLICADO EM TEMPO REAL");
}

static void SetDarkRavenSpeedTunerDefaults()
{
    s_DarkRavenSpeedTunerValuesReady = true;
    s_DarkRavenSpeedTunerFlight = DARK_RAVEN_APPROVED_FLIGHT_SPEED;
    s_DarkRavenSpeedTunerAttack = DARK_RAVEN_APPROVED_ATTACK_SPEED;
    s_DarkRavenSpeedTunerWings = DARK_RAVEN_APPROVED_WING_SPEED;
    ApplyDarkRavenSpeedTunerValues(true);

    s_DarkRavenSpeedTunerX = ((float)GetScreenWidth() - DARK_RAVEN_TUNER_WIDTH) * 0.5f;
    s_DarkRavenSpeedTunerY = 190.0f;
    s_DarkRavenSpeedTunerPositionReady = true;
    SaveDarkRavenSpeedTunerPosition(false);
    SetDarkRavenSpeedTunerFeedback(4, "DEFAULT APLICADO");
}

static void UpdateDarkRavenSpeedTunerButton(float x, float y, DARK_RAVEN_SPEED_TUNER tuner, float step, bool mousePressed)
{
    const bool minusHover = IsMouseInDarkRavenSpeedButton(x, y, DARK_RAVEN_TUNER_BUTTON_WIDTH, DARK_RAVEN_TUNER_ROW_HEIGHT);
    const bool plusHover = IsMouseInDarkRavenSpeedButton(
        x + DARK_RAVEN_TUNER_BUTTON_WIDTH + DARK_RAVEN_TUNER_GAP + DARK_RAVEN_TUNER_LABEL_WIDTH + DARK_RAVEN_TUNER_GAP,
        y,
        DARK_RAVEN_TUNER_BUTTON_WIDTH,
        DARK_RAVEN_TUNER_ROW_HEIGHT);

    if (mousePressed && minusHover)
    {
        SetDarkRavenSpeedTunerValue(tuner, GetDarkRavenSpeedTunerValue(tuner) - step);
        ConsumeDarkRavenSpeedTunerMouse();
    }
    else if (mousePressed && plusHover)
    {
        SetDarkRavenSpeedTunerValue(tuner, GetDarkRavenSpeedTunerValue(tuner) + step);
        ConsumeDarkRavenSpeedTunerMouse();
    }
}

void UpdateDarkRavenSpeedTunerMouse()
{
    if (!s_DarkRavenSpeedTunerVisible)
    {
        s_DarkRavenSpeedTunerDragging = false;
        s_DarkRavenSpeedTunerMouseDown = MouseLButton;
        return;
    }

    EnsureDarkRavenSpeedTunerPosition();
    ApplyDarkRavenSpeedTunerValues(false);

    const bool mousePressed = (MouseLButtonPush || (MouseLButton && !s_DarkRavenSpeedTunerMouseDown));
    const bool panelHover = IsMouseInDarkRavenSpeedButton(s_DarkRavenSpeedTunerX, s_DarkRavenSpeedTunerY, DARK_RAVEN_TUNER_WIDTH, DARK_RAVEN_TUNER_HEIGHT);
    const bool headerHover = IsMouseInDarkRavenSpeedButton(s_DarkRavenSpeedTunerX, s_DarkRavenSpeedTunerY, DARK_RAVEN_TUNER_WIDTH, DARK_RAVEN_TUNER_HEADER_HEIGHT);

    if (panelHover || s_DarkRavenSpeedTunerDragging)
    {
        ConsumeDarkRavenSpeedTunerMouse();
    }

    if (mousePressed)
    {
        const float rowX = s_DarkRavenSpeedTunerX + 4.0f;
        const float rowY = s_DarkRavenSpeedTunerY + 25.0f;
        const float defaultX = s_DarkRavenSpeedTunerX + 50.0f;
        const float defaultY = s_DarkRavenSpeedTunerY + DARK_RAVEN_TUNER_DEFAULT_Y;
        const bool defaultHover = IsMouseInDarkRavenSpeedButton(defaultX, defaultY, 80.0f, DARK_RAVEN_TUNER_ROW_HEIGHT);
        const bool controlHover =
            IsMouseInDarkRavenSpeedButton(rowX, rowY, DARK_RAVEN_TUNER_WIDTH - 8.0f, DARK_RAVEN_TUNER_ROW_HEIGHT) ||
            IsMouseInDarkRavenSpeedButton(rowX, rowY + DARK_RAVEN_TUNER_ROW_GAP, DARK_RAVEN_TUNER_WIDTH - 8.0f, DARK_RAVEN_TUNER_ROW_HEIGHT) ||
            IsMouseInDarkRavenSpeedButton(rowX, rowY + DARK_RAVEN_TUNER_ROW_GAP * 2.0f, DARK_RAVEN_TUNER_WIDTH - 8.0f, DARK_RAVEN_TUNER_ROW_HEIGHT) ||
            defaultHover;

        UpdateDarkRavenSpeedTunerButton(rowX, rowY, DARK_RAVEN_SPEED_FLIGHT, 0.05f, mousePressed);
        UpdateDarkRavenSpeedTunerButton(rowX, rowY + DARK_RAVEN_TUNER_ROW_GAP, DARK_RAVEN_SPEED_ATTACK, 0.05f, mousePressed);
        UpdateDarkRavenSpeedTunerButton(rowX, rowY + DARK_RAVEN_TUNER_ROW_GAP * 2.0f, DARK_RAVEN_SPEED_WINGS, 0.05f, mousePressed);

        if (defaultHover)
        {
            SetDarkRavenSpeedTunerDefaults();
            ConsumeDarkRavenSpeedTunerMouse();
        }
        else if (headerHover || (panelHover && !controlHover))
        {
            s_DarkRavenSpeedTunerDragging = true;
            s_DarkRavenSpeedTunerMoved = false;
            s_DarkRavenSpeedTunerDragX = MouseX - s_DarkRavenSpeedTunerX;
            s_DarkRavenSpeedTunerDragY = MouseY - s_DarkRavenSpeedTunerY;
            ConsumeDarkRavenSpeedTunerMouse();
        }
    }

    if (s_DarkRavenSpeedTunerDragging)
    {
        if (MouseLButton)
        {
            const float oldX = s_DarkRavenSpeedTunerX;
            const float oldY = s_DarkRavenSpeedTunerY;
            s_DarkRavenSpeedTunerX = MouseX - s_DarkRavenSpeedTunerDragX;
            s_DarkRavenSpeedTunerY = MouseY - s_DarkRavenSpeedTunerDragY;
            ClampDarkRavenSpeedTunerPosition();
            if ((int)oldX != (int)s_DarkRavenSpeedTunerX || (int)oldY != (int)s_DarkRavenSpeedTunerY)
            {
                s_DarkRavenSpeedTunerMoved = true;
                SaveDarkRavenSpeedTunerPosition(false);
            }
            ConsumeDarkRavenSpeedTunerMouse();
        }
        else
        {
            if (s_DarkRavenSpeedTunerMoved)
            {
                SaveDarkRavenSpeedTunerPosition(true);
            }
            s_DarkRavenSpeedTunerDragging = false;
            s_DarkRavenSpeedTunerMoved = false;
        }
    }

    s_DarkRavenSpeedTunerMouseDown = MouseLButton;
}

static void RenderDarkRavenSpeedTunerRow(float x, float y, const char* name, DARK_RAVEN_SPEED_TUNER tuner, const char* tip, bool highlight)
{
    const bool minusHover = IsMouseInDarkRavenSpeedButton(x, y, DARK_RAVEN_TUNER_BUTTON_WIDTH, DARK_RAVEN_TUNER_ROW_HEIGHT);
    const bool plusHover = IsMouseInDarkRavenSpeedButton(
        x + DARK_RAVEN_TUNER_BUTTON_WIDTH + DARK_RAVEN_TUNER_GAP + DARK_RAVEN_TUNER_LABEL_WIDTH + DARK_RAVEN_TUNER_GAP,
        y,
        DARK_RAVEN_TUNER_BUTTON_WIDTH,
        DARK_RAVEN_TUNER_ROW_HEIGHT);

    if (highlight)
    {
        EnableAlphaBlend3();
        glColor4f(0.42f, 0.42f, 0.42f, 1.00f);
        RenderColor(x, y, DARK_RAVEN_TUNER_WIDTH - 8.0f, DARK_RAVEN_TUNER_ROW_HEIGHT);
        EndRenderColor();
    }

    char label[32] = { 0 };
    sprintf_s(label, "%s %.2f", name, GetDarkRavenSpeedTunerValue(tuner));

    RenderDarkRavenSpeedButton(x, y, DARK_RAVEN_TUNER_BUTTON_WIDTH, DARK_RAVEN_TUNER_ROW_HEIGHT, "-", minusHover);
    RenderDarkRavenSpeedButton(x + DARK_RAVEN_TUNER_BUTTON_WIDTH + DARK_RAVEN_TUNER_GAP, y, DARK_RAVEN_TUNER_LABEL_WIDTH, DARK_RAVEN_TUNER_ROW_HEIGHT, label, highlight);
    RenderDarkRavenSpeedButton(
        x + DARK_RAVEN_TUNER_BUTTON_WIDTH + DARK_RAVEN_TUNER_GAP + DARK_RAVEN_TUNER_LABEL_WIDTH + DARK_RAVEN_TUNER_GAP,
        y,
        DARK_RAVEN_TUNER_BUTTON_WIDTH,
        DARK_RAVEN_TUNER_ROW_HEIGHT,
        "+",
        plusHover);

    if (minusHover || plusHover)
    {
        RenderTipText((int)x, (int)(y + DARK_RAVEN_TUNER_ROW_HEIGHT + 2.0f), tip);
    }
}

static void RenderDarkRavenSpeedTuner(float x, float y)
{
    x = s_DarkRavenSpeedTunerX;
    y = s_DarkRavenSpeedTunerY;

    const bool headerHover = IsMouseInDarkRavenSpeedButton(x, y, DARK_RAVEN_TUNER_WIDTH, DARK_RAVEN_TUNER_HEADER_HEIGHT);
    const bool feedbackVisible = s_DarkRavenSpeedTunerFeedbackTick != 0 && (GetTickCount() - s_DarkRavenSpeedTunerFeedbackTick) < 1400;
    const bool defaultHover = IsMouseInDarkRavenSpeedButton(x + 50.0f, y + DARK_RAVEN_TUNER_DEFAULT_Y, 80.0f, DARK_RAVEN_TUNER_ROW_HEIGHT);

    EnableAlphaBlend3();
    RenderColor(x, y, DARK_RAVEN_TUNER_WIDTH, DARK_RAVEN_TUNER_HEIGHT, 1.0f, 1);
    glColor4f(0.62f, 0.62f, 0.62f, 1.00f);
    RenderColor(x, y, DARK_RAVEN_TUNER_WIDTH, 1.0f);
    RenderColor(x, y + DARK_RAVEN_TUNER_HEIGHT - 1.0f, DARK_RAVEN_TUNER_WIDTH, 1.0f);
    RenderColor(x, y, 1.0f, DARK_RAVEN_TUNER_HEIGHT);
    RenderColor(x + DARK_RAVEN_TUNER_WIDTH - 1.0f, y, 1.0f, DARK_RAVEN_TUNER_HEIGHT);
    glColor4f(headerHover || s_DarkRavenSpeedTunerDragging ? 0.24f : 0.12f, headerHover || s_DarkRavenSpeedTunerDragging ? 0.24f : 0.12f, headerHover || s_DarkRavenSpeedTunerDragging ? 0.24f : 0.12f, 1.00f);
    RenderColor(x + 1.0f, y + 1.0f, DARK_RAVEN_TUNER_WIDTH - 2.0f, DARK_RAVEN_TUNER_HEADER_HEIGHT - 1.0f);
    EndRenderColor();

    EnableAlphaTest();
    g_pRenderText->SetFont(g_hFontBold);
    g_pRenderText->SetBgColor(0, 0, 0, 0);
    g_pRenderText->SetTextColor(255, 255, 255, 255);
    g_pRenderText->RenderText((int)(x + DARK_RAVEN_TUNER_WIDTH * 0.5f), (int)(y + 4.0f), "NOXYDEV CORVO", (int)DARK_RAVEN_TUNER_WIDTH, 0, RT3_WRITE_CENTER);

    const float rowX = x + 4.0f;
    const float rowY = y + 25.0f;
    RenderDarkRavenSpeedTunerRow(rowX, rowY, "Voo", DARK_RAVEN_SPEED_FLIGHT, "Vel. voo/seguindo", feedbackVisible && s_DarkRavenSpeedTunerLastChanged == DARK_RAVEN_SPEED_FLIGHT);
    RenderDarkRavenSpeedTunerRow(rowX, rowY + DARK_RAVEN_TUNER_ROW_GAP, "Ataque", DARK_RAVEN_SPEED_ATTACK, "Vel. investida/ataque", feedbackVisible && s_DarkRavenSpeedTunerLastChanged == DARK_RAVEN_SPEED_ATTACK);
    RenderDarkRavenSpeedTunerRow(rowX, rowY + DARK_RAVEN_TUNER_ROW_GAP * 2.0f, "Asas", DARK_RAVEN_SPEED_WINGS, "Vel. bater asas", feedbackVisible && s_DarkRavenSpeedTunerLastChanged == DARK_RAVEN_SPEED_WINGS);
    RenderDarkRavenSpeedButton(x + 50.0f, y + DARK_RAVEN_TUNER_DEFAULT_Y, 80.0f, DARK_RAVEN_TUNER_ROW_HEIGHT, "DEFAULT", defaultHover || (feedbackVisible && s_DarkRavenSpeedTunerLastChanged == 4));

    if (feedbackVisible || s_DarkRavenSpeedTunerDragging)
    {
        g_pRenderText->SetTextColor(230, 230, 230, 255);
        g_pRenderText->RenderText((int)(x + DARK_RAVEN_TUNER_WIDTH * 0.5f), (int)(y + 101.0f), feedbackVisible ? s_DarkRavenSpeedTunerFeedbackText : "ARRASTANDO", (int)DARK_RAVEN_TUNER_WIDTH, 0, RT3_WRITE_CENTER);
    }
}

static void RenderDarkRavenSpeedTunerOnce(float x, float y)
{
    if (!s_DarkRavenSpeedTunerVisible)
    {
        return;
    }

    if (s_DarkRavenSpeedTunerFrame == MoveSceneFrame)
    {
        return;
    }

    EnsureDarkRavenSpeedTunerPosition();

    s_DarkRavenSpeedTunerFrame = MoveSceneFrame;
    RenderDarkRavenSpeedTuner(s_DarkRavenSpeedTunerX, s_DarkRavenSpeedTunerY);
}

void RenderDarkRavenSpeedTunerGlobal()
{
    RenderDarkRavenSpeedTunerOnce(((float)GetScreenWidth() - DARK_RAVEN_TUNER_WIDTH) * 0.5f, 190.0f);
}

static bool IsDarkSpiritActionValid(OBJECT* o, int action)
{
    if (o == NULL || action < 0)
    {
        return false;
    }

    return action < Models[o->Type].NumActions;
}

static bool IsDarkSpiritActionAnimated(OBJECT* o, int action)
{
    return IsDarkSpiritActionValid(o, action) && Models[o->Type].Actions[action].NumAnimationKeys > 1;
}

static int GetDarkSpiritPreferredAction(int ai)
{
    switch (ai)
    {
    case PET_FLY:           return 0;
    case PET_FLYING:        return 1;
    case PET_STAND:         return 2;
    case PET_STAND_START:   return 1;
    case PET_ATTACK:
    case PET_ATTACK_MAGIC:
    case PET_ESCAPE:        return 3;
    default:                return 0;
    }
}

static int ResolveDarkSpiritAction(OBJECT* o)
{
    const int preferred = GetDarkSpiritPreferredAction(o->AI);

    if (IsDarkSpiritActionAnimated(o, preferred))
    {
        return preferred;
    }

    const int fallbackActions[] = { 1, 0, 2, 3 };
    for (int i = 0; i < (int)(sizeof(fallbackActions) / sizeof(fallbackActions[0])); ++i)
    {
        if (IsDarkSpiritActionAnimated(o, fallbackActions[i]))
        {
            return fallbackActions[i];
        }
    }

    if (IsDarkSpiritActionValid(o, preferred))
    {
        return preferred;
    }

    for (int i = 0; i < (int)(sizeof(fallbackActions) / sizeof(fallbackActions[0])); ++i)
    {
        if (IsDarkSpiritActionValid(o, fallbackActions[i]))
        {
            return fallbackActions[i];
        }
    }

    return -1;
}

static void ApplyDarkSpiritAction(OBJECT* o, bool restartSameAction)
{
    const int action = ResolveDarkSpiritAction(o);
    if (action < 0)
    {
        return;
    }

    if (o->CurrentAction != action)
    {
        SetAction(o, action);
    }
    else if (restartSameAction)
    {
        o->AnimationFrame = 0.f;
        o->PriorAnimationFrame = 0.f;
        o->PriorAction = (unsigned short)action;
    }
}

static bool IsDarkRavenAttackAI(int ai)
{
    return ai == PET_ATTACK || ai == PET_ATTACK_MAGIC || ai == PET_ESCAPE;
}

static float GetDarkRavenFollowVelocity(OBJECT* o)
{
    const float baseVelocity = (o != NULL && o->Type == MODEL_DARK_SPIRIT) ? 1.0f : gHelperSpeedConfig.DarkRavenFollowVelocity();
    return baseVelocity * gHelperSpeedConfig.DarkRavenFlightSpeed();
}

static float GetDarkRavenCatchUpScale(float distanceSq)
{
	if (distanceSq > 900.f * 900.f)
	{
		return 1.35f;
	}
	if (distanceSq > 640.f * 640.f)
	{
		return 1.25f;
	}
	if (distanceSq > 420.f * 420.f)
	{
		return 1.15f;
	}
	if (distanceSq > 260.f * 260.f)
	{
		return 1.05f;
	}

	return 1.0f;
}

static int GetDarkRavenCatchUpDirectionInterval(float distanceSq)
{
    if (distanceSq > 900.f * 900.f)
    {
        return 6;
    }
    if (distanceSq > 640.f * 640.f)
    {
        return 8;
    }
    if (distanceSq > 420.f * 420.f)
    {
        return 12;
    }
    if (distanceSq > 260.f * 260.f)
    {
        return 18;
    }

    return 28;
}

static float GetDarkRavenPetAnimationSpeed(OBJECT* o)
{
    float playSpeed = gHelperSpeedConfig.DarkRavenWingAnimationSpeed();
    if (o != NULL && IsDarkRavenAttackAI(o->AI))
    {
        playSpeed *= gHelperSpeedConfig.DarkRavenAttackSpeed();
    }
    if (playSpeed < 0.01f)
    {
        playSpeed = 0.01f;
    }
    else if (playSpeed > 3.0f)
    {
        playSpeed = 3.0f;
    }
    return playSpeed;
}

static int GetDarkRavenAttackFrameMax(int baseFrames)
{
    float attackSpeed = gHelperSpeedConfig.DarkRavenAttackSpeed();
    if (attackSpeed < 0.10f)
    {
        attackSpeed = 0.10f;
    }

    int frameMax = static_cast<int>((static_cast<float>(baseFrames) / attackSpeed) + 0.5f);
    if (frameMax < 3)
    {
        frameMax = 3;
    }
    else if (frameMax > baseFrames * 4)
    {
        frameMax = baseFrames * 4;
    }

    return frameMax;
}

CSPetSystem::~CSPetSystem()
{
	SAFE_DELETE_ARRAY(m_PetCharacter.Object.BoneTransform);
}

void CSPetSystem::CreatePetPointer ( int Type, unsigned char PositionX, unsigned char PositionY, float Rotation )
{
    CHARACTER*  c = &m_PetCharacter;
	OBJECT*     o = &c->Object;

    m_PetTarget = NULL;
    m_byCommand = PET_CMD_DEFAULT;
	m_pPetInfo	= NULL;	

	o->Initialize();
	c->PositionX      = PositionX;
	c->PositionY      = PositionY;
	c->TargetX      = PositionX;
	c->TargetY      = PositionY;

	c->byExtensionSkill = 0;
    c->m_pPet = NULL;

	int Index = TERRAIN_INDEX_REPEAT ( c->PositionX, c->PositionY );
	if ( ( TerrainWall[Index]&TW_SAFEZONE)==TW_SAFEZONE )
		c->SafeZone = true;
	else
		c->SafeZone = false;
	
	c->Path.PathNum     = 0;
	c->Path.CurrentPath = 0;
	c->Movement         = false;
	o->Live             = true;
	o->Visible          = false;
	o->AlphaEnable      = true;
	o->LightEnable      = true;
	o->ContrastEnable   = false;
	o->EnableBoneMatrix = true;
	o->EnableShadow     = false;
	c->Dead             = false;
	c->Blood            = false;
	c->GuildTeam        = 0;
	c->Run              = 0;
	c->GuildMarkIndex	= -1;
	c->PK               = PVP_NEUTRAL;
	o->Type             = Type;
	o->Scale            = 0.7f;
	o->Timer            = 0.f;
	o->Alpha            = 1.f;
	o->AlphaTarget      = 1.f;
	o->Velocity         = 0.f;
	o->ShadowScale      = 0.f;
	o->m_byHurtByOneToOne = 0;
    o->AI               = 0;
	o->Velocity         = 1.f;
    o->Gravity          = 13;
    c->ExtendState = 0;
    c->ExtendStateTime = 0;

	c->GuildStatus		= -1;
	c->GuildType		= 0;
	c->ProtectGuildMarkWorldTime = 0.0f;

	c->m_byDieType		= 0;
	o->m_bActionStart	= false;
	o->m_bySkillCount	= 0;

    c->Class            = 0;
    o->PriorAction      = 0;
    o->CurrentAction    = 0;
    o->AnimationFrame   = 0.f;
	o->PriorAnimationFrame   = 0;
	c->JumpTime         = 0;
	o->HiddenMesh       = -1;
	c->MoveSpeed        = 10;

	g_CharacterClearBuff(o);
	
	o->Teleport         = TELEPORT_NONE;
	o->Kind             = KIND_PET;
	c->Change           = false;
	o->SubType          = 0;
	c->MonsterIndex     = -1;
    o->BlendMeshTexCoordU = 0.f;
    o->BlendMeshTexCoordV = 0.f;
	o->Position[0] = (float)( PositionX*TERRAIN_SCALE ) + 0.5f*TERRAIN_SCALE;
	o->Position[1] = (float)( PositionY*TERRAIN_SCALE ) + 0.5f*TERRAIN_SCALE;

	o->InitialSceneFrame = MoveSceneFrame;
    o->Position[2] = RequestTerrainHeight ( o->Position[0], o->Position[1] );

	Vector ( 0.f, 0.f, Rotation, o->Angle );
	Vector ( 0.5f, 0.5f, 0.5f, o->Light );
	Vector ( -60.f, -60.f, 0.f, o->BoundingBoxMin );
	switch ( Type )
	{
    case MODEL_DARK_SPIRIT:
      	Vector( 50.f, 50.f,150.f,o->BoundingBoxMax);
        break;
    default:
      	Vector( 50.f, 50.f,150.f,o->BoundingBoxMax);
		break;
	}

	if ( o->BoneTransform!=NULL )
	{
		delete[] o->BoneTransform;
		o->BoneTransform = NULL;
	}
	o->BoneTransform = new vec34_t [Models[Type].NumBones];
	
	int i;
	for ( i=0; i<2; i++ )
	{
		c->Weapon[i].Type = -1;
		c->Weapon[i].Level = 0;
        c->Weapon[i].Option1 = 0;
	}
	for ( i=0; i<MAX_BODYPART; i++ )
	{
		c->BodyPart[i].Type     = -1;
		c->BodyPart[i].Level    = 0;
        c->BodyPart[i].Option1  = 0;
        c->BodyPart[i].ExtOption = 0;
	}

    c->Wing.Type        = -1;
	c->Helper.Type      = -1;
	c->Flag.Type        = -1;
	
	c->LongRangeAttack  = -1;
	c->CollisionTime    = 0;
	o->CollisionRange   = 200.f;
	c->Rot              = 0.f;
	c->Level            = 0;
	c->Item             = -1;
	
	o->BlendMesh        = -1;
	o->BlendMeshLight   = 1.f;

    switch ( Type )
	{
    case MODEL_DARK_SPIRIT:
		c->Weapon[0].LinkBone = 0; 
		c->Weapon[1].LinkBone = 0;
        break;
	default:
		c->Weapon[0].LinkBone = 0; 
		c->Weapon[1].LinkBone = 0;
		break;
	}
    m_byCommand = PET_CMD_DEFAULT;
}

bool CSPetSystem::PlayAnimation ( OBJECT* o )
{
    BMD*    b = &Models[o->Type];
    float   playSpeed = 0.1f;

  
    switch ( m_PetType )
    {
    case PET_TYPE_DARK_SPIRIT:
        playSpeed = GetDarkRavenPetAnimationSpeed(o);
        break;
    }

    b->CurrentAction = o->CurrentAction;
    return b->PlayAnimation ( &o->AnimationFrame, &o->PriorAnimationFrame, &o->PriorAction, playSpeed, o->Position, o->Angle );
}

void CSPetSystem::SetAI ( int AI )
{
    m_PetCharacter.Object.AI = AI;
    m_PetCharacter.Object.LifeTime = 0;
}

void CSPetSystem::SetCommand ( int Key, BYTE cmd ) 
{ 
    m_byCommand = cmd; 
    if ( m_PetCharacter.Object.AI!=PET_ATTACK && m_PetCharacter.Object.AI!=PET_ATTACK_MAGIC )
    {
        m_PetCharacter.Object.m_bActionStart = false;
    }
    if ( cmd==PET_CMD_TARGET )
    {
        int Index = FindCharacterIndex ( Key );
        m_PetTarget = &CharactersClient[Index];
        
        m_PetCharacter.Object.m_bActionStart = true;
    }
}

void CSPetSystem::SetAttack ( int Key, int attackType )
{
    int Index = FindCharacterIndex ( Key );
    m_PetTarget = &CharactersClient[Index];
	OBJECT* Owner = &m_PetOwner->Object;

	if ( g_isCharacterBuff( Owner, eDeBuff_Stun) )
	{
		return;
	}
	else if ( g_isCharacterBuff( Owner, eBuff_Cloaking) )
	{
		m_PetCharacter.TargetCharacter = Index;
		SetAI ( PET_ATTACK+attackType );
		return;
	}

    m_PetCharacter.TargetCharacter = Index;
    m_PetCharacter.AttackTime = 0;    
    SetAI ( PET_ATTACK+attackType );

    if ( m_PetCharacter.Object.AI==PET_ATTACK )
    {
        OBJECT* o = &m_PetCharacter.Object;

        o->m_bActionStart = true;

        const float attackMin = (o->Type == MODEL_DARK_SPIRIT) ? 10.0f : gHelperSpeedConfig.DarkRavenAttackVelocityMin();
        const float attackMax = (o->Type == MODEL_DARK_SPIRIT) ? 19.0f : gHelperSpeedConfig.DarkRavenAttackVelocityMax();
        const float attackGravity = (o->Type == MODEL_DARK_SPIRIT) ? 0.5f : gHelperSpeedConfig.DarkRavenAttackGravity();
        const int attackRange = static_cast<int>((attackMax - attackMin) * 1000.f);

        o->Velocity = attackMin + ((attackRange > 0) ? (static_cast<float>(rand() % (attackRange + 1)) / 1000.f) : 0.f);
        o->Gravity = attackGravity;

        PlayBuffer ( SOUND_DSPIRIT_RUSH );
    }
    else if ( m_PetCharacter.Object.AI==PET_ATTACK_MAGIC )
    {
        PlayBuffer ( SOUND_DSPIRIT_MISSILE );
    }
}

void CSPetSystem::MoveInventory ( void )
{
}

CSPetDarkSpirit::CSPetDarkSpirit ( CHARACTER* c )
{
    m_PetType  = PET_TYPE_DARK_SPIRIT;
    m_PetOwner = c;

    int Type = 0;

    auto Raven = gDarkSpirit.getDarkSpirit(c->Weapon[1].Type - MODEL_ITEM);

    if (Raven != nullptr)
    {
        Type = Raven->ItemModel;
    }
    else
    {
        Type = MODEL_DARK_SPIRIT;
    }

    m_PetCharacter.Object.BoneTransform = NULL;
    CreatePetPointer(Type, (c->PositionX), (c->PositionY), 0.f);

    m_PetCharacter.Object.Position[2] += 300.f;
    m_PetCharacter.Object.CurrentAction = 0;

    if (giPetManager::ShouldDarkSpiritStandOnArm(c) && c->Object.BoneTransform != NULL)
    {
        OBJECT* o = &m_PetCharacter.Object;
        BMD* b = &Models[c->Object.Type];
        vec3_t p;

        Vector(-10.f, 0.f, 10.f, p);
        b->TransformPosition(c->Object.BoneTransform[37], p, o->Position, true);
        VectorCopy(c->Object.Angle, o->Angle);
        o->Angle[2] -= 120.f;
        o->Velocity = 0.f;
        o->Gravity = 0.f;
        SetAI(PET_STAND);
        ApplyDarkSpiritAction(o, false);
    }
}

CSPetDarkSpirit::~CSPetDarkSpirit ( void )
{
	SAFE_DELETE_ARRAY(m_PetCharacter.Object.BoneTransform);
}

void CSPetDarkSpirit::MovePet ( void )
{
    bool    Play;
    CHARACTER* c = &m_PetCharacter;
    OBJECT* o = &c->Object;
    OBJECT* Owner = &m_PetOwner->Object;

    ApplyDarkRavenSpeedTunerValues(false);

	if ( g_isCharacterBuff( Owner, eDeBuff_Stun) )
		return;

    BMD* b = &Models[Owner->Type];
    o->WeaponLevel = (BYTE)c->Level;
   
	if(!g_DuelMgr.IsPetDuelEnabled())
	{
		m_byCommand = 0;
		CSPetSystem* pPet = (CSPetSystem*)m_PetOwner->m_pPet;
		SendRequestPetCommand ( pPet->GetPetType(), AT_PET_COMMAND_DEFAULT, 0xffff );
		g_DuelMgr.EnablePetDuel(TRUE);
	}
	
	if ( c->TargetCharacter >= 0 && c->TargetCharacter<MAX_CHARACTERS_CLIENT )
	{
		CHARACTER* tc = &CharactersClient[c->TargetCharacter];

	if ( (g_isCharacterBuff((&tc->Object), eBuff_Cloaking)||g_isCharacterBuff(Owner,eBuff_Cloaking)) 
		&& ( o->AI==PET_ATTACK || o->AI==PET_ESCAPE || o->AI == PET_ATTACK_MAGIC ) )
		{
			CSPetSystem* pPet = (CSPetSystem*)m_PetOwner->m_pPet;

			float dx = o->Position[0]-Owner->Position[0];
			float dy = o->Position[1]-Owner->Position[1];
			float Distance = sqrtf(dx*dx+dy*dy);

			c->TargetCharacter = -1;
			SendRequestPetCommand ( pPet->GetPetType(), AT_PET_COMMAND_DEFAULT, 0xffff );
			SetAI ( PET_STAND );
			if ( Distance>50 || ( o->AI!=PET_STAND_START && o->AI>=PET_FLYING && o->AI<=PET_STAND ) )
			{
				SetAI ( PET_STAND_START );
				o->Velocity = GetDarkRavenFollowVelocity(o);
			}
		}
	}
	else if((g_isCharacterBuff(Owner,eBuff_Cloaking)) && ( o->AI==PET_ATTACK || o->AI==PET_ESCAPE || o->AI == PET_ATTACK_MAGIC ))
	{
		CSPetSystem* pPet = (CSPetSystem*)m_PetOwner->m_pPet;

		float dx = o->Position[0]-Owner->Position[0];
		float dy = o->Position[1]-Owner->Position[1];
		float Distance = sqrtf(dx*dx+dy*dy);

		c->TargetCharacter = -1;
		SendRequestPetCommand ( pPet->GetPetType(), AT_PET_COMMAND_DEFAULT, 0xffff );
		SetAI ( PET_STAND );
		if ( Distance>50 || ( o->AI!=PET_STAND_START && o->AI>=PET_FLYING && o->AI<=PET_STAND ) )
		{
			SetAI ( PET_STAND_START );
			o->Velocity = GetDarkRavenFollowVelocity(o);
		}
	}

	const bool standOnOwnerArm = giPetManager::ShouldDarkSpiritStandOnArm(m_PetOwner);

    if ( standOnOwnerArm )
    {
		if ( o->AI!=PET_STAND )
		{
			SetAI ( PET_STAND );
			ApplyDarkSpiritAction(o, false);
		}

		c->TargetCharacter = -1;
		o->m_bActionStart = false;
		o->Velocity = 0.f;
		o->Gravity = 0.f;
    }
    else if ( o->AI==PET_STAND || o->AI==PET_STAND_START )
    {
		o->Gravity = 13.f;
        SetAI ( PET_FLYING );
    }

    ApplyDarkSpiritAction(o, false);

    Play = PlayAnimation ( o );
    if ( Play==false )
    {
        ApplyDarkSpiritAction(o, true);
    }

	vec3_t  Range, TargetPosition;
    float   FlyRange = 150.f;

    if ( o->m_bActionStart==true )
    {
        OBJECT* to = &m_PetTarget->Object;

        VectorCopy ( to->Position, TargetPosition );
	    VectorSubtract ( TargetPosition, o->Position, Range );
    }
    else
    {
        VectorCopy ( Owner->Position, TargetPosition );
	    VectorSubtract ( TargetPosition, o->Position, Range );
    }

    if ( o->AI==PET_FLY || o->AI==PET_FLYING )
    {           
        const float flightSpeed = gHelperSpeedConfig.DarkRavenFlightSpeed();
        o->m_bActionStart = false;

        vec3_t p, Pos, Light;
        Vector ( 0.3f, 0.4f, 0.7f, Light );
        Vector ( 0.f, 0.f, 0.f, p );

        /*
		if(!g_isCharacterBuff(Owner,eBuff_Cloaking))
		{
			for ( int i=0; i<1; ++i )
			{
				b->TransformPosition ( o->BoneTransform[rand()%66], p, Pos);
            
                Vector(0.7f, 0.1f, 0.1f, Light);
				CreateParticle(BITMAP_SPARK+1, Pos, o->Angle, Light, 5, 0.8f); //DARK RAVEN EFFECT
			}
		}
        */

	    float Distance = Range[0]*Range[0]+Range[1]*Range[1];
        const float catchUpScale = GetDarkRavenCatchUpScale(Distance);
	    if ( Distance>=FlyRange*FlyRange )
	    {
		    float Angle = CreateAngle ( o->Position[0], o->Position[1], TargetPosition[0], TargetPosition[1] );
            float turnSpeed = (float)(rand() % 15) + 5.f + ((catchUpScale - 1.0f) * 8.0f);
            if (turnSpeed > 35.f)
            {
                turnSpeed = 35.f;
            }
            o->Angle[2] = TurnAngle2 ( o->Angle[2], Angle, turnSpeed );
	    }
	    AngleMatrix ( o->Angle, o->Matrix );
	    
        vec3_t Direction;
	    VectorRotate ( o->Direction, o->Matrix, Direction );
        const float moveSpeed = flightSpeed * catchUpScale;
        Direction[0] *= moveSpeed;
        Direction[1] *= moveSpeed;
        Direction[2] *= moveSpeed;
	    VectorAdd(o->Position, Direction, o->Position);

        int speedRandom = 28;
        int CharacterHeight = 250;    

        if ( m_PetOwner->Helper.Type==ITEM_HELPER+4 || gMapManager.WorldActive == WD_55LOGINSCENE)
        {
            CharacterHeight = 350;
        }

        float Height = TargetPosition[2]+CharacterHeight;
        if ( o->Position[2]<Height )
        {
            speedRandom = 10;
            o->Angle[0] -= 2.f * FPS_ANIMATION_FACTOR;
            if ( o->Angle[0]<-15.f ) o->Angle[0] = -15.f;
        }
        else if ( o->Position[2]>Height+100 )
        {
            speedRandom = 20;
            o->Angle[0] += 2.f * FPS_ANIMATION_FACTOR;
            if ( o->Angle[0]>15.f ) o->Angle[0] = 15.f;
        }
	const int catchUpDirectionInterval = GetDarkRavenCatchUpDirectionInterval(Distance);
	if (catchUpDirectionInterval < speedRandom)
	{
		speedRandom = catchUpDirectionInterval;
	}

	float Speed = 0;
	if (rand_fps_check(speedRandom))
	{
		if ( Distance>=FlyRange*FlyRange )
		{
			Speed = -(float)(rand()%64+128)*0.1f;
		}
		else
		{
			Speed = -(float)(rand() % 8 + 32) * 0.1f;
			o->Angle[2] += (float)(rand() % 60) * FPS_ANIMATION_FACTOR;
		}

		Speed += o->Direction[1] * FPS_ANIMATION_FACTOR;
		Speed = Speed / 2.f;

		    o->Direction[0] = 0.f;
		    o->Direction[1] = Speed;
		    o->Direction[2] = (float)(rand()%64-32)*0.1f;
	    }

     	if ( o->Direction[1]<-12.f )
        {
            if ( o->AI!=PET_FLYING )
            {
                SetAI ( PET_FLYING );
            }
        }
        else if ( o->AI!=PET_FLY )
        {
            SetAI ( PET_FLY );
        }
    }
    else if ( o->AI==PET_ATTACK || o->AI==PET_ESCAPE )
    {
        const float attackSpeed = gHelperSpeedConfig.DarkRavenAttackSpeed();
        vec3_t p, Pos;

        AngleMatrix ( o->Angle, o->Matrix );
        Vector(0.f, -o->Velocity * attackSpeed, 0.f, p);
        VectorRotate(p,o->Matrix,Pos );
        if (o->Type == MODEL_DARK_SPIRIT)
        {
            VectorAdd(o->Position, Pos, o->Position);
        }
        else
        {
            VectorAddScaled(o->Position, Pos, o->Position, FPS_ANIMATION_FACTOR);
        }

        if (o->AI == PET_ATTACK)
        {
            TargetPosition[2] += 50.f;
            float Distance = MoveHumming(o->Position, o->Angle, TargetPosition, o->Velocity * attackSpeed);
            if (Distance < 20 || o->LifeTime > GetDarkRavenAttackFrameMax(20))
            {
                SetAI(PET_ESCAPE);
                o->Angle[0] = -45.f;
                if (m_byCommand != PET_CMD_TARGET)
                {
                    o->m_bActionStart = false;
                }
            }
            if (o->Type == MODEL_DARK_SPIRIT)
            {
                o->Velocity += o->Gravity * attackSpeed;
                o->Gravity += 0.2f * attackSpeed;
                o->LifeTime++;
            }
            else
            {
                o->Velocity += o->Gravity * attackSpeed * FPS_ANIMATION_FACTOR;
                if (o->Velocity > gHelperSpeedConfig.DarkRavenAttackVelocityMax())
                {
                    o->Velocity = gHelperSpeedConfig.DarkRavenAttackVelocityMax();
                }
                o->Gravity += 0.2f * attackSpeed * FPS_ANIMATION_FACTOR;
                o->LifeTime += FPS_ANIMATION_FACTOR;
            }
        }
        else if ( o->AI==PET_ESCAPE )
        {
	        float Distance = Range[0]*Range[0]+Range[1]*Range[1];
            if ( Distance>=(FlyRange+100)*(FlyRange+100) )
            {
                SetAI ( PET_FLYING );
            }
            o->Velocity -= (o->Type == MODEL_DARK_SPIRIT) ? attackSpeed : (attackSpeed * FPS_ANIMATION_FACTOR);
        }
        SetAction ( o, 3 );
    }
    else if ( o->AI==PET_ATTACK_MAGIC )
    {
        if ( c->TargetCharacter!=-1 )
        {
            CHARACTER* tc = &CharactersClient[c->TargetCharacter];
            OBJECT* to = &tc->Object;

            float Angle = CreateAngle ( o->Position[0], o->Position[1], to->Position[0], to->Position[1] );
            o->Angle[2] = TurnAngle2 ( o->Angle[2], Angle, 40.f );
        }
    }
    else if (o->AI == PET_STAND)
    {
        vec3_t p;
    
        Vector(-10.f, 0.f, 10.f, p);
        b->TransformPosition(Owner->BoneTransform[37], p, o->Position, true);
        VectorCopy(Owner->Angle, o->Angle);
        o->Angle[2] -= 120.f;
    }
    //else if (o->AI == PET_STAND) //DARK RAVEN OMBRO
    //{
    //    vec3_t p;
    //
    //    Vector(-10.f, 0.f, 10.f, p);
    //    b->TransformPosition(Owner->BoneTransform[35], p, o->Position, true);
    //    VectorCopy(Owner->Angle, o->Angle);
    //    o->Angle[0] -= 10.f;
    //}
    else if ( o->AI==PET_STAND_START )
    {
        vec3_t p, Pos;

        AngleMatrix ( o->Angle, o->Matrix );
        if ( o->Velocity!=0.0f )
        {
            Vector(0.f,-o->Velocity,0.f,p);
            VectorRotate(p,o->Matrix,Pos );
            VectorAddScaled(o->Position, Pos, o->Position, FPS_ANIMATION_FACTOR);
        }
        Vector ( 0.f, 0.f, 0.f, p );
        b->TransformPosition ( Owner->BoneTransform[42], p, Pos, true );

        float Distance = MoveHumming ( o->Position, o->Angle, Pos, o->Velocity );
        const float flightSpeed = gHelperSpeedConfig.DarkRavenFlightSpeed();
        const float standMaxVelocity = gHelperSpeedConfig.DarkRavenStandMaxVelocity() * flightSpeed;
        o->Velocity += gHelperSpeedConfig.DarkRavenStandAcceleration() * flightSpeed * FPS_ANIMATION_FACTOR;
        if (o->Velocity > standMaxVelocity)
        {
            o->Velocity = standMaxVelocity;
        }
        if ( Distance<50 )
        {
            SetAI ( PET_STAND );
        }
    }
    if ( o->AI>=PET_ATTACK && o->AI<=PET_ATTACK_MAGIC )
    {
        const int attackTimeMax = GetDarkRavenAttackFrameMax((o->AI == PET_ATTACK_MAGIC && o->Type == MODEL_DARK_SPIRIT) ? 21 : 15);
        c->AttackTime++;
        if ( c->AttackTime>=attackTimeMax )
        {
            c->AttackTime = attackTimeMax;
        }
    }
    //SOM DO CORVO
    if (rand_fps_check(100) && rand_fps_check(60))
    {
        PlayBuffer ( SOUND_DSPIRIT_SHOUT, o );
    }

    KeepDarkSpiritAboveTerrain(o);
    KeepDarkSpiritInOwnerView(o, Owner);

    VectorCopy ( m_PetOwner->Object.Position, TargetPosition );
	VectorSubtract ( TargetPosition, o->Position, Range );
	float Distance = Range[0]*Range[0]+Range[1]*Range[1];
	if (!standOnOwnerArm && !o->m_bActionStart && !IsDarkRavenAttackAI(o->AI)
		&& (o->Position[2] < (TargetPosition[2] - 200.f) || Distance > 640.f * 640.f))
    {
        o->LifeTime += FPS_ANIMATION_FACTOR;
    }
    else if (!IsDarkRavenAttackAI(o->AI))
    {
        o->LifeTime = 0;
    }
	if (!standOnOwnerArm && !o->m_bActionStart && !IsDarkRavenAttackAI(o->AI) && o->LifeTime > 30)
	{
		o->LifeTime = 0;
		VectorCopy(TargetPosition, o->Position);
		o->Position[2] += 250.f;
		VectorCopy(Owner->Angle, o->Angle);
		o->Angle[2] -= 120.f;
		o->Velocity = GetDarkRavenFollowVelocity(o);
		o->Direction[0] = 0.f;
		o->Direction[1] = 0.f;
		o->Direction[2] = 0.f;
		KeepDarkSpiritAboveTerrain(o);
		KeepDarkSpiritInOwnerView(o, Owner);
	}
}

void CSPetDarkSpirit::CalcPetInformation ( const PET_INFO& Petinfo )
{

}

void    CSPetDarkSpirit::RenderPetInventory ( void )
{
    RenderCmdType ();
}

void    CSPetDarkSpirit::RenderPet ( int PetState )
{
    CHARACTER* c = &m_PetCharacter;
    OBJECT* o    = &c->Object;

    if (c == nullptr || o == nullptr) 
    {
        return;
    }

	if(m_pPetInfo)
		o->WeaponLevel = static_cast<BYTE>(m_pPetInfo->m_wLevel & 0xFF);

	if ( o->Live )
	{
		o->Visible = TestFrustrum2D ( o->Position[0]*0.01f, o->Position[1]*0.01f, -20.f );
		if ( o->Visible )
		{
			if ( m_PetOwner->Object.Type!=MODEL_PLAYER && o->Type!=MODEL_DARK_SPIRIT )
				return;

			RenderObject ( o, false,0, PetState);
            AttackEffect ( c, o );
		}
        else
        {
            o->Velocity = GetDarkRavenFollowVelocity(o);
            if ( o->AI!=PET_STAND_START )
            {
                o->AI = PET_FLYING;
            }
        }
	}

}

void    CSPetDarkSpirit::Eff_LevelUp ( void )
{
    OBJECT* o    = &m_PetCharacter.Object;

    vec3_t Angle    = { 0.f, 0.f, 0.f };
    vec3_t Position = { o->Position[0], o->Position[1], o->Position[2] };

    for ( int i=0; i<5; ++i )
    {
        CreateJoint ( BITMAP_FLARE, Position, Position, Angle, 0, o, 40, 2 );
    }
}

void    CSPetDarkSpirit::Eff_LevelDown ( void )
{
    OBJECT* o    = &m_PetCharacter.Object;

    vec3_t Position = { o->Position[0], o->Position[1], o->Position[2] };

    for ( int i=0; i<15; ++i )
    {
        CreateJoint(BITMAP_FLARE,Position,o->Position,o->Angle,0,o,40,2);
    }
}

void    CSPetDarkSpirit::RenderCmdType ( void )
{
    float   x, y, Width, Height;
    float   PartyWidth = 0.f;

	g_pRenderText->SetFont(g_hFontBold);
	g_pRenderText->SetTextColor(220, 220, 220, 255);
	g_pRenderText->SetBgColor(0, 0, 0, 128);

    if ( PartyNumber>0 )
    {
        PartyWidth = 50.f;
    }
    if ( (Hero->Helper.Type>=MODEL_HELPER+0 && Hero->Helper.Type<=MODEL_HELPER+4) || Hero->Helper.Type == MODEL_HELPER+37)
    {
        PartyWidth += 60.f;
    }


	int Dur = 255;
    Width=50;Height=2;x=GetScreenWidth()-Width-PartyWidth-15;y=4;
	ITEM* pDarkSpiritItem = &CharacterMachine->Equipment[EQUIPMENT_WEAPON_LEFT];
	if (pDarkSpiritItem->Type != ITEM_HELPER + 5 && !gDarkSpirit.checkIsDarkSpirit(pDarkSpiritItem->Type))
	{
		pDarkSpiritItem = &CharacterMachine->Equipment[NEW_EQUIPMENT_HELPER_FLY];
	}
	int Life = pDarkSpiritItem->Durability*(int)Width/Dur;

    EnableAlphaTest();
    
	g_pRenderText->RenderText((int)x+50, (int)y, GlobalText[1214], 0, 0, RT3_WRITE_RIGHT_TO_LEFT);
    
	RenderBar ( x, y+12, Width, Height, (float)Life );

    glColor3f ( 1.f, 1.f, 1.f );

    Width=20.f; Height=28.f; x=GetScreenWidth()-Width-PartyWidth-65.f; y=5.f; 
    RenderBitmap ( BITMAP_SKILL_INTERFACE+2, (float)x, (float)y, (float)Width-4, (float)Height-8, (((m_byCommand)%8)*32+6.f)/256.f, (((m_byCommand)/8)*Height+3.f)/256.f,Width/256.f,(Height-1.f)/256.f);

    Width -= 8.f; Height -= 8.f;

    if ( MouseX>=x && MouseX<=x+Width && MouseY>=y && MouseY<=y+Height )
    {
        RenderTipText ( (int)x, (int)(y+Height), GlobalText[1219+m_byCommand] );
    }
}

void CSPetDarkSpirit::AttackEffect ( CHARACTER* c, OBJECT* o )
{
    BMD* b = &Models[o->Type];
    vec3_t p, Pos, Light;

   

    switch ( o->AI )
    {
    case PET_ATTACK:
		if ( c->AttackTime>=0 && c->AttackTime<=2 && m_PetTarget!=NULL )
        {
            for( int i=0;i<10;i++)
            {
			    CreateJoint ( BITMAP_LIGHT, o->Position, o->Position, o->Angle, 1, NULL, (float)(rand()%40+20) );
            }
            if ( c->AttackTime==1 )
            {
                vec3_t Angle, Light;

                Vector ( 45.f, (float)(rand()%180)-90, 0.f, Angle );
                Vector ( 1.f, 0.8f, 0.6f, Light );
                CreateEffect ( MODEL_DARKLORD_SKILL, o->Position, Angle, Light, 3 );
            }
        }
        if ( c->AttackTime>3 && c->AttackTime%2 )
        {
            if ( o->Position[2]>(m_PetOwner->Object.Position[2]+100.f ) )
            {
                Vector ( 50.f, 0.f, 0.f, p );
                b->TransformPosition ( o->BoneTransform[6], p, Pos);

	            CreateEffect ( MODEL_AIR_FORCE, Pos, o->Angle, o->Light, 0, o );
            }
        }
		break;

    case PET_ATTACK_MAGIC:
    {
        const int magicAttackTimeMax = GetDarkRavenAttackFrameMax((o->Type == MODEL_DARK_SPIRIT) ? 21 : 15);
        if ( c->AttackTime<magicAttackTimeMax )
        {
            if ( o->BoneTransform!=NULL )
            {
                Vector ( 1.f, 0.6f, 0.4f, Light );
                Vector ( 0.f, 0.f, 0.f, p );
                for ( int i=rand()%2; i<66; i+=2 )
                {
                    if ( !b->Bones[i].Dummy && i<b->NumBones )
                    {
                        b->TransformPosition ( o->BoneTransform[i], p, Pos );
                        CreateParticle( BITMAP_LIGHT, Pos, o->Angle, Light, 6, 1.3f );
                    }
                }
            }
        }
        else if ( c->AttackTime>=magicAttackTimeMax )
        {
			if(c->TargetCharacter != -1)
			{
				CHARACTER* tc = &CharactersClient[c->TargetCharacter];
				OBJECT* to = &tc->Object;

				if ( to!=NULL )
				{
					b->RenderMeshEffect ( 1, BITMAP_LIGHT, 1, o->Angle, to );
				}

				SetAI ( PET_FLYING );
			}
            c->AttackTime = 0;
        }
        break;
    }

    default :
        break;
    }
}
