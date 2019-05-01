#include "Dxlib.h"
#include "ImageMng.h"
#include "Character.h"


Character::Character()
{
	animFileName["�ҋ@"]		  = "stand";
	animFileName["�O�ړ�"]		  = "move_front";
	animFileName["���ړ�"]	  = "move_back";
	animFileName["�W�����v_��"]	  = "jump";
	animFileName["�W�����v_�O"]	  = "jump_front";
	animFileName["�W�����v_���"] = "jump_back";
}

Character::~Character()
{
}

bool Character::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 10, 5, true);				// stand
	AddAnim("�O�ړ�", 0, 0, 11, 5, true);			// move_front
	AddAnim("���ړ�", 0, 0, 11, 5, true);			// move_back
	AddAnim("�W�����v_��", 0, 0, 12, 5, true);		// jump
	AddAnim("�W�����v_�O", 0, 0, 14, 5, true);		// jump_front
	AddAnim("�W�����v_���", 0, 0, 14, 5, true);	// jump_back
	SetAnim("�ҋ@");
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
