#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	characterName = "�_�l��";

	AddAnim("�g��", 0, 0, 11, 5, false, 0, 0);
	spAttackAnimName[0] = "�g��";
	spAttackAnimFileName[0] = "hadou";
	spAttackType[0] = SKILL_TYPE_PUNCH;

	spAttackCommand[0][DIR_RIGHT].resize(3);
	spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
	spAttackCommand[0][DIR_LEFT].resize(3);
	spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

	AddAnim("����", 0, 0, 13, 5, false, 0, 0);
	spAttackAnimName[1] = "����";
	spAttackAnimFileName[1] = "throw";
	spAttackType[1] = SKILL_TYPE_PUNCH;

	spAttackCommand[1][DIR_RIGHT].resize(3);
	spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_ACCUMULATE, COM_DIR_ACCUMULATE };
	spAttackCommand[1][DIR_LEFT].resize(3);
	spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_ACCUMULATE, COM_DIR_ACCUMULATE };

	AddAnim("����", 0, 0, 16, 5, false, 0, 0);
	spAttackAnimName[2] = "����";
	spAttackAnimFileName[2] = "shoryu";
	spAttackType[2] = SKILL_TYPE_PUNCH;

	spAttackCommand[2][DIR_RIGHT].resize(3);
	spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
	spAttackCommand[2][DIR_LEFT].resize(3);
	spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };

	Init("image/�_�l��/�ҋ@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
