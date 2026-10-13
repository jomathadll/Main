#pragma once

class CHelperSpeedConfig
{
public:
	CHelperSpeedConfig();

	float DarkHorseMoveSpeed();
	float CustomHorseMoveSpeed();
	float UniriaMoveSpeed();
	float CustomUniriaMoveSpeed();

	float DarkHorseRunVelocity();
	float DarkHorseAttackVelocity();
	float DarkHorseIdleVelocity();
	float UniriaRunVelocity();
	float DinorantRunVelocity();

	float DarkRavenFollowVelocity();
	float DarkRavenStandMaxVelocity();
	float DarkRavenStandAcceleration();
	float DarkRavenAttackVelocityMin();
	float DarkRavenAttackVelocityMax();
	float DarkRavenAttackGravity();
	float DarkRavenFlightSpeed();
	float DarkRavenAttackSpeed();
	float DarkRavenWingAnimationSpeed();
	float DarkRavenAnimationSpeed();
	float DarkRavenMinTerrainHeight();
	float DarkRavenMaxOwnerHeight();
	void SetDarkRavenFlightSpeed(float value, bool save);
	void SetDarkRavenAttackSpeed(float value, bool save);
	void SetDarkRavenWingAnimationSpeed(float value, bool save);
	void SetDarkRavenAnimationSpeed(float value, bool save);

private:
	void ReloadIfNeeded();

	bool m_Loaded;

	float m_DarkHorseMoveSpeed;
	float m_CustomHorseMoveSpeed;
	float m_UniriaMoveSpeed;
	float m_CustomUniriaMoveSpeed;
	float m_DarkHorseRunVelocity;
	float m_DarkHorseAttackVelocity;
	float m_DarkHorseIdleVelocity;
	float m_UniriaRunVelocity;
	float m_DinorantRunVelocity;
	float m_DarkRavenFollowVelocity;
	float m_DarkRavenStandMaxVelocity;
	float m_DarkRavenStandAcceleration;
	float m_DarkRavenAttackVelocityMin;
	float m_DarkRavenAttackVelocityMax;
	float m_DarkRavenAttackGravity;
	float m_DarkRavenFlightSpeed;
	float m_DarkRavenAttackSpeed;
	float m_DarkRavenWingAnimationSpeed;
	float m_DarkRavenAnimationSpeed;
	float m_DarkRavenMinTerrainHeight;
	float m_DarkRavenMaxOwnerHeight;
};

extern CHelperSpeedConfig gHelperSpeedConfig;
