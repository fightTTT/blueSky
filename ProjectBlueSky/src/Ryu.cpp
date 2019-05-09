#include "Ryu.h"

Ryu::Ryu(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	characterName = "ÉäÉÖÉE";

	AddAnim("îgìÆåù", 0, 0, 11, 5, false);
	spAttackAnimName[0] = "îgìÆåù";
	spAttackAnimFileName[0] = "hadoken";
	spAttackCommand[0][DIR_RIGHT].resize(3);
	spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
	spAttackCommand[0][DIR_LEFT].resize(3);
	spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

	AddAnim("ó≥ä™ê˘ïóãr", 0, 0, 13, 5, false);
	spAttackAnimName[1] = "ó≥ä™ê˘ïóãr";
	spAttackAnimFileName[1] = "tatsumaki_senpukyaku";
	spAttackCommand[1][DIR_RIGHT].resize(3);
	spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
	spAttackCommand[1][DIR_LEFT].resize(3);
	spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

	AddAnim("è∏ó≥åù", 0, 0, 14, 5, false);
	spAttackAnimName[2] = "è∏ó≥åù";
	spAttackAnimFileName[2] = "syoryuken";
	spAttackCommand[2][DIR_RIGHT].resize(3);
	spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
	spAttackCommand[2][DIR_LEFT].resize(3);
	spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
	

	Init("image/ÉäÉÖÉE/ë“ã@/stand_0.png", VECTOR2(290, 178), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

Ryu::~Ryu()
{
}
