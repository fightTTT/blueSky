#include "SceneMng.h"
#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	switch (lpSceneMng.GetCharID(id))
	{
	case 0:
		characterName = "�_�l��";
		AddAnim("�g��", 0, 0, 11, 5, false, 0, 0);
		spAttackAnimName[0] = "�g��";
		spAttackAnimFileName[0] = "hadou";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�g��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�g��"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("�����r", 0, 0, 4, 5, true, 0, 0);
		spAttackAnimName[1] = "�����r";
		spAttackAnimFileName[1] = "senpu";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("����", 0, 0, 16, 5, false, 0, 0);
		spAttackAnimName[2] = "����";
		spAttackAnimFileName[2] = "shoryu";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["����"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["����"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 1:
		characterName = "�_�l��_��";
		break;

	case 2:
		characterName = "�_�l��_��";
		break;

	case 3:
		characterName = "�_�l��";
		AddAnim("�g��", 0, 0, 11, 5, false, 0, 0);
		spAttackAnimName[0] = "�g��";
		spAttackAnimFileName[0] = "hadou";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["�g��"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["�g��"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("����", 0, 0, 13, 5, false, 0, 0);
		spAttackAnimName[1] = "����";
		spAttackAnimFileName[1] = "throw";
		spAttackType[1] = SKILL_TYPE_PUNCH;

		spAttackCommand[1][DIR_RIGHT].resize(2);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_ACCUMULATE };
		spAttackCommand[1][DIR_LEFT].resize(2);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_ACCUMULATE };

		AddAnim("����", 0, 0, 16, 5, false, 0, 0);
		spAttackAnimName[2] = "����";
		spAttackAnimFileName[2] = "shoryu";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["����"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["����"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 4:
		characterName = "�_�l��_��";
		break;

	case 5:
		characterName = "�_�l��";
		break;

	case 6:
		characterName = "�_�l��";
		break;

	case 7:
		characterName = "�_�l��";
		break;

	default:
		break;
	}

	Init("image/" + characterName + "/�ҋ@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
