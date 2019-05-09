#include "AICharacter.h"
#include "ImageMng.h"
#include "Shot.h"
#include "AIState.h"
#include "MoveState.h"

#include "DxLib.h"

AICharacter::AICharacter()
{
	AIStateTime = 0;
	moveStateTime = 0;
	LongAttackFlag = false;
	moveDirFlag = true;
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
	dir = tmpDir;

	if (state)
	{
		state->Update(this);
	}

	ChangeState(MoveState::GetInstance());

	switch (AIStateType)
	{
	case AI_STATE_NONE:
		SetAnim("�ҋ@");
		break;
	case AI_STATE_MOVE:
		Move();
		break;
	case AI_STATE_ATTACK:
		if (!(LongAttackFlag) && (animTable[GetAnim()][ANIM_TBL_LOOP] || animEndFlag))
		{
			AddObjList()(objList, std::make_unique<Shot>(pos, dir));
			SetAnim(spAttackAnimName[0]);
			LongAttackFlag = true;
		}
		break;
	case AI_STATE_JUMP:
		break;
	case AI_STATE_MAX:
		break;
	default:
		break;
	}

	if (LongAttackFlag && animEndFlag)
	{
		AIStateType = AI_STATE_MOVE;
		LongAttackFlag = false;
		AIStateTime = 0;
	}

	if (AIStateTime > 100 && AIStateType == AI_STATE_MOVE)
	{
		AIStateType = AI_STATE_ATTACK;
		
		AIStateTime = 0;
	}

	AIStateTime++;
}

void AICharacter::Draw()
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

	// DIR��RIGHT�̏ꍇ�摜�𔽓]
	if (dir == DIR_RIGHT)
	{
		turnFlag = true;
	}
	else
	{
		turnFlag = false;
	}

	if (visible)
	{
		DrawRotaGraph(pos.x, pos.y - 178 / 2, 1.0, 0.0, IMAGE_ID(imageName)[0], true, turnFlag);
	}
	animCnt++;
}

void AICharacter::ChangeState(AIState * s)
{
	state = s;
	state->Init(this);
}

void AICharacter::Move()
{
	if (enemyState.enemyPos < 0)
	{
		return;
	}

	VECTOR2 vec = enemyState.enemyPos - pos;

	int rand = GetRand(10);

	if (abs(vec.x) < 100)
	{
		moveDirFlag = !(abs(vec.x) < 100);
		moveStateTime = 0;
	}
	else if (moveStateTime > 150)
	{

		if (rand == 0)
		{
			moveDirFlag = !moveDirFlag;
			moveStateTime = 0;
		}
	}

	if (moveDirFlag)
	{
		// �O�ɐi��
		if (vec.x < 0)
		{
			pos.x -= 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("���ړ�");
			}
			else
			{
				SetAnim("�O�ړ�");
			}
		}
		else
		{
			pos.x += 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("�O�ړ�");
			}
			else
			{
				SetAnim("���ړ�");
			}

		}
	}
	else
	{
		// ���ɉ�����

		if (vec.x < 0)
		{
			pos.x += 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("�O�ړ�");
			}
			else
			{
				SetAnim("���ړ�");
			}
		}
		else
		{
			pos.x -= 2;

			if (dir == DIR_RIGHT)
			{
				SetAnim("���ړ�");
			}
			else
			{
				SetAnim("�O�ړ�");
			}
		}
	}

	moveStateTime++;
}

bool AICharacter::InitAnim(void)
{
	AddAnim("�ҋ@", 0, 0, 10, 5, true);
	AddAnim("�O�ړ�", 0, 0, 11, 5, true);
	AddAnim("���ړ�", 0, 0, 11, 5, true);
	AddAnim("�W�����v_��", 0, 0, 12, 5, true);
	AddAnim("�W�����v_�O", 0, 0, 14, 5, true);
	AddAnim("�W�����v_���", 0, 0, 14, 5, true);
	AddAnim("���Ⴊ��", 0, 0, 4, 5, true);
	AddAnim("���Ⴊ��_���", 0, 0, 1, 5, true);
	AddAnim("���Ⴊ�ݎn��", 0, 0, 2, 5, false);
	AddAnim("�����n��", 0, 0, 2, 5, false);
	AddAnim("�p���`_��", 0, 0, 4, 5, false);
	AddAnim("�p���`_��", 0, 0, 7, 5, false);
	AddAnim("�K�[�h_���Ⴊ��", 0, 0, 1, 5, true);
	SetAnim("�ҋ@");

	return true;
}

bool AICharacter::Init(std::string fileName, VECTOR2 divSize, VECTOR2 divCut, VECTOR2 pos, bool turn)
{
	Obj::Init(fileName, divSize, divCut, pos, turn);
	InitAnim();

	jumpSpeed = { 0, 0 };
	jumpFlag = false;

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
