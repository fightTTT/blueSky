#include "AIStickHuman.h"



AIStickHuman::AIStickHuman(VECTOR2 pos, VECTOR2 offset, DIR charaDir)
{
	characterName = "�_�l��";

	AddAnim("�g��", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "�g��";
	spAttackAnimFileName[0] = "hadou";

	AddAnim("����", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "����";
	spAttackAnimFileName[1] = "throw";

	AddAnim("����", 0, 0, 16, 5, false);
	spAttackAnimName[2] = "����";
	spAttackAnimFileName[2] = "shoryu";

	Init("image/�_�l��/�ҋ@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	dir = charaDir;
}

AIStickHuman::~AIStickHuman()
{
}
