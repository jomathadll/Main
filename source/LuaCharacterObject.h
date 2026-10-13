#pragma once

#include "Lua.h"

class characterObject
{
public:
	characterObject() : characterObj(nullptr)
	{

	};

	characterObject(DWORD objClass)
	{
		this->characterObj = (CHARACTER*)objClass;
	};

	~characterObject() {

	}

	int getSword() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Weapon[0].Type;
	}

	int getShield() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Weapon[1].Type;
	}

	int getHelm() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->BodyPart[BODYPART_HELM].Type;
	}

	int getArmor() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->BodyPart[BODYPART_ARMOR].Type;
	}

	int getPants() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->BodyPart[BODYPART_PANTS].Type;
	}

	int getGloves() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->BodyPart[BODYPART_GLOVES].Type;
	}

	int getBoots() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->BodyPart[BODYPART_BOOTS].Type;
	}

	int getWing() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Wing.Type;
	}

	int getHelper() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Helper.Type;
	}

	int getMonsterIndex() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->MonsterIndex;
	}

	int getSafeZone() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->SafeZone;
	}

	int getClass() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Class;
	}

	int getSkin() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Skin;
	}

	int getCtlCode() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->CtlCode;
	}

	int getAction() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Action;
	}

	int getLevel() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->Level;
	}

	bool getRun() const
	{
		if (this->characterObj == nullptr)
		{
			return false;
		}

		return this->characterObj->Run;
	}

	int getEtcPart() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->EtcPart;
	}

	int getPatente() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		if (this->characterObj->EtcPart != 0)
		{
			return this->characterObj->EtcPart - 19;
		}
		else
		{
			return 0;

		}
	}

	int getPK() const
	{
		if (this->characterObj == nullptr)
		{
			return 0;
		}

		return this->characterObj->PK;
	}

	std::string getName() const
	{
		if (this->characterObj == nullptr)
		{
			return "";
		}

		return this->characterObj->ID;
	}

private:
	CHARACTER* characterObj;
};

void RegisterLuaCharacterObject(lua_State* lua);