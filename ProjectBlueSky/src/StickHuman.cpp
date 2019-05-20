#include "SceneMng.h"
#include "StickHuman.h"


StickHuman::StickHuman(VECTOR2 pos, VECTOR2 offset, PAD_ID id, DIR charaDir) : Character(offset)
{
	switch (lpSceneMng.GetCharID(id))
	{
	case 0:
		characterName = "ñ_êlä‘";
		AddAnim("îgìÆ", 0, 0, 11, 5, false, 0, 0);
		spAttackAnimName[0] = "îgìÆ";
		spAttackAnimFileName[0] = "hadou";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["îgìÆ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["îgìÆ"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("ê˘ïóãr", 0, 0, 4, 5, true, 0, 0);
		spAttackAnimName[1] = "ê˘ïóãr";
		spAttackAnimFileName[1] = "senpu";
		spAttackType[1] = SKILL_TYPE_KICK;

		spAttackCommand[1][DIR_RIGHT].resize(3);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };
		spAttackCommand[1][DIR_LEFT].resize(3);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };

		AddAnim("è∏ó≥", 0, 0, 16, 5, false, 0, 0);
		spAttackAnimName[2] = "è∏ó≥";
		spAttackAnimFileName[2] = "shoryu";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["è∏ó≥"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["è∏ó≥"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 1:
		characterName = "ñ_êlä‘_çï";
		break;

	case 2:
		characterName = "ñ_êlä‘_ê‘";
		break;

	case 3:
		characterName = "ñ_êlä‘";
		AddAnim("îgìÆ", 0, 0, 11, 5, false, 0, 0);
		spAttackAnimName[0] = "îgìÆ";
		spAttackAnimFileName[0] = "hadou";
		spAttackType[0] = SKILL_TYPE_PUNCH;
		animAttributeTbl["îgìÆ"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["îgìÆ"][1] = ANIM_ATTRIBUTE_NON;

		spAttackCommand[0][DIR_RIGHT].resize(3);
		spAttackCommand[0][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_RIGHT_DOWN, COM_DIR_RIGHT };
		spAttackCommand[0][DIR_LEFT].resize(3);
		spAttackCommand[0][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_LEFT_DOWN, COM_DIR_LEFT };

		AddAnim("ìäÇ∞", 0, 0, 13, 5, false, 0, 0);
		spAttackAnimName[1] = "ìäÇ∞";
		spAttackAnimFileName[1] = "throw";
		spAttackType[1] = SKILL_TYPE_PUNCH;

		spAttackCommand[1][DIR_RIGHT].resize(2);
		spAttackCommand[1][DIR_RIGHT] = { COM_DIR_DOWN, COM_DIR_ACCUMULATE };
		spAttackCommand[1][DIR_LEFT].resize(2);
		spAttackCommand[1][DIR_LEFT] = { COM_DIR_DOWN, COM_DIR_ACCUMULATE };

		AddAnim("è∏ó≥", 0, 0, 16, 5, false, 0, 0);
		spAttackAnimName[2] = "è∏ó≥";
		spAttackAnimFileName[2] = "shoryu";
		spAttackType[2] = SKILL_TYPE_PUNCH;
		animAttributeTbl["è∏ó≥"][0] = ANIM_ATTRIBUTE_STAND;
		animAttributeTbl["è∏ó≥"][1] = ANIM_ATTRIBUTE_INVINCIBLE;

		spAttackCommand[2][DIR_RIGHT].resize(3);
		spAttackCommand[2][DIR_RIGHT] = { COM_DIR_RIGHT, COM_DIR_DOWN, COM_DIR_RIGHT_DOWN };
		spAttackCommand[2][DIR_LEFT].resize(3);
		spAttackCommand[2][DIR_LEFT] = { COM_DIR_LEFT, COM_DIR_DOWN, COM_DIR_LEFT_DOWN };
		break;

	case 4:
		characterName = "ñ_êlä‘_óŒ";
		break;

	case 5:
		characterName = "ñ_êlä‘";
		break;

	case 6:
		characterName = "ñ_êlä‘";
		break;

	case 7:
		characterName = "ñ_êlä‘";
		break;

	default:
		break;
	}

	Init("image/" + characterName + "/ë“ã@/stand_0.png", VECTOR2(256, 256), VECTOR2(1, 1), VECTOR2(pos.x, pos.y), false, id);

	dir = charaDir;
}

StickHuman::~StickHuman()
{
}
