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
	if (character->GetAnim() == "ワープ")
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
		character->SetAnim("パンチ_大");
	}
	else if (rand == 1)
	{
		character->SetAnim("パンチ_小");
	}
	else if (rand == 2)
	{
		character->SetAnim("キック_大");
	}
	else if (rand == 3)
	{
		character->SetAnim("キック_小");
	}
	else if (rand == 4)
	{
		character->SetAnim("パンチ_小_しゃがみ");
	}
	else if (rand == 5)
	{
		character->SetAnim("パンチ_大_しゃがみ");
	}
	else if (rand == 6)
	{
		character->SetAnim("キック_小_しゃがみ");
	}
	else if (rand == 7)
	{
		character->SetAnim("キック_大_しゃがみ");
	}

	character->SetDirChange(false);
}

void AttackState::Update(AICharacter * character)
{
	if (character->GetAnimEndFlag())
	{
		character->SetDirChange(true);
		character->SetAnim("待機");
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
