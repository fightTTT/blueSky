#include "JumpState.h"
#include "AICharacter.h"
#include "MoveState.h"
#include "SceneMng.h"
#include "Collision.h"
#include "DamageState.h"

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

	int randLeft = 0;
	int randRight = 0;

	if (jumpType == JUMP_TYPE_RAND)
	{
		// 前にジャンプするように向いている方向以外をfalse
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = false;
			randRight = GetRand(10);
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = false;
			randLeft = GetRand(10);
		}
	}
	else if (jumpType == JUMP_TYPE_FRONT)
	{
		// 前にジャンプするように向いている方向以外をfalse
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = false;
			randRight = 10;
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = false;
			randLeft = 10;
		}
	}
	else if (jumpType == JUMP_TYPE_BACK)
	{
		// 後ろにジャンプするように向いている方向をtrue
		if (charaDir == DIR_LEFT)
		{
			jumpLeft = true;
			randLeft = 10;
		}
		else if (charaDir == DIR_RIGHT)
		{
			jumpRight = true;
			randRight = 10;
		}
	}
	else if (jumpType == JUMP_TYPE_UP)
	{
		randLeft = 0;
		randRight = 0;
	}

	if (!jumpFlag)
	{
		if (randLeft >= 7 && jumpLeft)
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
		else if (randRight >= 7 && jumpRight)
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
	if (jumpSpeed.y == JUMP_SPEED_Y)
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
		if (distance.x < 0)
		{
			character->SetDir(DIR_LEFT);
		}
		else
		{
			character->SetDir(DIR_RIGHT);
		}
		character->SetAnim("キック_大_空中");
	}
}
