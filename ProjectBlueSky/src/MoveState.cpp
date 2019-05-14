#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "AttackState.h"
#include "LongAttackState.h"
#include "GuardState.h"

#include "DxLib.h"

#define MOVE_SPEED (2)
#define ATTACK_RANGE (150)

MoveState::MoveState()
{
	// �ŏ�����������
	initFlag = false;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * character)
{
	if (!initFlag)
	{
		moveDirFlag = true;
		initFlag = true;
	}

	stateTime = 0;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	VECTOR2 vec = enemy.enemyPos - pos;
	int rand;

	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimName == "�L�b�N_��" || enemy.enemyAnimName == "�L�b�N_��"))
	{
		character->SetAnim("�K�[�h_����");
		character->ChangeState(GuardState::GetInstance());
		return;
	}

	if (abs(vec.x) < ATTACK_RANGE && (enemy.enemyAnimName == "�L�b�N_��_���Ⴊ��" || enemy.enemyAnimName == "�L�b�N_��_���Ⴊ��"))
	{
		character->SetAnim("�K�[�h_���Ⴊ��");
		character->ChangeState(GuardState::GetInstance());
		return;
	}

	// �ߋ����U���������鋗���̏ꍇ�U��
	if (abs(vec.x) < 100)
	{
		rand = GetRand(5);

		if (rand == 0)
		{
			character->SetAnim("�p���`_��");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
		else if (rand == 1)
		{
			character->SetAnim("�p���`_��");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
		else if (rand == 2)
		{
			character->SetAnim("�L�b�N_��");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
		else if (rand == 3)
		{
			character->SetAnim("�L�b�N_��");
			character->ChangeState(AttackState::GetInstance());
			return;
		}
	}
	// �����������ꍇ
	else if (abs(vec.x) > 250)
	{
		rand = GetRand(500);

		if (rand == 0)
		{
			character->ChangeState(LongAttackState::GetInstance());
			return;
		}
	}

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

	rand = GetRand(500);

	// �ړ������ɃW�����v
	if (moveDirFlag)
	{
		if (rand == 0)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState(JumpState::GetInstance());
			return;
		}
	}
	else
	{
		if (rand == 0)
		{
			character->SetJumpType(JUMP_TYPE_BACK);
			character->ChangeState(JumpState::GetInstance());
			return;
		}
	}

	//if (abs(vec.x) < 100)
	//{
	//	moveDirFlag = !(abs(vec.x) < 100);
	//	stateTime = 0;
	//}

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
