#include "Dxlib.h"
#include "ImageMng.h"
#include "Character.h"


Character::Character()
{
	animFileName["待機"]		  = "stand";
	animFileName["前移動"]		  = "move_front";
	animFileName["後ろ移動"]	  = "move_back";
	animFileName["ジャンプ_上"]	  = "jump";
	animFileName["ジャンプ_前"]	  = "jump_front";
	animFileName["ジャンプ_後ろ"] = "jump_back";
}

Character::~Character()
{
}

bool Character::InitAnim(void)
{
	AddAnim("待機", 0, 0, 10, 5, true);				// stand
	AddAnim("前移動", 0, 0, 11, 5, true);			// move_front
	AddAnim("後ろ移動", 0, 0, 11, 5, true);			// move_back
	AddAnim("ジャンプ_上", 0, 0, 12, 5, true);		// jump
	AddAnim("ジャンプ_前", 0, 0, 14, 5, true);		// jump_front
	AddAnim("ジャンプ_後ろ", 0, 0, 14, 5, true);	// jump_back
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
