#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"
#include "GuardState.h"
#include "Collision.h"
#include "DamageState.h"

#include "DxLib.h"

#define MOVE_SPEED (2)
#define ATTACK_RANGE (180)

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
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand = 0;

	if (GetRand(100) == 0)
	{
		moveDirFlag = true;
	}

	if (character->GetAnim() == "�K�[�h_����" && character->GetAnim() == "�K�[�h_���Ⴊ��")
	{
		character->ChangeState(GuardState::GetInstance());
		return;
	}

	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimName == "�L�b�N_��" || enemy.enemyAnimName == "�L�b�N_��"))
	{
		moveDirFlag = false;
	}
	else if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimName == "�L�b�N_��_���Ⴊ��" || enemy.enemyAnimName == "�L�b�N_��_���Ⴊ��"))
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

	if (notAttackCount >= 180)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		notAttackCount = 0;
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
				return;
			}
		}
		else
		{
			continue;
		}
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

	auto ssize = lpSceneMng.GetScreenSize();

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
	stateTime++;
}

void MoveState::CheckHitFlag(AICharacter * character)
{
	auto dir = character->GetDir();
	auto hitData = character->GetHitData();
	auto anim = character->GetAnim();

	auto hitFlag = hitData.hitFlag && hitData.colType == COLTYPE_HIT;

	if (hitFlag && !(anim == "�_���[�W_����" ) && !(anim == "�_���[�W_�_�E��"))
	{
		character->SetAnim("�_���[�W_����");
		character->ChangeState(DamageState::GetInstance());
	}
}
