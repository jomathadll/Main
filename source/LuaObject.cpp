#include "stdafx.h"
#include "LuaObject.h"
#include "LuaStack.hpp"

class ObjectClass {
public:
	ObjectClass() : m_Struct(0) {

	};

	ObjectClass(DWORD Struct) {
		m_Struct = (OBJECT*)Struct;
	};

	~ObjectClass() {

	};

	float Alpha() const {
		return m_Struct->Alpha;
	};

	DWORD Mesh() const {
		return m_Struct->BlendMesh;
	};

	float Light() const {
		return m_Struct->BlendMeshLight;
	};

	float TexCoordU() const {
		return m_Struct->BlendMeshTexCoordU;
	};

	float TexCoordV() const {
		return m_Struct->BlendMeshTexCoordV;
	};

	DWORD Hidden() const {
		return m_Struct->HiddenMesh;
	};

	void setAction(int value) const {
		m_Struct->CurrentAction = value;
	};

	int getAction() const {
		return m_Struct->CurrentAction;
	};

	void setTime(float value) const {
		m_Struct->Timer = value;
	};

	float getTime() const {
		return m_Struct->Timer;
	};

	int getSubType() const {
		if (m_Struct->Type == MODEL_PLAYER) {
			return m_Struct->SubType;
		}
		return -1;
	};

	void setScale(float value) const {
		m_Struct->Scale = value;
	};

	float getScale() const {
		return m_Struct->Scale;
	};

	int getType() const {
		return m_Struct->Type;
	};

	bool HideSkin()
	{
		if (!m_Struct)
		{
			return false;
		}
		if (m_Struct->m_bRenderShadow)
		{
			return true;
		}
		return false;
	}

	void setEdgeScale(float value) const {
		m_Struct->m_fEdgeScale = value;
	};

	int getEdgeScale() const {
		return m_Struct->m_fEdgeScale;
	};

	void setAnimation(float value) const {
		m_Struct->m_iAnimation = value;
	};

	float getAnimation() const {
		return m_Struct->m_iAnimation;
	};

	float getAnimationFrame() const {
		return m_Struct->AnimationFrame;
	};

	void HiddenMesh(int value) const {
		m_Struct->HiddenMesh = value;
	};

	int getWeapon() const {
		return m_Struct->Weapon;
	};

	void RenderShadow(bool value) {
		m_Struct->m_bRenderShadow = value;
	}

	OBJECT* getOwner() const {
		return m_Struct->Owner;
	}

	void EyeLeft() const {
		m_Struct->EyeLeft;
	};

	void EyeLeft2() const {
		m_Struct->EyeLeft2;
	};

	void EyeLeft3() const {
		m_Struct->EyeLeft3;
	};

	void EyeRight() const {
		m_Struct->EyeRight;
	};

	void EyeRight2() const {
		m_Struct->EyeRight2;
	};

	void EyeRight3() const {
		m_Struct->EyeRight3;
	};

	int getAnimationChangeCount() const {
		return m_Struct->p_changeImageCount;
	};

	void setAnimationChangeCount(int value) const {
		m_Struct->p_changeImageCount = value;
	};

private:
	OBJECT* m_Struct;
};

float worldTimes() {
	return WorldTime;
}

float GetDoubleRender(float a1, float a2) 
{
	float Return;
	float InitValue;
	
	InitValue = (float)((int32_t)(a2 * 0.01745f * 1000.0f / a1 + WorldTime) % (int32_t)(6283.185546875f / a1)) * 0.001f * a1;
	
	if (InitValue >= 3.14f)
	{
		Return = cos(InitValue);
	}
	else
	{
		Return = -cos(InitValue);
	}

	return (float)((Return + 1.0f) * 0.5f);
}

void RegisterClassObject(lua_State * lua)
{
	luaaa::LuaClass<ObjectClass> luaObject(lua, "Object");
	luaObject.ctor<DWORD>();
	luaObject.fun("getType", &ObjectClass::getType);
	luaObject.fun("Alpha", &ObjectClass::Alpha);
	luaObject.fun("Mesh", &ObjectClass::Mesh);
	luaObject.fun("Light", &ObjectClass::Light);
	luaObject.fun("TexCoordU", &ObjectClass::TexCoordU);
	luaObject.fun("TexCoordV", &ObjectClass::TexCoordV);
	luaObject.fun("Hidden", &ObjectClass::Hidden);
	luaObject.fun("getAction", &ObjectClass::getAction);
	luaObject.fun("setAction", &ObjectClass::setAction);
	luaObject.fun("getTime", &ObjectClass::getTime);
	luaObject.fun("setTime", &ObjectClass::setTime);
	luaObject.fun("getScale", &ObjectClass::getScale);
	luaObject.fun("setScale", &ObjectClass::setScale);
	luaObject.fun("getEdgeScale", &ObjectClass::getEdgeScale);
	luaObject.fun("setEdgeScale", &ObjectClass::setEdgeScale);
	luaObject.fun("getAnimationFrame", &ObjectClass::getAnimationFrame);
	luaObject.fun("getAnimation", &ObjectClass::getAnimation);
	luaObject.fun("setAnimation", &ObjectClass::setAnimation);
	luaObject.fun("HiddenMesh", &ObjectClass::HiddenMesh);
	luaObject.fun("getWeapon", &ObjectClass::getWeapon);
	luaObject.fun("RenderShadow", &ObjectClass::RenderShadow);
	luaObject.fun("getOwner", &ObjectClass::getOwner);
	luaObject.fun("getSubType", &ObjectClass::getSubType);
	luaObject.fun("getAnimationChangeCount", &ObjectClass::getAnimationChangeCount);
	luaObject.fun("setAnimationChangeCount", &ObjectClass::setAnimationChangeCount);

	luaObject.fun("EyeLeft", &ObjectClass::EyeLeft);
	luaObject.fun("EyeLeft2", &ObjectClass::EyeLeft2);
	luaObject.fun("EyeLeft3", &ObjectClass::EyeLeft3);
	luaObject.fun("EyeRight", &ObjectClass::EyeRight);
	luaObject.fun("EyeRight2", &ObjectClass::EyeRight2);
	luaObject.fun("EyeRight3", &ObjectClass::EyeRight3);

	luaaa::LuaModule(lua).fun("worldTime", &worldTimes);
	luaaa::LuaModule(lua).fun("GetDoubleRender", &GetDoubleRender);
}
