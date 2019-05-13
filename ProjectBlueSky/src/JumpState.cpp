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

void JumpState::Init(AICharacter * character)
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
	auto jumpType = character->GetJumpType();

	// ジャンプ方向フラグ
	bool jumpLeft = true;
	bool jumpRight = true;

	// ジャンプタイプフラグ
	bool jumpTypeLeft = false;
	bool jumpTypeRight = false;

	int rand = 0;

	if (jumpType == JUMP_TYPE_RAND)
	{
		rand = GetRand(10);

		// 前にジャンプするように向いている方向以外をfalse
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = false;
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = false;
		}
	}
	else if (jumpType == JUMP_TYPE_FRONT)
	{
		// 前にジャンプするように向いている方向以外をfalse
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = false;
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = false;
		}

		rand = 10;
	}
	else if (jumpType == JUMP_TYPE_BACK)
	{

	}

	if (!jumpFlag)
	{
		if (rand >= 5 && jumpLeft)
		{
			// 右上
			jumpSpeed = { JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;
			character->SetDirChange(false);

			if (charaDir == DIR_LEFT)
			{
				character->SetAnim("ジャンプ_後ろ");
			}
			else
			{
				character->SetAnim("ジャンプ_前");
			}
		}
		else if (rand >= 5 && jumpRight)
		{
			// 左上
			jumpSpeed = { -JUMP_SPEED_X, -JUMP_SPEED_Y };
			jumpFlag = true;
			character->SetDirChange(false);

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
			character->SetDirChange(false);
			character->SetAnim("ジャンプ_上");
		}
	}
	else
	{
		jumpSpeed.y += 1;
		pos += jumpSpeed;
	}

	auto ssize = lpSceneMng.GetScreenSize();

	// 着地
	if (pos.y > ssize.y)
	{
		pos.y = ssize.y;
		jumpFlag = false;
		character->SetDirChange(true);
		character->ChangeState(MoveState::GetInstance());
	}

	// ジャンプ中に画面外にいったら戻す
	if (jumpFlag && (pos.x > ssize.x || pos.y < 0))
	{
		pos.x -= jumpSpeed.x;
	}

	character->SetPos(pos);

	auto distance = enemy.enemyPos - pos;

	if (abs(distance.x) < 200 && jumpSpeed.y > -10)
	{
		character->SetAnim("キック_大_空中");
	}
}
