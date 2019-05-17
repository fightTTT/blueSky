#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"
#include "GuardState.h"
#include "Collision.h"
#include "DamageState.h"
#include "WaitState.h"

#include "DxLib.h"

#define MOVE_SPEED (4)
#define ATTACK_RANGE (180)

#define WAITSTATE_INV (60)

MoveState::MoveState()
{
	// �ŏ�����������
	moveDirFlag = true;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * character)
{
	changeGuardStateCount = 0;
	stateTime = 0;
	notAttackCount = 0;
	moveFrontCount = 0;
	changeWaitStateCount = 0;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand = 0;

	if (character->GetAnimAttribute(1) == ANIM_ATTRIBUTE_GUARD)
	{
		character->ChangeState(GuardState::GetInstance());
		moveDirFlag = true;
		return;
	}

	changeWaitStateCount++;

	if (moveDirFlag)
	{
		// �O�ړ������Ă���ꍇ�J�E���g�����Z
		moveFrontCount++;
	}
	else
	{
		moveFrontCount = 0;
	}

	// �U�������Ă��Ȃ���ԂőO�Ɉړ��������Ă������Ԃ�ύX
	if (moveFrontCount > 40 && notAttackCount > 80)
	{
		if (changeWaitStateCount > WAITSTATE_INV)
		{
			character->ChangeState(WaitState::GetInstance());
			changeWaitStateCount = 0;
			moveDirFlag = true;
			return;
		}
	}

	// ���ړ��ŋ������͂Ȃꂷ���Ă������Ԃ�ύX
	if (!moveDirFlag && (abs(vec.x) > 300))
	{
		if (changeWaitStateCount > WAITSTATE_INV)
		{
			character->ChangeState(WaitState::GetInstance());
			changeWaitStateCount = 0;
			moveDirFlag = true;
			return;
		}
	}

	// �����_���ŕ������t�����ɐ؂�ւ�
	if (!(abs(vec.x) < ATTACK_RANGE - 80) && GetRand(100) == 0)
	{
		moveDirFlag = !moveDirFlag;
	}

	// �G�Ƃ̋������߂���ԂœG���U�����Ă����ꍇ�K�[�h�����邽�߂Ɍ��ړ��ɐ؂�ւ���
	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_ATTACK))
	{
		moveDirFlag = false;
	}

	// �ߋ����U���������鋗���̏ꍇ�U��
	if (abs(vec.x) < ATTACK_RANGE - 80)
	{
		rand = GetRand(100);
		if (rand <= 5)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState(JumpState::GetInstance());
			moveDirFlag = true;
			return;
		}
		character->ChangeState(AttackState::GetInstance());
		notAttackCount = 0;
		return;
	}
	// �����������ꍇ
	else if (abs(vec.x) > 250)
	{
		rand = GetRand(100);

		if (rand == 0 && notAttackCount > 180)
		{
			character->ChangeState(LongAttackState::GetInstance());
			notAttackCount = 0;
			return;
		}
	}

	// �U�����Ă��Ȃ���Ԃ��������ꍇ
	if (notAttackCount >= 180)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		notAttackCount = 0;
		moveDirFlag = true;
		return;
	}

	notAttackCount++;

	// �e���W�����v���
	for (auto data : enemy.shotData)
	{
		// �������������e�̏ꍇ
		if (data.id == character->GetPadID())
		{
			continue;
		}

		auto distance = pos - data.pos;

		// �e���������O�ɂ���ꍇ���s
		if ((charaDir == DIR_RIGHT && distance.x > -divSize.x) || (charaDir == DIR_LEFT && distance.x < divSize.x))
		{
			if (data.pos.x > 0 && abs(distance.x) < 200)
			{
				character->SetJumpType(JUMP_TYPE_RAND);
				character->ChangeState(JumpState::GetInstance());
				moveDirFlag = true;
				return;
			}
		}
		else
		{
			continue;
		}
	}

	// ��ʒ[�ɋ߂Â����������؂�ւ���
	VECTOR2 ssize = lpSceneMng.GetScreenSize();
	int screenDistance = 0;

	if (charaDir == DIR_RIGHT)
	{
		screenDistance = pos.x;
	}
	else
	{
		screenDistance = abs(pos.x - ssize.x);
	}

	if (abs(vec.x < 250) && screenDistance < 200)
	{
		rand = GetRand(1);
		if (rand == 0)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState(JumpState::GetInstance());
			moveDirFlag = true;
			return;
		}
		else
		{
			moveDirFlag = true;
		}
	}

	stateTime++;

	// �K�[�h��Ԃ̏ꍇ�͈ړ����Ȃ�
	if (character->GetAnimAttribute(1) == ANIM_ATTRIBUTE_GUARD)
	{
		return;
	}

	// �ړ�����
	if (moveDirFlag)
	{
		// �O�ɐi��
		if (vec.x < 0)
		{
			pos.x -= MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("���ړ�");
			}
			else
			{
				character->SetAnim("�O�ړ�");
			}
		}
		else
		{
			pos.x += MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("�O�ړ�");
			}
			else
			{
				character->SetAnim("���ړ�");
			}

		}
	}
	else
	{
		// ���ɉ�����

		if (vec.x < 0)
		{
			pos.x += MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("�O�ړ�");
			}
			else
			{
				character->SetAnim("���ړ�");
			}
		}
		else
		{
			pos.x -= MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("���ړ�");
			}
			else
			{
				character->SetAnim("�O�ړ�");
			}
		}
	}

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
}
