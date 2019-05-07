#include "Dxlib.h"
#include "ImageMng.h"
#include "Character.h"


Character::Character(VECTOR2 offset) : Obj(offset)
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
	animFileName["パンチ_小"] = "punch_small";
	animFileName["パンチ_大"] = "punch_big";
	animFileName["波動拳"] = "hadoken";
	animFileName["竜巻旋風脚"] = "tatsumaki_senpukyaku";
	animFileName["昇竜拳"] = "syoryuken";

	std::vector<std::string> animName = { "待機",
										  "前移動",
										  "後ろ移動",
										  "ジャンプ_上",
										  "ジャンプ_前",
										  "ジャンプ_後ろ",
										  "パンチ_小", 
										  "パンチ_大", 
										  "波動拳", 
										  "竜巻旋風脚", 
										  "昇竜拳"};		// ｱﾆﾒｰｼｮﾝ名を要素として持つvector

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
	AddAnim("パンチ_小", 0, 0, 4, 5, false);
	AddAnim("パンチ_大", 0, 0, 7, 5, false);
	AddAnim("波動拳", 0, 0, 12, 5, false);
	AddAnim("竜巻旋風脚", 0, 0, 13, 5, false);
	AddAnim("昇竜拳", 0, 0, 14, 5, false);
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
		//DrawGraph(drawOffset.x + pos.x, drawOffset.y + pos.y, IMAGE_ID(imageName)[0], true);
		DrawRotaGraph(pos.x, pos.y - 178 / 2, 1.0, 0.0, IMAGE_ID(imageName)[0], true, dir == DIR_RIGHT);
	}
	animCnt++;

	if (longAttackFlag)
	{
		DrawBox(longAttackPos.x, longAttackPos.y - 178 / 2, longAttackPos.x + 50, longAttackPos.y + 30, 0xffffff, true);
	}
}
