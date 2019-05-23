#include "AttackState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "Collision.h"
#include "DamageState.h"
#include "JumpState.h"
#include "SceneMng.h"

#include "DxLib.h"

AttackState::AttackState()
{
	attackCount = 0;
	warpFlag = false;
}

AttackState::~AttackState()
{
}

void AttackState::Init(AICharacter * character)
{
	if (character->GetAnim() == "���[�v")
	{
		warpFlag = true;
		character->SetDirChange(false);
		return;
	}
	else
	{
		warpFlag = false;
	}

	if (attackCount >= static_cast<unsigned int>(GetRand(30) + 4))
	{
		attackCount = 0;
		character->SetJumpType(JUMP_TYPE_FRONT);
		character->ChangeState(JumpState::GetInstance());
		return;
	}
	else
	{
		attackCount++;
	}

	int rand = GetRand(7);

	if (rand == 0)
	{
		character->SetAnim("�p���`_��");
	}
	else if (rand == 1)
	{
		character->SetAnim("�p���`_��");
	}
	else if (rand == 2)
	{
		character->SetAnim("�L�b�N_��");
	}
	else if (rand == 3)
	{
		character->SetAnim("�L�b�N_��");
	}
	else if (rand == 4)
	{
		character->SetAnim("�p���`_��_���Ⴊ��");
	}
	else if (rand == 5)
	{
		character->SetAnim("�p���`_��_���Ⴊ��");
	}
	else if (rand == 6)
	{
		character->SetAnim("�L�b�N_��_���Ⴊ��");
	}
	else if (rand == 7)
	{
		character->SetAnim("�L�b�N_��_���Ⴊ��");
	}

	character->SetDirChange(false);
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->SetDirChange(true);
		character->SetAnim("�ҋ@");
		character->ChangeState(MoveState::GetInstance());
		return;
	}

	auto ssize = lpSceneMng.GetScreenSize();
	auto pos = character->GetPos();
	auto dir = character->GetDir();

	if (warpFlag)
	{
		if (character->GetFrame() == 6)
		{
			character->SetAnimStopFlag(true);
		}

		if (character->GetAnimStopFlag())
		{
			pos.y = (ssize.y * 2);

			if (dir == DIR_RIGHT)
			{
				if (pos.x > ((ssize.x * 3) / 5))
				{
					dir = character->GetTmpDir();
					character->SetAnimStopFlag(false);
				}
				else
				{
					pos.x += 30;
				}
			}
			else
			{
				if (pos.x < ((ssize.x * 2) / 5))
				{
					dir = character->GetTmpDir();
					character->SetAnimStopFlag(false);
				}
				else
				{
					pos.x -= 30;
				}
			}
		}
	}

	character->SetPos(pos);
}
