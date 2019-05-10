#include "AIStickHuman.h"



AIStickHuman::AIStickHuman(VECTOR2 pos, VECTOR2 offset, DIR charaDir)
{
	characterName = "ñ_êlä‘";

	AddAnim("îgìÆ", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "îgìÆ";
	spAttackAnimFileName[0] = "hadou";

	AddAnim("ìäÇ∞", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "ìäÇ∞";
	spAttackAnimFileName[1] = "throw";

	AddAnim("è∏ó≥", 0, 0, 16, 5, false);
	spAttackAnimName[2] = "è∏ó≥";
	spAttackAnimFileName[2] = "shoryu";

	Init("image/ñ_êlä‘/ë“ã@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	dir = charaDir;
}

AIStickHuman::~AIStickHuman()
{
}
