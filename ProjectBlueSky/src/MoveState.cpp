#include "MoveState.h"
#include "AICharacter.h"
#include "SceneMng.h"
#include "JumpState.h"
#include "LongAttackState.h"

#include "DxLib.h"

#define MOVE_SPEED (2)

MoveState::MoveState()
{
}

MoveState::~MoveState()
{
}

void MoveState::Init(AICharacter * chara)
{
	moveDirFlag = true;
	stateTime = 0;
}

void MoveState::Update(AICharacter * character)
{
	auto enemy = character->GetEnemyState();
	auto pos = character->GetPos();
	auto charaDir = character->GetDir();
	auto divSize = character->GetDivSize();

	for (auto data : enemy.shotData)
	{
		auto distance = pos - data.pos;

		// 弾が自分より前にある場合実行
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

	VECTOR2 vec = enemy.enemyPos - pos;

	int rand = GetRand(99);

	if (abs(vec.x) < 100)
	{
		moveDirFlag = !(abs(vec.x) < 100);
		stateTime = 0;
	}
	else if (stateTime > 150)
	{
		// 150カウントの状態で1/100の確率で方向を切り替える
		if (rand == 0)
		{
			moveDirFlag = !moveDirFlag;
			stateTime = 0;
		}
	}

	if (moveDirFlag)
	{
		// 前に進む
		if (vec.x < 0)
		{
			pos.x -= MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("後ろ移動");
			}
			else
			{
				character->SetAnim("前移動");
			}
		}
		else
		{
			pos.x += MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("前移動");
			}
			else
			{
				character->SetAnim("後ろ移動");
			}

		}
	}
	else
	{
		// 後ろに下がる

		if (vec.x < 0)
		{
			pos.x += MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("前移動");
			}
			else
			{
				character->SetAnim("後ろ移動");
			}
		}
		else
		{
			pos.x -= MOVE_SPEED;

			if (charaDir == DIR_RIGHT)
			{
				character->SetAnim("後ろ移動");
			}
			else
			{
				character->SetAnim("前移動");
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
