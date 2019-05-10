#include "AIRyu.h"

AIRyu::AIRyu(VECTOR2 pos, VECTOR2 offset, DIR charaDir) : AICharacter(offset)
{
	characterName = "ƒŠƒ…ƒE";

	AddAnim("”g“®Œ", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "”g“®Œ";
	spAttackAnimFileName[0] = "hadoken";

	AddAnim("—³Šªù•—‹r", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "—³Šªù•—‹r";
	spAttackAnimFileName[1] = "tatsumaki_senpukyaku";

	AddAnim("¸—³Œ", 0, 0, 14, 5, false);
	spAttackAnimName[2] = "¸—³Œ";
	spAttackAnimFileName[2] = "syoryuken";

	Init("image/ƒŠƒ…ƒE/‘Ò‹@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false);

	dir = charaDir;
}

AIRyu::~AIRyu()
{
}
