#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "Collision.h"

#include "DxLib.h"

#define MOVE_SPEED (4)
#define ATTACK_RANGE (180)

#define WAITSTATE_INV (60)

MoveState::MoveState()
{
	// �ŏ�����������
	moveDirFlag = true;
	guardHitCount = 0;
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * character)
{
	changeGuardStateCount = 0;
	stateTime = 0;
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
	notAttackCount++;

	if (guardHitCount >= 50)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState("Jump");
		guardHitCount = 0;
		moveDirFlag = true;
		return;
	}

	if (character->GetAnimAttribute(1) == ANIM_ATTRIBUTE_GUARD)
	{
		character->ChangeState("Guard");
		guardHitCount++;
		moveDirFlag = true;
		return;
	}

	if ( static_cast<int>(changeGuardCount) > GetRand(30))
	{
		moveDirFlag = false;
		changeGuardCount = 0;
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
			character->ChangeState("Wait");
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
			character->ChangeState("Wait");
			changeWaitStateCount = 0;
			moveDirFlag = true;
			return;
		}
	}

	// �����_���ŕ������t�����ɐ؂�ւ�
	if (!(abs(vec.x) < ATTACK_RANGE - GetRand(200)) && GetRand(50) == 0)
	{
		moveDirFlag = !moveDirFlag;
	}

	if (abs(vec.x) < ATTACK_RANGE
	 && ((enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_ATTACK_SMALL)
	 || (enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_ATTACK_BIG)
	 || (enemy.enemyAnimAttribute[1] == ANIM_ATTRIBUTE_ATTACK_SP)))
	{
		changeGuardCount++;
	}
	else
	{
		changeGuardCount = 0;
	}

	// �ߋ����U���������鋗���̏ꍇ�U��
	if (abs(vec.x) < ATTACK_RANGE - GetRand(120))
	{
		rand = GetRand(100);
		if (rand >= 95)
		{
			character->SetJumpType(JUMP_TYPE_FRONT);
			character->ChangeState("Jump");
			moveDirFlag = true;
			return;
		}
		else if (rand <= 10)
		{
			// �ߋ����̕K�E�Z�̂ݎ��s
			for (int i = 1; i < 3; ++i)
			{
				if (!(character->isSPLongAttack(character->GetSPAttackName(i))))
				{
					rand = GetRand(2);
					if (rand == 0)
					{
						character->SetAnim(character->GetSPAttackName(i));
						character->ChangeState("Attack");
						notAttackCount = 0;
						return;
					}
				}
			}
		}
		character->ChangeState("Attack");
		notAttackCount = 0;
		return;
	}

	// �U�����Ă��Ȃ���Ԃ��������ꍇ
	if (notAttackCount >= 120)
	{
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState("Jump");
		notAttackCount = 0;
		moveDirFlag = true;
		return;
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
				character->ChangeState("Jump");
				moveDirFlag = true;
				return;
			}
		}
		else
		{
			continue;
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

	VECTOR2 ssize = lpSceneMng.GetScreenSize();

	if (pos.x > ssize.x - divSize.x / 4)
	{
		pos.x -= MOVE_SPEED;
	}

	character->SetPos(pos);
}
