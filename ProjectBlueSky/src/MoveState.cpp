#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"

#include "DxLib.h"

#define MOVE_SPEED (2)

MoveState::MoveState()
{
	// �ŏ�����������
	changeAttackFlag = false;
	initFlag = false;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * chara)
{
	if (initFlag)
	{
		auto pos = chara->GetPos();
		auto enemy = chara->GetEnemyState();

		auto distance = enemy.enemyPos - pos;

		if (abs(distance.x) < 100)
		{
			// �G�Ƌ������߂��ꍇ�͂Ȃ��
			moveDirFlag = false;
		}
		else
		{
			moveDirFlag = true;
		}

		stateTime = 0;

		initFlag = true;
	}
	moveDirFlag = true;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand;

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

	if (!changeAttackFlag)
	{
		int rand = GetRand(200);

		changeAttackFlag = rand == 0;
	}

	auto ssize = lpSceneMng.GetScreenSize();

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
	stateTime++;

	// �������Čv�Z
	vec = enemy.enemyPos - pos;

	// �ߋ����U���������鋗���̏ꍇ�U��
	if (abs(vec.x) < 100)
	{
		int rand = GetRand(5);

		if (rand == 0)
		{
			character->ChangeState(AttackState::GetInstance());
			changeAttackFlag = false;
			return;
		}

	}

	// �e���W�����v���
	for (auto data : enemy.shotData)
	{
		auto distance = pos - data.pos;

		// �e���������O�ɂ���ꍇ���s
		if ((charaDir == DIR_RIGHT && distance.x > -divSize.x) || (charaDir == DIR_LEFT && distance.x < divSize.x))
		{
			if (data.pos.x > 0 && abs(distance.x) < 200)
			{
				character->SetShotJumpFlag(true);
				character->ChangeState(JumpState::GetInstance());
				return;
			}
		}
		else
		{
			continue;
		}
	}

	rand = GetRand(99);

	if (abs(vec.x) < 100)
	{
		moveDirFlag = !(abs(vec.x) < 100);
		stateTime = 0;
	}
	else if (stateTime > 150)
	{
		// 150�J�E���g�̏�Ԃ�1/100�̊m���ŕ�����؂�ւ���
		if (rand == 0)
		{
			moveDirFlag = !moveDirFlag;
			stateTime = 0;
		}

		// 150�J�E���g�̏�Ԃ�1/100�̊m���ŃW�����v
		if (rand == 1)
		{
			stateTime = 0;
			character->ChangeState(JumpState::GetInstance());
			return;
		}
	}
}
