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

	animFileName["�ҋ@"] = "stand";
	animFileName["�O�ړ�"] = "move_front";
	animFileName["���ړ�"] = "move_back";
	animFileName["�W�����v_��"] = "jump";
	animFileName["�W�����v_�O"] = "jump_front";
	animFileName["�W�����v_���"] = "jump_back";
	animFileName["�p���`_��"] = "punch_small";
	animFileName["�p���`_��"] = "punch_big";
	animFileName["�g����"] = "hadoken";
	animFileName["���������r"] = "tatsumaki_senpukyaku";
	animFileName["������"] = "syoryuken";

	std::vector<std::string> animName = { "�ҋ@",
										  "�O�ړ�",
										  "���ړ�",
										  "�W�����v_��",
										  "�W�����v_�O",
										  "�W�����v_���",
										  "�p���`_��", 
										  "�p���`_��", 
										  "�g����", 
										  "���������r", 
										  "������"};		// ��Ұ��ݖ���v�f�Ƃ��Ď���vector

	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	return true;
}

bool Character::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 10, 5, true);
	AddAnim("�O�ړ�", 0, 0, 11, 5, true);
	AddAnim("���ړ�", 0, 0, 11, 5, true);
	AddAnim("�W�����v_��", 0, 0, 12, 5, true);
	AddAnim("�W�����v_�O", 0, 0, 14, 5, true);
	AddAnim("�W�����v_���", 0, 0, 14, 5, true);
	AddAnim("�p���`_��", 0, 0, 4, 5, false);
	AddAnim("�p���`_��", 0, 0, 7, 5, false);
	AddAnim("�g����", 0, 0, 12, 5, false);
	AddAnim("���������r", 0, 0, 13, 5, false);
	AddAnim("������", 0, 0, 14, 5, false);
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
		//DrawGraph(drawOffset.x + pos.x, drawOffset.y + pos.y, IMAGE_ID(imageName)[0], true);
		DrawRotaGraph(pos.x, pos.y - 178 / 2, 1.0, 0.0, IMAGE_ID(imageName)[0], true, dir == DIR_RIGHT);
	}
	animCnt++;

	if (longAttackFlag)
	{
		DrawBox(longAttackPos.x, longAttackPos.y - 178 / 2, longAttackPos.x + 50, longAttackPos.y + 30, 0xffffff, true);
	}
}
