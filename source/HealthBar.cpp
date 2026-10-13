#include "stdafx.h"
#include "HealthBar.h"
#include "ZzzOpenglUtil.h"
#include "ZzzCharacter.h"
#include "UIControls.h"
#include "WideScreen.h"

CHealthBar gHealthBar;

namespace
{
	struct HEALTH_BAR_RENDER_INFO
	{
		GLuint BackTexture;
		GLuint LifeTexture;
		float SourceBackX;
		float SourceBackY;
		float SourceBackWidth;
		float SourceBackHeight;
		float SourceLifeX;
		float SourceLifeY;
		float SourceLifeWidth;
		float SourceLifeHeight;
		float ScreenWidth;
		float ScreenHeight;
		float ScreenLifeOffsetX;
		float ScreenLifeOffsetY;
		float ScreenLifeWidth;
		float ScreenLifeHeight;
		float ScreenNameOffsetX;
		float ScreenNameOffsetY;
		float ScreenNameWidth;
	};

	bool IsBossMonster(WORD MonsterIndex)
	{
		static const WORD BossMonsterIndex[] = {
			43, 53, 54, 55, 78, 79, 80, 81, 82, 83,
			161, 181, 189, 197, 267, 275, 295, 338,
			349, 361, 362, 363, 364, 459, 493, 494, 495,
			496, 497, 498, 499, 500, 501, 502, 504, 561,
			601, 602
		};

		for (int n = 0; n < (int)(sizeof(BossMonsterIndex) / sizeof(BossMonsterIndex[0])); n++)
		{
			if (BossMonsterIndex[n] == MonsterIndex)
			{
				return true;
			}
		}

		return false;
	}

	BYTE ClampHealthRate(BYTE Rate)
	{
		return ((Rate > 100) ? 100 : Rate);
	}

	void RenderHealthBarTexture(const HEALTH_BAR_RENDER_INFO& Info, float PosX, float PosY, BYTE Rate)
	{
		EnableAlphaTest(false);
		glColor4f(1.f, 1.f, 1.f, 1.f);

		const float LifeRate = (float)ClampHealthRate(Rate) / 100.f;
		const float LifeScaleX = Info.ScreenWidth / Info.SourceBackWidth;
		const float LifeScaleY = Info.ScreenHeight / Info.SourceBackHeight;
		const float LifePosX = PosX + ((Info.SourceLifeX - Info.SourceBackX) * LifeScaleX);
		const float LifePosY = PosY + ((Info.SourceLifeY - Info.SourceBackY) * LifeScaleY);
		const float LifeWidth = Info.SourceLifeWidth * LifeScaleX;
		const float LifeHeight = Info.SourceLifeHeight * LifeScaleY;
		const bool HasCustomLifeSize = (Info.ScreenLifeWidth > 0.f && Info.ScreenLifeHeight > 0.f);
		const float RenderLifePosX = HasCustomLifeSize ? (PosX + Info.ScreenLifeOffsetX) : LifePosX;
		const float RenderLifePosY = HasCustomLifeSize ? (PosY + Info.ScreenLifeOffsetY) : LifePosY;
		const float RenderLifeWidth = HasCustomLifeSize ? Info.ScreenLifeWidth : LifeWidth;
		const float RenderLifeHeight = HasCustomLifeSize ? Info.ScreenLifeHeight : LifeHeight;

		SEASON3B::RenderImageScale(Info.BackTexture, PosX, PosY, Info.ScreenWidth, Info.ScreenHeight, Info.SourceBackX, Info.SourceBackY, Info.SourceBackWidth, Info.SourceBackHeight);

		if (LifeRate > 0.f)
		{
			SEASON3B::RenderImageScale(Info.LifeTexture, RenderLifePosX, RenderLifePosY, RenderLifeWidth * LifeRate, RenderLifeHeight, Info.SourceLifeX, Info.SourceLifeY, Info.SourceLifeWidth * LifeRate, Info.SourceLifeHeight);
		}
	}
}

CHealthBar::CHealthBar()
{
	ClearNewHealthBar();
}

CHealthBar::~CHealthBar()
{

}

void CHealthBar::ClearNewHealthBar() // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		gNewHealthBar[n].index = 0xFFFF;
		gNewHealthBar[n].type = 0;
		gNewHealthBar[n].rate = 0;
		gNewHealthBar[n].life = 0;
		gNewHealthBar[n].maxLife = 0;
	}
}

void CHealthBar::InsertNewHealthBar(WORD index, BYTE type, BYTE rate, DWORD life, DWORD maxLife) // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (gNewHealthBar[n].index == 0xFFFF)
		{
			gNewHealthBar[n].index = index;
			gNewHealthBar[n].type = type;
			gNewHealthBar[n].rate = rate;
			gNewHealthBar[n].life = life;
			gNewHealthBar[n].maxLife = maxLife;
			return;
		}
	}
}

NEW_HEALTH_BAR* CHealthBar::GetNewHealthBar(WORD index, BYTE type) // OK
{
	for (int n = 0; n < MAX_MAIN_VIEWPORT; n++)
	{
		if (gNewHealthBar[n].index != 0xFFFF)
		{
			if (gNewHealthBar[n].index == index && gNewHealthBar[n].type == type)
			{
				return &gNewHealthBar[n];
			}
		}
	}

	return 0;
}

void CHealthBar::DrawNewHealthBar() // OK
{
	char    Text[100];

	//for (int j = 0; j < MAX_CHARACTERS_CLIENT; ++j)
	if(SelectedCharacter != -1)
	{
		CHARACTER* c = &CharactersClient[SelectedCharacter];
		OBJECT* o = &c->Object;

		if (KIND_MONSTER == c->Object.Kind && MODEL_PLAYER != c->Object.Type && c->Object.Kind != KIND_TRAP)
		{
			if (o->Live && KIND_NPC != c->Object.Kind)
			{
				NEW_HEALTH_BAR* lpNewHealthBar = gHealthBar.GetNewHealthBar(c->Key, c->Object.Kind);

				if (lpNewHealthBar == NULL) {
					glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
					DisableAlphaBlend();
					glColor3f(1.f, 1.f, 1.f);
					return;
				}

				static const HEALTH_BAR_RENDER_INFO BossHealthBar =
				{
					BITMAP_NEW_HEALTHBAR_MONSTER_BACK,
					BITMAP_NEW_HEALTHBAR_MONSTER_LIFE,
					393.f, 327.f, 1120.f, 156.f,
					621.f, 393.f, 674.f, 42.f,
					230.f, 32.f,
					0.f, 0.f, 0.f, 0.f,
					0.f, 0.f, 0.f
				};

				static const HEALTH_BAR_RENDER_INFO MonsterHealthBar =
				{
					BITMAP_NEW_HEALTHBAR_MONSTER_BACK,
					BITMAP_NEW_HEALTHBAR_MONSTER_LIFE,
					393.f, 327.f, 1120.f, 156.f,
					621.f, 393.f, 674.f, 42.f,
					230.f, 32.f,
					0.f, 0.f, 0.f, 0.f,
					0.f, 0.f, 0.f
				};

				const bool bIsBoss = IsBossMonster(c->MonsterIndex);
				const HEALTH_BAR_RENDER_INFO& HealthBarInfo = bIsBoss ? BossHealthBar : MonsterHealthBar;
				const float posX = ((gWideScreen.g_WideWindowWidth - HealthBarInfo.ScreenWidth) / 2.f) + (bIsBoss ? 45.f : 0.f);
				const float posY = bIsBoss ? 10.f : 18.f;

				RenderHealthBarTexture(HealthBarInfo, posX, posY, lpNewHealthBar->rate);

				g_pRenderText->SetFont(g_hFontBold);
				if (c->Dead != 0)
				{
					sprintf(Text, "%s die...", c->ID);
				}
				else
				{
					sprintf(Text, "%s", c->ID);
				}
				g_pRenderText->SetBgColor(0, 0, 0, 0);
				if (bIsBoss)
				{
					g_pRenderText->SetTextColor(255, 255, 0, 255);
					g_pRenderText->RenderText(posX, posY + (HealthBarInfo.ScreenHeight / 2.f) - 5.f, Text, HealthBarInfo.ScreenWidth, 0, RT3_SORT_CENTER);
				}
				else
				{
					g_pRenderText->SetTextColor(255, 230, 210, 255);
					g_pRenderText->RenderText(posX, posY + (HealthBarInfo.ScreenHeight / 2.f) - 5.f, Text, HealthBarInfo.ScreenWidth, 0, RT3_SORT_CENTER);
				}
			}
		}
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	DisableAlphaBlend();
	glColor3f(1.f, 1.f, 1.f);
}
