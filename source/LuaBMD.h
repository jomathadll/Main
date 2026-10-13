#pragma once

#include <set>
#include "Lua.h"
#include "ZzzBMD.h"
#include "ZzzEffect.h"
#include "CDirection.h"
#include "MapManager.h"
#include "ZzzCharacter.h"

class BMDClass {

private:
	mutable float backupBodyLight[3];

public:
	BMDClass() : m_Struct(nullptr){

	};

	BMDClass(DWORD Struct) {
		this->m_Struct = (BMD*)Struct;
	};

	~BMDClass() {

	};

	void SaveLight() const {
		if (!m_Struct) {
			return;
		}
		backupBodyLight[0] = m_Struct->BodyLight[0];
		backupBodyLight[1] = m_Struct->BodyLight[1];
		backupBodyLight[2] = m_Struct->BodyLight[2];
	}

	void ResetLight() const {
		if (!m_Struct) {
			return;
		}
		m_Struct->BodyLight[0] = backupBodyLight[0];
		m_Struct->BodyLight[1] = backupBodyLight[1];
		m_Struct->BodyLight[2] = backupBodyLight[2];
	}

	float GetLight(int value) const 
	{
		if (!m_Struct) 
		{
			return 0.0f;
		}

		if (value == 0) 
		{
			return m_Struct->BodyLight[0];
		}
		else if (value == 1) 
		{
			return m_Struct->BodyLight[1];
		}
		else if (value == 2) 
		{
			return m_Struct->BodyLight[2];
		}

		return *(float*)m_Struct->BodyLight;
	};

	float getAnimationKeys(int action)
	{
		if (m_Struct->NumActions < action) {
			return 0;
		}

		return m_Struct->Actions[action].NumAnimationKeys;
	}

	void SetLight(float r, float g, float b) const
	{
		if (!m_Struct)
		{
			return;
		}

		m_Struct->BodyLight[0] = r;
		m_Struct->BodyLight[1] = g;
		m_Struct->BodyLight[2] = b;
	};

	bool HideSkin()
	{
		if (!m_Struct)
		{
			return false;
		}
		if (m_Struct->HideSkin)
		{
			return true;
		}
		return false;
	}

	int Skin()
	{
		if (!m_Struct)
		{
			return -1;
		}
		return m_Struct->Skin;
	}

	float setScale()
	{
		if (!m_Struct)
		{
			return 1.f;
		}
		m_Struct->BodyScale;
	}

	float getScale()
	{
		if (!m_Struct)
		{
			return 1.f;
		}
		return m_Struct->BodyScale;
	}

	void RenderBody(int Flag, float Alpha, int BlendMesh, float BlendMeshLight, float BlendMeshTexCoordU, float BlendMeshTexCoordV, int HiddenMesh, int Texture) const 
	{
		if (!m_Struct) 
		{
			return;
		}

		m_Struct->RenderBody(Flag, Alpha, BlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, HiddenMesh, Texture);
	};

	void RenderMesh(int i, int RenderFlag, float Alpha, int BlendMesh, float BlendMeshLight, float BlendMeshTexCoordU, float BlendMeshTexCoordV, int MeshTexture) const 
	{
		if (!m_Struct) 
		{
			return;
		}

		m_Struct->RenderMesh(i, RenderFlag, Alpha, BlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, MeshTexture);
	};

	void RenderBodyWithoutMuSign(int Flag, float Alpha, int BlendMesh, float BlendMeshLight, float BlendMeshTexCoordU, float BlendMeshTexCoordV, int HiddenMesh, int Texture) const
	{
		if (!m_Struct || m_Struct->NumMeshs == 0)
		{
			return;
		}

		int iBlendMesh = BlendMesh;
		m_Struct->BeginRender(Alpha);
		if (!m_Struct->LightEnable)
		{
			if (Alpha >= 0.99f)
				glColor3fv(m_Struct->BodyLight);
			else
				glColor4f(m_Struct->BodyLight[0], m_Struct->BodyLight[1], m_Struct->BodyLight[2], Alpha);
		}

		for (int i = 0; i < m_Struct->NumMeshs; i++)
		{
			if (i == HiddenMesh || IsMuSignMesh(i))
			{
				continue;
			}

			iBlendMesh = BlendMesh;
			Mesh_t* m = &m_Struct->Meshs[i];
			if (m->m_csTScript != NULL)
			{
				if (m->m_csTScript->getHiddenMesh() == false)
				{
					if (m->m_csTScript->getBright())
					{
						iBlendMesh = i;
					}

					m_Struct->RenderMesh(i, Flag, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, Texture);

					BYTE shadowType = m->m_csTScript->getShadowMesh();
					if (shadowType == SHADOW_RENDER_COLOR)
					{
						DisableAlphaBlend();
						if (Alpha >= 0.99f)
							::glColor3f(0.f, 0.f, 0.f);
						else
							glColor4f(0.f, 0.f, 0.f, Alpha);

						m_Struct->RenderMesh(i, RENDER_COLOR | RENDER_SHADOWMAP, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV);
						::glColor3f(1.f, 1.f, 1.f);
					}
					else if (shadowType == SHADOW_RENDER_TEXTURE)
					{
						DisableAlphaBlend();
						if (Alpha >= 0.99f)
							::glColor3f(0.f, 0.f, 0.f);
						else
							glColor4f(0.f, 0.f, 0.f, Alpha);

						m_Struct->RenderMesh(i, RENDER_TEXTURE | RENDER_SHADOWMAP, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV);
						::glColor3f(1.f, 1.f, 1.f);
					}
				}
			}
			else
			{
				m_Struct->RenderMesh(i, Flag, Alpha, iBlendMesh, BlendMeshLight, BlendMeshTexCoordU, BlendMeshTexCoordV, Texture);
			}
		}

		m_Struct->EndRender();
	};

	void glColor3f() const
	{
		if (!m_Struct)
		{
			return;
		}

		glColor3fv(m_Struct->BodyLight);
	}

	void BeginRender(float value) const {
		//glPushMatrix();
	}

	void EndRender() const {
		//glPopMatrix();
	}

	void setMesh(int value) const
	{
		if (!m_Struct) 
		{
			return;
		}
	
		m_Struct->StreamMesh = value;
	}

	bool IsMuSignMesh(int meshIndex) const
	{
		if (!m_Struct || meshIndex < 0 || meshIndex >= m_Struct->NumMeshs)
		{
			return false;
		}

		Mesh_t* m = &m_Struct->Meshs[meshIndex];
		const int textureIndex = m->Texture;
		if (textureIndex < 0 || textureIndex >= m_Struct->NumMeshs)
		{
			return false;
		}

		const char* fileName = m_Struct->Textures[textureIndex].FileName;
		const char token[] = "musign";
		for (int i = 0; i < 32 && fileName[i] != '\0'; i++)
		{
			int j = 0;
			while (token[j] != '\0' && i + j < 32 && fileName[i + j] != '\0' && ToLowerAscii(fileName[i + j]) == token[j])
			{
				j++;
			}

			if (token[j] == '\0')
			{
				return true;
			}
		}

		return false;
	}

	char ToLowerAscii(char value) const
	{
		if (value >= 'A' && value <= 'Z')
		{
			return value + ('a' - 'A');
		}

		return value;
	}

	void RenderShadowModel() const 
	{
		if (!m_Struct) 
		{
			return;
		}
				
		if (gMapManager.WorldActive != WD_10HEAVEN && gMapManager.InHellas() == FALSE)
		{
			if (!g_Direction.m_CKanturu.IsMayaScene())
			{
				EnableAlphaTest(1);
		
				if (gMapManager.WorldActive == 7)
				{
					glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
				}
				else 
				{
					glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
				}
		
				m_Struct->RenderBodyShadow(-1, -1, -1, -1);
			}
		}
	}

	void TransformPosition(int Link, float PosX, float PosY, float PosZ) 
	{
		if (!m_Struct) {
			return;
		}
		
		if (Link != -1) 
		{
			vec3_t vPos;
		
			Vector(PosX, PosY, PosZ, vPos);
		
			m_Struct->TransformPosition(BoneTransform[Link], vPos, this->Position, 0);
		}
		
		this->LinkObject = Link;
	}

	void TransformPosition2(int Link, float PosX, float PosY, float PosZ) 
	{
		if (!m_Struct) 
		{
			return;
		}
		
		if (Link != -1) {
			vec3_t vPos;
		
			Vector(PosX, PosY, PosZ, vPos);
		
			m_Struct->TransformPosition(BoneTransform[Link], vPos, this->Position, 1);
		}
		
		this->LinkObject = Link;
	}
	void TransformPosition3(int Link, float PosX, float PosY, float PosZ, DWORD ObjectStruct)
	{
		if (!m_Struct) 
		{
			return;
		}
		
		if (Link != -1) {
			vec3_t vPos;
		
			Vector(PosX, PosY, PosZ, vPos);

			auto o = (OBJECT*)ObjectStruct;
		
			if (o == nullptr)
			{
				return;
			}


			m_Struct->TransformPosition(o->BoneTransform[Link], vPos, this->Position);
		}
		
		this->LinkObject = Link;
	}

	void CreateSprites(int Bitmap, float Scale, float LightX, float LightY, float LightZ, DWORD ObjectStruct, int Black) 
	{
		if (!m_Struct || !ObjectStruct) 
		{
			return;
		}
		
		vec3_t Light;
		
		Vector(LightX, LightY, LightZ, Light);

		if (Black == 1) {
			float Luminosity = 1.f;
			Vector(Luminosity * 1.f, Luminosity * 1.f, Luminosity * 1.f, Light);
		}
		
		auto obj = (OBJECT*)ObjectStruct;
		
		if (obj == nullptr) 
		{
			return;
		}
		
		CreateSprite(Bitmap, this->Position, Scale, Light, obj, 0.0, Black);
	}

	void CreateParticles(int Bitmap, int SubType, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct)
	{
		if (!this->m_Struct || !ObjectStruct)
		{
			return;
		}

		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);

		auto obj = (OBJECT*)ObjectStruct;

		if (obj == nullptr)
		{
			return;
		}

		CreateParticle(Bitmap, this->Position, obj->Angle, Light, SubType, Scale, obj);
	}

	void CreateParticles2(int Bitmap, int SubType, float Scale, float LightR, float LightG, float LightB, DWORD ObjectStruct)
	{
		if (!this->m_Struct || !ObjectStruct)
		{
			return;
		}

		vec3_t Light;

		Vector(LightR, LightG, LightB, Light);

		auto obj = (OBJECT*)ObjectStruct;

		if (obj == nullptr)
		{
			return;
		}

		CreateParticle(Bitmap, this->Position, obj->Angle, Light, SubType, Scale);
	}

	void CreateEffects(int Bitmap, int SubType, float LightX, float LightY, float LightZ, DWORD ObjectStruct) 
	{
		if (!this->m_Struct || !ObjectStruct) 
		{
			return;
		}
		
		auto obj = (OBJECT*)ObjectStruct;
		
		if (obj == nullptr) {
			return;
		}
		
		vec3_t Light;
		
		Vector(LightX, LightY, LightZ, Light);
		
		CreateEffect(Bitmap, (this->LinkObject == -1 ? obj->Position : this->Position), obj->Angle, Light, SubType, obj, -1, 0, 0, 0, 0.0, -1);
	}

	void CreateJointEffects(int Bitmap, int SubType, float Size, float LightX, float LightY, float LightZ, DWORD ObjectStruct)
	{
		if (!this->m_Struct || !ObjectStruct)
		{
			return;
		}

		auto obj = (OBJECT*)ObjectStruct;

		if (obj == nullptr) {
			return;
		}

		vec3_t p;
		vec3_t vColor;

		//VectorCopy(p, (this->LinkObject == -1 ? obj->Position : this->Position));

		Vector(LightX, LightY, LightZ, vColor);

		//CreateJoint(Bitmap, p, p, obj->Angle, SubType, obj, Size);
		CreateJoint(Bitmap, this->Position, this->Position, obj->Angle, SubType, obj, Size);
	}

	void CreateEffectsInFenrirUsingSkill(DWORD ObjectStruct, float r, float g, float b, int effctid, int EffectLv, int EffectType, float size, int fl, int fr, int rl, int rr)
	{
		if (!ObjectStruct) 
		{
			return;
		}
		
		auto o = (OBJECT*)ObjectStruct;
		vec3_t vLight;
		vec3_t vPos, vPosition;
		
		Vector(0.0, 0.0, 0.0, vPos);
		vLight[0] = r;
		vLight[1] = g;
		vLight[2] = b;
		
		if (o == nullptr) 
		{
			return;
		}
		
		if (o->CurrentAction == 1)
		{
			if (o->AnimationFrame >= 0.0f && o->AnimationFrame <= 1.5f)
			{
				switch (EffectType) {
				case 0:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					break;
				case 1:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					break;
				case 2:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					break;
				}
			}
		}
		else if (o->CurrentAction == 2)
		{
			if (o->AnimationFrame>1.f && o->AnimationFrame <= 1.4f)
			{
				switch (EffectType) {
				case 0:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					break;
				case 1:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					break;
				case 2:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					break;
				}
			}
			else if (o->AnimationFrame>4.8f && o->AnimationFrame <= 5.2f)
			{
				switch (EffectType) {
				case 0:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					break;
				case 1:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					break;
				case 2:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					break;
				}
			}
		}
	}

	void CreateEffectsInHorseUsingSkill(DWORD ObjectStruct, float r, float g, float b, int effctid, int EffectLv, int EffectType, float size, int fl, int fr, int rl, int rr, float framefront, float framefront2, float frameback, float frameback2)
	{
		if (!ObjectStruct)
		{
			return;
		}

		auto o = (OBJECT*)ObjectStruct;
		vec3_t vLight;
		vec3_t vPos, vPosition;

		Vector(0.0, 0.0, 0.0, vPos);
		vLight[0] = r;
		vLight[1] = g;
		vLight[2] = b;

		if (o == nullptr)
		{
			return;
		}

		if (o->CurrentAction == 1)
		{
			if (o->AnimationFrame >= framefront && o->AnimationFrame <= framefront2)
			{
				switch (EffectType) {
				case 0:

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					break;
				case 1:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					break;
				case 2:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rl], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fr], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					break;
				}
			}
			else if (o->AnimationFrame > frameback && o->AnimationFrame <= frameback2)
			{
				switch (EffectType) {
				case 0:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateSprite(effctid, vPosition, size, vLight, o, 0.0, 0);
					break;
				case 1:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateParticle(effctid, vPosition, o->Angle, vLight, EffectLv, size, o);
					break;
				case 2:
					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[fl], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);

					Vector(0.0f, 0.0f, 0.0f, vPos);
					m_Struct->TransformPosition(BoneTransform[rr], vPos, vPosition, false);
					CreateEffect(effctid, vPosition, o->Angle, vLight, 0, o, -1, 0, 0, 0, 0.0, -1);
					break;
				}
			}
		}

	}

public:
	BMD* m_Struct;
	vec3_t Position;
	int LinkObject;
};

class LuaBMD {
public:
	LuaBMD();
	~LuaBMD();

	void RegisterClassBMD(lua_State* lua);
};
