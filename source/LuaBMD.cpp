#include "stdafx.h"
#include "ZzzBMD.h"
#include "LuaBMD.h"
#include "LuaStack.hpp"

LuaBMD::LuaBMD()
{
}

LuaBMD::~LuaBMD()
{
}

void LuaBMD::RegisterClassBMD(lua_State * lua)
{
	luaaa::LuaClass<BMDClass> luaBMD(lua, "BMD");
	luaBMD.ctor<DWORD>();
	luaBMD.fun("GetAnimationKeys", &BMDClass::getAnimationKeys);
	luaBMD.fun("GetLight", &BMDClass::GetLight);
	luaBMD.fun("SetLight", &BMDClass::SetLight);
	luaBMD.fun("SaveLight", &BMDClass::SaveLight);
	luaBMD.fun("ResetLight", &BMDClass::ResetLight);
	luaBMD.fun("RenderBody", &BMDClass::RenderBody);
	luaBMD.fun("RenderBodyWithoutMuSign", &BMDClass::RenderBodyWithoutMuSign);
	luaBMD.fun("RenderMesh", &BMDClass::RenderMesh);
	luaBMD.fun("glColor3fv", &BMDClass::glColor3f);
	luaBMD.fun("BeginRender", &BMDClass::BeginRender);
	luaBMD.fun("EndRender", &BMDClass::EndRender);
	luaBMD.fun("setMesh", &BMDClass::setMesh);
	luaBMD.fun("setScale", &BMDClass::setScale);
	luaBMD.fun("getScale", &BMDClass::getScale);
	luaBMD.fun("RenderShadowModel", &BMDClass::RenderShadowModel);
	luaBMD.fun("TransformPosition", &BMDClass::TransformPosition);
	luaBMD.fun("TransformPosition2", &BMDClass::TransformPosition2);
	luaBMD.fun("TransformPosition3", &BMDClass::TransformPosition3);
	luaBMD.fun("CreateSprite", &BMDClass::CreateSprites);
	luaBMD.fun("CreateParticle", &BMDClass::CreateParticles);
	luaBMD.fun("CreateParticle2", &BMDClass::CreateParticles2);
	luaBMD.fun("CreateEffect", &BMDClass::CreateEffects);
	luaBMD.fun("CreateJoint", &BMDClass::CreateJointEffects);
	luaBMD.fun("CreateEffectsInFenrirUsingSkill", &BMDClass::CreateEffectsInFenrirUsingSkill);
	luaBMD.fun("CreateEffectsInHorseUsingSkill", &BMDClass::CreateEffectsInHorseUsingSkill);

	luaBMD.fun("IsInventory", &BMDClass::HideSkin);
	luaBMD.fun("GetSkin", &BMDClass::Skin);
}
