#include "AIRyu.h"

AIRyu::AIRyu(VECTOR2 pos, VECTOR2 offset, DIR charaDir) : AICharacter(offset)
{
	characterName = "�����E";

	AddAnim("�g����", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "�g����";
	spAttackAnimFileName[0] = "hadoken";

	AddAnim("���������r", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "���������r";
	spAttackAnimFileName[1] = "tatsumaki_senpukyaku";

	AddAnim("������", 0, 0, 14, 5, false);
	spAttackAnimName[2] = "������";
	spAttackAnimFileName[2] = "syoryuken";

	Init("image/�����E/�ҋ@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	dir = charaDir;
}

AIRyu::~AIRyu()
{
}
