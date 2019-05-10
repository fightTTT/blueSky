#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	characterName = "ñ_êlä‘";

	AddAnim("îgìÆ", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "îgìÆ";
	spAttackAnimFileName[0] = "hadou";
	spAttackCommand[0][DIR_RIGHT].resize(3);
	spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
	spAttackCommand[0][DIR_LEFT].resize(3);
	spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

	AddAnim("ìäÇ∞", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "ìäÇ∞";
	spAttackAnimFileName[1] = "throw";
	spAttackCommand[1][DIR_RIGHT].resize(3);
	spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
	spAttackCommand[1][DIR_LEFT].resize(3);
	spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

	AddAnim("è∏ó≥", 0, 0, 16, 5, false);
	spAttackAnimName[2] = "è∏ó≥";
	spAttackAnimFileName[2] = "shoryu";
	spAttackCommand[2][DIR_RIGHT].resize(3);
	spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
	spAttackCommand[2][DIR_LEFT].resize(3);
	spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };

	Init("image/ñ_êlä‘/ë“ã@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
