#include "stdafx.h"
#include "HelperSpeedConfig.h"

static float ClampHelperFloat(float value, float minValue, float maxValue)
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

CHelperSpeedConfig gHelperSpeedConfig;

CHelperSpeedConfig::CHelperSpeedConfig()
{
	m_Loaded = false;

	m_DarkHorseMoveSpeed = 20.0f;
	m_CustomHorseMoveSpeed = 20.0f;
	m_UniriaMoveSpeed = 10.0f;
	m_CustomUniriaMoveSpeed = 15.0f;
	m_DarkHorseRunVelocity = 0.33f;
	m_DarkHorseAttackVelocity = 0.34f;
	m_DarkHorseIdleVelocity = 0.30f;
	m_UniriaRunVelocity = 0.14f;
	m_DinorantRunVelocity = 0.34f;
	m_DarkRavenFollowVelocity = 1.0f;
	m_DarkRavenStandMaxVelocity = 1.2f;
	m_DarkRavenStandAcceleration = 0.05f;
	m_DarkRavenAttackVelocityMin = 8.0f;
	m_DarkRavenAttackVelocityMax = 12.0f;
	m_DarkRavenAttackGravity = 0.25f;
	m_DarkRavenFlightSpeed = 0.90f;
	m_DarkRavenAttackSpeed = 3.00f;
	m_DarkRavenWingAnimationSpeed = 0.46f;
	m_DarkRavenAnimationSpeed = 0.46f;
	m_DarkRavenMinTerrainHeight = 80.0f;
	m_DarkRavenMaxOwnerHeight = 280.0f;
}

void CHelperSpeedConfig::SetDarkRavenFlightSpeed(float value, bool save)
{
	(void)save;
	value = ClampHelperFloat(value, 0.10f, 3.0f);

	m_DarkRavenFlightSpeed = value;
	m_Loaded = true;
}

void CHelperSpeedConfig::SetDarkRavenAttackSpeed(float value, bool save)
{
	(void)save;
	value = ClampHelperFloat(value, 0.10f, 3.0f);

	m_DarkRavenAttackSpeed = value;
	m_Loaded = true;
}

void CHelperSpeedConfig::SetDarkRavenWingAnimationSpeed(float value, bool save)
{
	(void)save;
	value = ClampHelperFloat(value, 0.01f, 3.0f);

	m_DarkRavenWingAnimationSpeed = value;
	m_DarkRavenAnimationSpeed = value;
	m_Loaded = true;
}

void CHelperSpeedConfig::SetDarkRavenAnimationSpeed(float value, bool save)
{
	SetDarkRavenWingAnimationSpeed(value, save);
}

void CHelperSpeedConfig::ReloadIfNeeded()
{
	m_Loaded = true;

	if (m_DarkRavenAttackVelocityMax < m_DarkRavenAttackVelocityMin)
	{
		m_DarkRavenAttackVelocityMax = m_DarkRavenAttackVelocityMin;
	}
}

float CHelperSpeedConfig::DarkHorseMoveSpeed() { ReloadIfNeeded(); return m_DarkHorseMoveSpeed; }
float CHelperSpeedConfig::CustomHorseMoveSpeed() { ReloadIfNeeded(); return m_CustomHorseMoveSpeed; }
float CHelperSpeedConfig::UniriaMoveSpeed() { ReloadIfNeeded(); return m_UniriaMoveSpeed; }
float CHelperSpeedConfig::CustomUniriaMoveSpeed() { ReloadIfNeeded(); return m_CustomUniriaMoveSpeed; }
float CHelperSpeedConfig::DarkHorseRunVelocity() { ReloadIfNeeded(); return m_DarkHorseRunVelocity; }
float CHelperSpeedConfig::DarkHorseAttackVelocity() { ReloadIfNeeded(); return m_DarkHorseAttackVelocity; }
float CHelperSpeedConfig::DarkHorseIdleVelocity() { ReloadIfNeeded(); return m_DarkHorseIdleVelocity; }
float CHelperSpeedConfig::UniriaRunVelocity() { ReloadIfNeeded(); return m_UniriaRunVelocity; }
float CHelperSpeedConfig::DinorantRunVelocity() { ReloadIfNeeded(); return m_DinorantRunVelocity; }
float CHelperSpeedConfig::DarkRavenFollowVelocity() { ReloadIfNeeded(); return m_DarkRavenFollowVelocity; }
float CHelperSpeedConfig::DarkRavenStandMaxVelocity() { ReloadIfNeeded(); return m_DarkRavenStandMaxVelocity; }
float CHelperSpeedConfig::DarkRavenStandAcceleration() { ReloadIfNeeded(); return m_DarkRavenStandAcceleration; }
float CHelperSpeedConfig::DarkRavenAttackVelocityMin() { ReloadIfNeeded(); return m_DarkRavenAttackVelocityMin; }
float CHelperSpeedConfig::DarkRavenAttackVelocityMax() { ReloadIfNeeded(); return m_DarkRavenAttackVelocityMax; }
float CHelperSpeedConfig::DarkRavenAttackGravity() { ReloadIfNeeded(); return m_DarkRavenAttackGravity; }
float CHelperSpeedConfig::DarkRavenFlightSpeed() { ReloadIfNeeded(); return m_DarkRavenFlightSpeed; }
float CHelperSpeedConfig::DarkRavenAttackSpeed() { ReloadIfNeeded(); return m_DarkRavenAttackSpeed; }
float CHelperSpeedConfig::DarkRavenWingAnimationSpeed() { ReloadIfNeeded(); return m_DarkRavenWingAnimationSpeed; }
float CHelperSpeedConfig::DarkRavenAnimationSpeed() { ReloadIfNeeded(); return m_DarkRavenWingAnimationSpeed; }
float CHelperSpeedConfig::DarkRavenMinTerrainHeight() { ReloadIfNeeded(); return m_DarkRavenMinTerrainHeight; }
float CHelperSpeedConfig::DarkRavenMaxOwnerHeight() { ReloadIfNeeded(); return m_DarkRavenMaxOwnerHeight; }
