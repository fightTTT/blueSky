#include "JumpState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "SceneMng.h"

#include "DxLib.h"

#define JUMP_SPEED_X (4)
#define JUMP_SPEED_Y (30)

JumpState::JumpState()
{
}

JumpState::~JumpState()
{
}

void JumpState::Init(AICharacter * chara)
{
	stateTime = 0;
	jumpSpeed = { 0, 0 };
	jumpFlag = false;
}

void JumpState::Update(AICharacter * character)
{
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto enemy = character->GetEnemyState();

	bool shotJumpLeft = true;
	bool shotJumpRight = true;
	if (charaDir == DIR_LEFT)
	{
		shotJumpLeft = !character->GetShotJumpFlag();
	}
	if (charaDir == DIR_RIGHT)
	{
		shotJumpRight = !character->GetShotJumpFlag();
	}

	if (!jumpFlag)
	{
		int randRight = GetRand(10);
		int randLeft = GetRand(10);

		if (randRight <= 5 && shotJumpLeft)
		{
			// 右上
			jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;

			if (charaDir == DIR_LEFT)
			{
				character->SetAnim("ジャンプ_後ろ");
			}
			else
			{
				character->SetAnim("ジャンプ_前");
			}
		}
		else if (randLeft <= 5 && shotJumpRight)
		{
			// 左上
			jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;

			if (charaDir == DIR_LEFT)
			{
				character->SetAnim("ジャンプ_前");
			}
			else
			{
				character->SetAnim("ジャンプ_後ろ");
			}
		}
		else
		{
			// 上
			jumpSpeed = { 0, -JUMP_SPEED_Y };
			jumpFlag = true;
			character->SetAnim("ジャンプ_上");
		}
	}
	else
	{
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	character->SetShotJumpFlag(false);

	auto ssize = lpSceneMng.GetScreenSize();

	if (pos.y > ssize.y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
		character->ChangeState(MoveState::GetInstance());
	}

	character->SetPos(pos);

	auto distance = enemy.enemyPos - pos;

	if (abs(distance.x) < 200 && jumpSpeed.y > -10)
	{
		character->SetAnim("キック_大_空中");
	}
}
