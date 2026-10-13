#pragma once

#include <list>

BoostSmartPointer(HELPER_VIEW);

class HELPER_VIEW
{
public:
	static HELPER_VIEWPtr Make();

	HELPER_VIEW();


	virtual ~HELPER_VIEW()
	{
		this->Relese();
	};

	void Init()
	{
		if (m_Obj == nullptr)
		{

			OBJECT* CreateClass = new OBJECT();

			if (CreateClass)
			{
				m_Obj = new OBJECT();
			}
			else
			{
				m_Obj = nullptr;
			}
		}
	};

	bool Create(int Index, int ModelIndex, vec3_t Position, CHARACTER* m_Owner, int SubType, int Separate = 0);

	void Render();
	void Update();
	void UpdateModel(OBJECT* model);
	void UpdateMove(OBJECT* model);

	void WorkMoviment(OBJECT* ObjectStruct, float MaxPos, int type, int movement, int state, int SubType);
	void WorkdMovimentMiniature(OBJECT* ObjectStruct, float MaxPos);

	bool IsLive() { return m_Obj->Live; };
	bool IsSameOwner(OBJECT*Owner) { return (Owner == m_Obj->Owner) ? 1 : 0; };
	bool IsSameObject(OBJECT*Owner, int itemType) { return (Owner == m_Obj->Owner && (itemType == m_itemType || itemType + MODEL_ITEM == m_itemType)) ? 1 : 0; };
	bool IsSameObject(OBJECT*Owner) { return (Owner == m_Obj->Owner) ? 1 : 0; };
	void SetScale(float scale) 
	{
		m_Obj->Scale = scale; 
		this->PetOriginalSize = scale;
	};

	void SetMiniature(int miniature, float sizeminiature, float velocity) 
	{
		this->Miniature = miniature;
		this->SizeMiniature = sizeminiature;
		this->VelocityMiniature = velocity;
	}

	void Relese() 
	{
		m_itemType = -1;
		ModelIndex = -1;

		if (m_pOwner) 
		{
			m_pOwner = nullptr;
		}

		if (m_Obj) 
		{
			m_Obj->Live = false;
			m_Obj->Owner = nullptr;

			delete m_Obj;

			m_Obj = nullptr;
		}
	}

	int m_itemType;
	int ModelIndex;
	OBJECT* m_Obj;
	CHARACTER* m_pOwner;

	DWORD m_startTick;

	int m_Type;
	int m_SubType;
	int m_Movement;
	float m_HeightFloor;
	float PetOriginalSize;

	int Miniature;
	float SizeMiniature;
	float VelocityMiniature;

	bool  shrinking;
	bool  shrinkDirection;
	float shrinkStartTime;
	float shrinkDuration;
	bool  isMiniature;
	bool  lastSafeState;
	float shrinkStartScale;
};
