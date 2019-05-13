#include "AICharacter.h"
#include "ImageMng.h"
#include "Shot.h"

#include "AIState.h"
#include "MoveState.h"
#include "WaitState.h"

#include "DxLib.h"

AICharacter::AICharacter(VECTOR2 offset) : Obj(offset)
{
	longAttackFlag = false;
	shotJumpFlag = false;
	dirChangeFlag = true;
	jumpType = JUMP_TYPE_RAND;

	padID = PAD_AI;

	ChangeState(MoveState::GetInstance());
}

AICharacter::~AICharacter()
{
}

bool AICharacter::CheckObjType(OBJ_TYPE type)
{
	return (type == OBJ_TYPE_AICHARACTER);
}

void AICharacter::SetMove(const GameCtrl & ctl, weekListObj objList)
{
	if (dirChangeFlag)
	{
		// �L�����̌����ύX
		dir = tmpDir;
	}

	if (state)
	{
		state->Update(this);
	}

	if (longAttackFlag)
	{
		// �������U��
		AddObjList()(objList, std::make_unique<Shot>(pos, drawOffset, dir, padID));
		longAttackFlag = false;
	}
}

void AICharacter::Draw()
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;
	VECTOR2 animOffset(0, 0);
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

		if (dir == DIR_RIGHT)
		{
			animOffset = { animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
		else
		{
			animOffset = { -animTable[animName][ANIM_TBL_OFFSET_X] , animTable[animName][ANIM_TBL_OFFSET_Y] };
		}
	}

	std::string nowAnim = GetAnim();
	imageName = ("image/" + characterName + "/" + nowAnim + "/" + animFileName[nowAnim] + "_" + std::to_string(id) + ".png");

	// DIR��RIGHT�̏ꍇ�摜�𔽓]
	if (dir == DIR_LEFT)
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (visible)
	{
		SetDrawBright(100, 255, 100);
		DrawRotaGraph(drawOffset.x + animOffset.x + pos.x + (divSize.x / 2), drawOffset.y + animOffset.y + + pos.y + (divSize.y / 2), 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
		SetDrawBright(255, 255, 255);
	}
	animCnt++;
}

void AICharacter::ChangeState(AIState * s)
{
	state = s;
	state->Init(this);
}

bool AICharacter::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 6, 5, true, 0, 0);
	AddAnim("�O�ړ�", 0, 0, 8, 5, true, 0, 0);
	AddAnim("���ړ�", 0, 0, 8, 5, true, 0, 0);
	AddAnim("�W�����v_��", 0, 0, 10, 6, true, 0, 0);
	AddAnim("�W�����v_�O", 0, 0, 6, 6, true, 0, 0);
	AddAnim("�W�����v_���", 0, 0, 6, 6, true, 0, 0);
	AddAnim("���Ⴊ��", 0, 0, 6, 5, true, 0, 0);
	AddAnim("���Ⴊ��_���", 0, 0, 1, 5, true, 0, 0);
	AddAnim("���Ⴊ�ݎn��", 0, 0, 3, 5, false, 0, 0);
	AddAnim("�����n��", 0, 0, 3, 5, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 7, 5, false, 0, 0);
	AddAnim("�p���`_��", 0, 0, 11, 5, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 8, 5, false, 0, 0);
	AddAnim("�p���`_��_��", 0, 0, 10, 5, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 8, 5, false, 0, 0);
	AddAnim("�p���`_��_���Ⴊ��", 0, 0, 10, 5, false, 0, 0);
	AddAnim("�L�b�N_��", 0, 0, 9, 5, false, 64, 0);
	AddAnim("�L�b�N_��", 0, 0, 11, 5, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 5, 5, false, 0, 0);
	AddAnim("�L�b�N_��_��", 0, 0, 7, 5, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 7, 5, false, 0, 0);
	AddAnim("�L�b�N_��_���Ⴊ��", 0, 0, 10, 5, false, 0, 0);
	AddAnim("�K�[�h_����", 0, 0, 1, 5, true, 0, 0);
	AddAnim("�K�[�h_���Ⴊ��", 0, 0, 1, 5, true, 0, 0);
	SetAnim("�ҋ@");

	return true;
}

bool AICharacter::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

	// �ʏ�̃A�N�V����
	animFileName["�ҋ@"] = "stand";
	animFileName["�O�ړ�"] = "move_front";
	animFileName["���ړ�"] = "move_back";
	animFileName["�W�����v_��"] = "jump";
	animFileName["�W�����v_�O"] = "jump_front";
	animFileName["�W�����v_���"] = "jump_back";
	animFileName["���Ⴊ��"] = "squat";
	animFileName["���Ⴊ��_���"] = "squat_back";
	animFileName["���Ⴊ�ݎn��"] = "squat_start";
	animFileName["�����n��"] = "stand_start";
	animFileName["�p���`_��"] = "punch_small";
	animFileName["�p���`_��"] = "punch_big";
	animFileName["�p���`_��_��"] = "punch_small_air";
	animFileName["�p���`_��_��"] = "punch_big_air";
	animFileName["�p���`_��_���Ⴊ��"] = "punch_small_squat";
	animFileName["�p���`_��_���Ⴊ��"] = "punch_big_squat";
	animFileName["�L�b�N_��"] = "kick_small";
	animFileName["�L�b�N_��"] = "kick_big";
	animFileName["�L�b�N_��_��"] = "kick_small_air";
	animFileName["�L�b�N_��_��"] = "kick_big_air";
	animFileName["�L�b�N_��_���Ⴊ��"] = "kick_small_squat";
	animFileName["�L�b�N_��_���Ⴊ��"] = "kick_big_squat";
	animFileName["�K�[�h_����"] = "guard";
	animFileName["�K�[�h_���Ⴊ��"] = "guard_squat";

	std::vector<std::string> animName = { "�ҋ@",
										  "�O�ړ�",
										  "���ړ�",
										  "�W�����v_��",
										  "�W�����v_�O",
										  "�W�����v_���",
										  "���Ⴊ��",
										  "���Ⴊ��_���",
										  "���Ⴊ�ݎn��",
										  "�����n��",
										  "�p���`_��",
										  "�p���`_��",
										  "�p���`_��_��",
										  "�p���`_��_��",
										  "�p���`_��_���Ⴊ��",
										  "�p���`_��_���Ⴊ��",
										  "�L�b�N_��",
										  "�L�b�N_��",
										  "�L�b�N_��_��",
										  "�L�b�N_��_��",
										  "�L�b�N_��_���Ⴊ��",
										  "�L�b�N_��_���Ⴊ��",
										  "�K�[�h_����",
										  "�K�[�h_���Ⴊ��" };		// ��Ұ��ݖ���v�f�Ƃ��Ď���vector

	// �K�E�Z�n
	if ((spAttackAnimName[0] != "�Z1") && (spAttackAnimFileName[0] != "waza_1"))
	{
		animFileName[spAttackAnimName[0]] = spAttackAnimFileName[0];
		animName.push_back(spAttackAnimName[0]);
	}
	if ((spAttackAnimName[1] != "�Z2") && (spAttackAnimFileName[1] != "waza_2"))
	{
		animFileName[spAttackAnimName[1]] = spAttackAnimFileName[1];
		animName.push_back(spAttackAnimName[1]);
	}
	if ((spAttackAnimName[2] != "�Z3") && (spAttackAnimFileName[2] != "waza_3"))
	{
		animFileName[spAttackAnimName[2]] = spAttackAnimFileName[2];
		animName.push_back(spAttackAnimName[2]);
	}

	// �ꊇ�ǂݍ���
	lpImageMng.LoadImageCharacterAll(characterName, animName, animFileName);

	return true;
}
