#include "Dxlib.h"
#include "ImageMng.h"
#include "Character.h"


Character::Character()
{
}

Character::~Character()
{
}

bool Character::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos)
{
	Obj::Init(fileName, divSize, divCut, pos);
	InitAnim();

	animFileName["待機"] = "stand";
	animFileName["前移動"] = "move_front";
	animFileName["後ろ移動"] = "move_back";
	animFileName["ジャンプ_上"] = "jump";
	animFileName["ジャンプ_前"] = "jump_front";
	animFileName["ジャンプ_後ろ"] = "jump_back";

	std::vector<std::string> animName = { "待機",
										  "前移動",
										  "後ろ移動",
										  "ジャンプ_上",
										  "ジャンプ_前",
										  "ジャンプ_後ろ" };		// ｱﾆﾒｰｼｮﾝ名を要素として持つvector

	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	return true;
}

bool Character::InitAnim(void)
{
	AddAnim("待機", 0, 0, 10, 5, true);
	AddAnim("前移動", 0, 0, 11, 5, true);
	AddAnim("後ろ移動", 0, 0, 11, 5, true);
	AddAnim("ジャンプ_上", 0, 0, 12, 5, true);
	AddAnim("ジャンプ_前", 0, 0, 14, 5, true);
	AddAnim("ジャンプ_後ろ", 0, 0, 14, 5, true);
	SetAnim("待機");
	return true;
}

bool Character::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_CHARACTER);
}

void Character::Draw(void)
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;
	if (animTable.find(animName) != animTable.end())
	{
		id = animCnt / animTable[animName][ANIM_TBL_INV];
		if (animTable[animName][ANIM_TBL_LOOP] || id < animTable[animName][ANIM_TBL_FRAME])
		{
			id %= animTable[animName][ANIM_TBL_FRAME];
		}
		else
		{
			id = animTable[animName][ANIM_TBL_FRAME] - 1;
			animEndFlag = true;
		}
	}

	std::string nowAnim = GetAnim();
	imageName = ("image/" + characterName + "/" + nowAnim + "/" + animFileName[nowAnim] + "_" + std::to_string(id) + ".png");

	if (visible)
	{
		DrawGraph(drawOffset.x + pos.x, drawOffset.y + pos.y, IMAGE_ID(imageName)[0], true);
	}
	animCnt++;
}
